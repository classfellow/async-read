/*
* @author   classfellow@qq.com
* @bf       Read file using FILE_FLAG_OVERLAPPED
**
****************************************************************************************/

#pragma once
#include "../base/MessagePumpWin.h"
#include "../base/macros.h"
#include "../Event/WaitableEvent.h"
#include "../base/MessageLoop.h"
#include "../base/notification_observer.h"
#include "../base/notification_registrar.h"

#define READ_MAX_LEN 500

namespace file {
  class FileFetcher : public base::MessagePumpForIO::IOHandler,
    public base::MessageLoop::DestructionObserver,
    public base::NotificationObserver,
    public base::RefCountedThreadSafe<FileFetcher> {
  public:
    const static int s_buf_len = READ_MAX_LEN;
    class FileFetcherDelegate {
    public:
      virtual void OnFileFetchComplete(const FileFetcher* source) = 0;
      virtual void OnFileProgress(const FileFetcher* source,
        float cur);
      virtual ~FileFetcherDelegate();
    };


    struct Params {
      explicit Params(const std::wstring &name):name_(name) {
      }
      std::wstring name_;
    };

    struct State {
      explicit State(FileFetcher* fetcher) {
        context.handler = fetcher;
        memset(&(context.overlapped), 0, sizeof(context.overlapped));
      }
      ~State(){}
      base::MessagePumpForIO::IOContext context;
    };

    static FileFetcher* Create(const Params& params, FileFetcherDelegate* delegate);

    ~FileFetcher();

    void Start();
    void Stop();

    virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

    virtual void OnIOCompleted(base::MessagePumpForIO::IOContext* context, DWORD bytes_transfered,
      DWORD error) OVERRIDE;

    void FetchComplete();
    void FetchProgress(float cur);

    HANDLE io_port() { return io_port_; }
    bool status() const { return status_; }
    void set_status(bool status) { status_ = status; }

    State &state() { return input_state_; }
    std::string & data() { return data_; }
    void AddObserver(FileFetcherDelegate *content, int event);
    void setFileHandle(HANDLE handle){ file_handle_ = handle;}
    DWORD ErrorCode() { return dwErrorCode_; }
  private:


    explicit FileFetcher(const Params& params, FileFetcherDelegate* delegate);

    void Init();
    virtual void Observe(int type,
      const base::NotificationSource& source,
      const base::NotificationDetails& details) OVERRIDE;

    void StartInternal();

    std::wstring file_name_;
    std::string data_;
    int read_ptr;
    HANDLE file_handle_;
    HANDLE io_port_;
    State input_state_;
    FileFetcherDelegate * delegate_;
    bool status_;
    base::NotificationRegistrar registrar_;
    DWORD dwSize_;
    DWORD dwReaded;
    DWORD dwErrorCode_;
  public:

  };
} // end file
