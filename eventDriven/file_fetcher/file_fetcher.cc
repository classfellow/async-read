#include "StdAfx.h"
#include "file_fetcher.h"
#include "../base/common_threads.h"
#include "../base/macros.h"
#include "../base/FastDelegateImpl.h"
#include "../base/notification_service.h"
#include "../base/macros.h"
#include "../base/ref_counted.h"
#include "../Event/WaitableEvent.h"
#include "../base/MessagePumpWin.h"
#include "../base/macros.h"
#include "../Event/WaitableEvent.h"
#include "../base/MessageLoop.h"
#include "../base/notification_observer.h"
#include "../base/notification_registrar.h"
#include "../time/time.h"
#include "../base/aligned_memory.h"
#include "../base/lazy_instance.h"
#include "../base/ref_counted.h"
#include "../base/WrapperObj.h"
#include "../base/WeakPtr.h"
#include "../base/FastDelegateImpl.h"
#include "../base/MessagePump.h"
#include "../util/util_tools.h"
#include "../Event/WaitableEvent.h"
#include "../base/PendingTask.h"
#include "../base/observer_list.h"
#include "../base/MessagePumpDefault.h"
#include "../base/MessagePumpWin.h"
#include "../base/MessageLoop.h"
#include "../base/Thread.h"
#include "../base/at_exist.h"
#include "../base/common_threads.h"
#include "../base/notification_service.h"

namespace file{
  //static
  FileFetcher* FileFetcher::Create(const Params& params, FileFetcherDelegate* delegate) {
    return (new FileFetcher(params, delegate));
  }

  void FileFetcher::FileFetcherDelegate::OnFileProgress(const FileFetcher* source, float cur) {
  }

  FileFetcher::FileFetcherDelegate::~FileFetcherDelegate() {
  }

  FileFetcher::FileFetcher(const Params& params, FileFetcherDelegate* delegate)
    :file_name_(params.name_)
    ,delegate_(delegate)
    ,input_state_(this)
    ,file_handle_(NULL)
    ,dwSize_(0)
    ,dwReaded(0)
    ,status_(false)
    ,dwErrorCode_(0){
      io_port_ = base::Threads::Get(Threads::IO)->pump_win()->io_port();
      read_ptr = 0;
      assert(io_port_);
      data_.resize(FileFetcher::s_buf_len + 1);
      memset(const_cast<char*>(data_.c_str()), 0, data_.length());
  }

  void FileFetcher::Init() {

  }

  void FileFetcher::Start() {
    base::Threads::Get(Threads::IO)->PostTask(Bind(this, &FileFetcher::StartInternal));
  }

  FileFetcher::~FileFetcher() {
    registrar_.RemoveAll();
    Stop();
  }

  void FileFetcher::Stop() {
    assert(base::MessageLoop::current() == base::Threads::Get(Threads::UI));
    delegate_ = NULL;
  }

  void FileFetcher::StartInternal() {
    base::Threads::Get(Threads::IO)->AddDestructionObserver(this);
    assert(file_handle_);
    //OVERLAPPED非阻塞调用，ReadFile立即返回。操作系统将数据写入buffer 后通知
    ::ReadFile(file_handle_, const_cast<char*>(data_.c_str())
      , FileFetcher::s_buf_len
      , NULL
      , &(input_state_.context.overlapped)
      );
  }

  void FileFetcher::FetchComplete() {
    if(NULL != delegate_) {
      delegate_->OnFileFetchComplete(this);
    }
  }

  void FileFetcher::FetchProgress(float cur) {
  }

  void FileFetcher::WillDestroyCurrentMessageLoop() {
    if(delegate_) {
      base::Threads::Get(Threads::UI)->PostTask(Bind(this, &FileFetcher::FetchComplete));
    }
  }

  void FileFetcher::OnIOCompleted(base::MessagePumpForIO::IOContext* context,
    DWORD bytes_transfered,
    DWORD error) {
    base::Threads::Get(Threads::IO)->RemoveDestructionObserver(this);

    if(delegate_) {
      assert(context->handler == this);
      status_ = !error;
      base::Threads::Get(Threads::UI)->PostTask(Bind(this, &FileFetcher::FetchComplete));
      if(!status_){
        dwErrorCode_ = error;
      }
    }
  }

  void FileFetcher::Observe(int type,
    const base::NotificationSource& source,
    const base::NotificationDetails& details){
      CHECK(MessageLoop::current() == base::Threads::Get(Threads::UI));
      switch(type) {
        default:
          break;
      } //end switch
  }

  void FileFetcher::AddObserver(FileFetcherDelegate *content ,int event){
    assert(base::Threads::Get(Threads::UI) == base::MessageLoop::current());
    registrar_.Add(
      this, event,
      base::Source<FileFetcherDelegate>(content));
  }
} //end file
