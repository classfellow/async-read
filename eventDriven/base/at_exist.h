/* 
* @author   classfellow@qq.com
* @bf       Ä£·Â crtµÄexit 
**           
****************************************************************************************/
#pragma once

#include "macros.h"
#include "../util/util_tools.h"
#include "FastDelegateImpl.h"
#include <stack>
namespace base{
  class AtExitManager{
  public:
    typedef void (*AtExitCallbackType)(void*);
    AtExitManager();

    // The dtor calls all the registered callbacks. Do not try to register more
    // callbacks after this point.
    ~AtExitManager();

    static void RegisterCallback(AtExitCallbackType func, void* param);
    // Registers the specified task to be called at exit.
    static void RegisterTask(fastdelegate::Task<void>* task);
    // Calls the functions registered with RegisterCallback in LIFO order. It
    // is possible to register new callbacks after calling this function.
    static void ProcessCallbacksNow();

  private:

    CriticalSection m_cs;
    std::stack<fastdelegate::Task<void>*> stack_;

    DISALLOW_COPY_AND_ASSIGN(AtExitManager);
  };
} //end base