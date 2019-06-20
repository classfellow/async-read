/*
author  Banz
*/


#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include "base/macros.h"
#include "time/time.h"
#include "base/aligned_memory.h"
#include "base/lazy_instance.h"
#include "base/ref_counted.h"
#include "base/WrapperObj.h"
#include "base/WeakPtr.h"
#include "base/FastDelegateImpl.h"
#include "base/MessagePump.h"
#include "util/util_tools.h"
#include "event/WaitableEvent.h"
#include "base/PendingTask.h"
#include "base/observer_list.h"
#include "base/MessagePumpDefault.h"
#include "base/MessagePumpWin.h"
#include "base/MessageLoop.h"
#include "base/Thread.h"
#include "base/at_exist.h"
#include "base/common_threads.h"
#include "file_fetcher/file_fetcher.h"


WCHAR *filec = L"CreateIoCompletionPort\n\n"
L"The CreateIoCompletionPort function can associate an instance of an opened file with a newly created or an existing\n"
L"input/output (I/O) completion port; or it can create an I/O completion port without associating it with a file.\n\n"
L"Associating an instance of an opened file with an I/O completion port lets an application receive notification of the\n"
L"completion of asynchronous I/O operations involving that file.\n\n"
L"HANDLE CreateIoCompletionPort (\n"
L"  HANDLE FileHandle,     // file handle to associate with the I/O completion port\n"
L"  HANDLE ExistingCompletionPort,  // handle to the I/O completion port\n"
L"  DWORD CompletionKey,            // per-file completion key for I/O completion packets\n"
L"  DWORD NumberOfConcurrentThreads // number of threads allowed to execute concurrently\n"
L");"
L"Remarks\n"
L"The I/O system can be instructed to send I/O completion notification packets to I/O completion ports, where they are queued. The CreateIoCompletionPort function provides this functionality.\n"
L"After an instance of an open file is associated with an I/O completion port, it cannot be used in the ReadFileEx or WriteFileEx function. It is best not to share such an\n"
L"associated file through either handle inheritance or a call to the DuplicateHandle function. Operations performed with such duplicate handles will generate completion\n"
L"notifications.\n\n"
L"When you perform an I/O operation with a file handle that has an associated I/O completion port, the I/O system sends a completion notification packet to the \n"
L"completion port when the I/O operation completes. The I/O completion port places the completion packet in a first-in-first-out queue. Use the \n"
L"GetQueuedCompletionStatus function to retrieve these queued I/O completion packets.\n";

base::LazyInstance<base::Thread> g_io_thread = LAZY_INSTANCE_INITIALIZER;

#define FILENAME L"\\test.real"

class FetchFileDelegate {
public:
  virtual void FetchFileComplete(bool status, void* obj) = 0;
  virtual void FetchFileProgress(float cur) = 0;
};

class CFetchFile:public file::FileFetcher::FileFetcherDelegate {
public:
  explicit CFetchFile(FetchFileDelegate *delegate, std::wstring name) {
    delegate_ = delegate;
    fetcher_ = file::FileFetcher::Create(file::FileFetcher::Params(name), this);
  }
  ~CFetchFile() {}
  virtual void OnFileFetchComplete(const file::FileFetcher* source) OVERRIDE {
    delegate_->FetchFileComplete(source->status(), this);
  }
  scoped_refptr<file::FileFetcher> fetcher_;
private:
  FetchFileDelegate * delegate_;
};

class Reader:public base::RefCountedThreadSafe<Reader>,
             public FetchFileDelegate {
public:
  static bool s_readed;
  static Reader* s_reader;
  explicit Reader(std::wstring name): fetcher_(this, name),
  handle_(NULL), file_name_(name) {
  }
  void ReadFile();
  virtual void FetchFileComplete(bool status, void* obj) OVERRIDE {
    if(status) {
      std::cout<<reinterpret_cast<CFetchFile*>(obj)->fetcher_->data()<<endl;
    } else {
      std::cout<<"read error!\n";
    }
    s_readed = true;
  }

  virtual void FetchFileProgress(float cur) OVERRIDE {
  }
  CFetchFile fetcher_;
  HANDLE handle_;
  std::wstring file_name_;
};

bool Reader::s_readed = false;
Reader * Reader::s_reader = NULL;

void Reader::ReadFile() {
  if(!handle_) {
    //UI中打开文件，ReadFile 采用异步
    handle_ = ::CreateFileW(file_name_.c_str(),  GENERIC_READ
      , FILE_SHARE_READ
      , NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    if(handle_ == INVALID_HANDLE_VALUE) {
      handle_ = NULL;
    } else {
      g_io_thread.Get().message_loop()->pump_io()->RegisterIOHandler(handle_,
        this->fetcher_.fetcher_.get());
    }
  }

  if(!handle_) return;
  fetcher_.fetcher_->setFileHandle(handle_);
  //此处发起异步io请求,读文件
  fetcher_.fetcher_->Start();
}

void init() {
  if(!g_io_thread.Get().IsRunning()) {
    g_io_thread.Get().set_thread_name("io_thread");
    g_io_thread.Get().StartWithOptions(base::Thread::Options(base::MessageLoop::TYPE_IO));
    base::Threads::Set(base::Threads::IO, g_io_thread.Get().message_loop());
  }
}

//程序退出
void unInit() {
  g_io_thread.Get().Stop();
};

//请求读文件
void Run() {
  base::Threads::Get(Threads::UI)->PostTask(Bind(Reader::s_reader, &Reader::ReadFile));
}

void CheckQuit() {
  if(Reader::s_readed) {
    Reader::s_reader = NULL;
    base::Threads::Get(Threads::UI)->Quit();
  } else {
    base::Threads::Get(Threads::UI)->PostDelayedTask(base::Bind(CheckQuit),
      base::TimeDelta::FromSeconds(1));
  }
}

int _tmain(int argc, _TCHAR* argv[]) {
  base::AtExitManager atmgr;

  std::wstring strlogpath = L"";
  TCHAR szPath[MAX_PATH + 1] = {0};
  GetModuleFileName(NULL, szPath, MAX_PATH);
  PathRemoveFileSpec(szPath);
  strlogpath = szPath;
  strlogpath += FILENAME;

  do {
    // 创建一个文件，写入一些测试数据
    FILE * handle = _wfopen(strlogpath.c_str(), L"a");
    if(!handle) std::cout<<"open file error\n";
    fputws(filec, handle);
    fclose(handle);
  } while (false);


  init();
  std::auto_ptr<base::MessageLoop> message_loop;
  message_loop.reset(new base::MessageLoop(base::MessageLoop::TYPE_UI)); // UI 线程，即主线程
  message_loop->set_thread_name("ui_thread");
  base::Threads::Set(Threads::UI, message_loop.get());

  std::cout<<"Hello, welcome to test Event-Driven program! \n\n";

  scoped_refptr<Reader>  pReader = new Reader(strlogpath);
  Reader::s_reader = pReader.get();

  message_loop->PostTask(base::Bind(Run));
  message_loop->PostTask(base::Bind(CheckQuit));
  message_loop->Run();

  unInit();

  system("pause");
	return 0;
}
