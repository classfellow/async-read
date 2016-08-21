/* 
* @author   classfellow@qq.com
* @bf  线程循环委托
**           
****************************************************************************************/

#pragma once
#include "MessagePump.h"
#include "macros.h"
#include "FastDelegateImpl.h"
#include "../util/util_tools.h"
#include "../Event/WaitableEvent.h"
#include "observer_list.h"
#include "PendingTask.h"

namespace base{
  class MessagePumpWin;
  class MessagePumpForIO;

  class MessageLoop: public MessagePump::Delegate {
  public:
    enum Type {
      TYPE_DEFAULT,
      TYPE_UI,
      TYPE_IO,
    };

    template <class T>
    class DeleteHelper {
    public:
      static void DoDelete(const void* object) {
        delete reinterpret_cast<const T*>(object);
      }
      DISALLOW_COPY_AND_ASSIGN(DeleteHelper);
    };

    class DestructionObserver {
    public:
      virtual void WillDestroyCurrentMessageLoop() = 0;

    protected:
      virtual ~DestructionObserver() {}
    };
    void AddDestructionObserver(DestructionObserver* destruction_observer);

    // Remove a DestructionObserver.  It is safe to call this method while a
    // DestructionObserver is receiving a notification callback.
    void RemoveDestructionObserver(DestructionObserver* destruction_observer);

    explicit MessageLoop(Type type = TYPE_DEFAULT);

    virtual ~MessageLoop();
    //static
    static MessageLoop* current();

    void PostTask(fastdelegate::Task<void>* task);
    void PostDelayedTask(fastdelegate::Task<void>* task, TimeDelta delay);
    void PostQuitTask(fastdelegate::Task<void>* task);
    template<typename T>
    void DeleteSoon(T *object) {
      TASK *task = build TASK;
      PostTask(MAKETASK_FUN1(task, DeleteHelper<T>::DoDelete, object));
    }
    void Run();
    void Quit();
    Type type() const { return type_;}
    // Optional call to connect the thread name with this loop.
    void set_thread_name(const std::string& thread_name) {
      if(thread_name_.empty())
        thread_name_ = thread_name;
    }
    void reset_thread_name(const std::string& thread_name) {
      if(!thread_name_.empty())
        thread_name_ = thread_name;
    }
    const std::string & thread_name() const {return thread_name_;}

    // Can only be called from the thread that owns the MessageLoop.
    bool is_running() const;

    //static
    static MessagePump* CreateMessagePumpForType(Type type);

    MessagePumpWin *pump_win();

    MessagePumpForIO *pump_io();
  private:
    void ExeQuitQueue();
    void Init();
    void AddToDelayedWorkQueue(const PendingTask& pending_task);
    TimeTicks CalculateDelayedRuntime(TimeDelta delay);
    void QuitInternal();
    void DeletePendingTasks();
    void ReloadWorkQueue();
    bool RunPendingTask(PendingTask &pending_task);
    void AppendTask(PendingTask &task);
    // MessagePump::Delegate methods:
    virtual bool DoWork() OVERRIDE;
    virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) OVERRIDE;
    virtual bool DoIdleWork() OVERRIDE;

    bool running_;
    std::string thread_name_;
    Type type_;
    std::auto_ptr<MessagePump> pump_;
    ObserverList<DestructionObserver> destruction_observers_;

    TaskQueue incoming_queue_;
    TaskQueue working_queue_;
    //退出要执行的一些任务, MessageLoop已经在析构了，所以确保对象存在
    TaskQueue quitwork_queue_;
    DelayedTaskQueue delayed_work_queue_;
    CriticalSection m_cs;
    TimeTicks recent_time_;

  public:
    //
    class LayerLoop :public MessagePump::Delegate {
      public:
        explicit LayerLoop(Type type = TYPE_DEFAULT);
        void Run();
        virtual bool DoWork() OVERRIDE;
        virtual bool DoDelayedWork(TimeTicks* next_delayed_work_time) OVERRIDE;
        virtual bool DoIdleWork() OVERRIDE;
        MessagePumpWin *pump_win();
        std::auto_ptr<MessagePump> pump_;
        static void PushBackLoop(LayerLoop* loop);
        static void PopBackLoop();
        static LayerLoop* Back();
    private:
        static std::vector<LayerLoop*> layer_;
    };
  };

}// end base