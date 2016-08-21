/* 
* @author   classfellow@qq.com
* @warning  
**           
****************************************************************************************/
#include "StdAfx.h"
#include "at_exist.h"

namespace base
{
  static AtExitManager* g_top_manager = NULL;

  AtExitManager::AtExitManager(){
    g_top_manager = this;
  }

  AtExitManager::~AtExitManager(){
    assert(this == g_top_manager);

    ProcessCallbacksNow();

    g_top_manager = NULL;
  }

  // static
  void AtExitManager::RegisterCallback(AtExitCallbackType func, void* param) {
    assert(func);
    RegisterTask(base::Bind(func, param));
  }

  // static
  void AtExitManager::RegisterTask(fastdelegate::Task<void>* task) {
    if (!g_top_manager) {
      assert(0); // "Tried to RegisterCallback without an AtExitManager";
      return;
    }

    AutoCritSecLock<CriticalSection> lock(g_top_manager->m_cs, false);
    lock.Lock();
    g_top_manager->stack_.push(task);
  }

  // static
  void AtExitManager::ProcessCallbacksNow() {
    if (!g_top_manager) {
      assert(0); //"Tried to ProcessCallbacksNow without an AtExitManager";
      return;
    }

    AutoCritSecLock<CriticalSection> lock(g_top_manager->m_cs, false);
    lock.Lock();

    while (!g_top_manager->stack_.empty()) {
      std::auto_ptr<fastdelegate::Task<void>> task(g_top_manager->stack_.top());
      task->Run();
      g_top_manager->stack_.pop();
    }
  }
}