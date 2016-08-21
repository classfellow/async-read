#pragma once

#include "macros.h"
#include "MessagePump.h"
#include "../Event/WaitableEvent.h"

namespace base {
  class MessagePumpDefault : public MessagePump {
  public:
    MessagePumpDefault();
    virtual ~MessagePumpDefault();

    // MessagePump methods:
    virtual void Run(Delegate* delegate) OVERRIDE;
    virtual void Quit() OVERRIDE;
    virtual void ScheduleWork() OVERRIDE;
    virtual void ScheduleDelayedWork(const TimeTicks& delayed_work_time) OVERRIDE;

  private:
    // This flag is set to false when Run should return.
    bool keep_running_;

    // Used to sleep until there is more work to do.
    WaitableEvent event_;

    // The time at which we should call DoDelayedWork.
    TimeTicks delayed_work_time_;

    DISALLOW_COPY_AND_ASSIGN(MessagePumpDefault);
  };  
} //end base

