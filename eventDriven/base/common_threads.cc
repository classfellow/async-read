/* 
* @author   classfellow@qq.com
* @bf  
**           
****************************************************************************************/

#include "StdAfx.h"
#include "MessageLoop.h"
#include "common_threads.h"

namespace base {
  MessageLoop* Threads::threads_[Threads::COUNT] = {NULL};

  MessageLoop *Threads::Get(ThreadType type) {
    return Threads::threads_[type];
  }

  void Threads::Set(ThreadType type, MessageLoop *loop) {
    assert(type < Threads::COUNT && !Threads::threads_[type]);
    Threads::threads_[type] = loop;
  }
}// end base