/* 
* @author   classfellow@qq.com
* @bf  ∫Í∂®“Â
**           
****************************************************************************************/

#pragma once

#define OVERRIDE override

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName();                                    \
  DISALLOW_COPY_AND_ASSIGN(TypeName)

#define TASK  fastdelegate::Task<void>
#define build     new

//using namespace fastdelegate;


#define MAKETASK_FUN0(task, fun) \
  (task->SetFun(fastdelegate::MakeFunctionEntity(fun)) , task)

#define MAKETASK_FUN1(task, fun, p1) \
  (task->SetFun(fastdelegate::MakeFunctionEntity(fun, (p1))) , task)

#define MAKETASK_FUN2(task, fun, p1, p2) \
  (task->SetFun(fastdelegate::MakeFunctionEntity(fun, (p1), (p2))) , task)

#define MAKETASK_FUN3(task, fun, p1, p2, p3) \
  (task->SetFun(fastdelegate::MakeFunctionEntity(fun, (p1), (p2), (p3))) , task)

#define MAKETASK_FUN4(task, fun, p1, p2, p3, p4) \
  (task->SetFun(fastdelegate::MakeFunctionEntity(fun, (p1), (p2), (p3), (p4))) , task)

#define MAKETASK_CLOSURE0(task, obj, fun) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun)), task)

#define MAKETASK_CLOSURE1(task, obj, fun, p1) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun, (p1))), task)

#define MAKETASK_CLOSURE2(task, obj, fun, p1, p2) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun, (p1), (p2))), task)

#define MAKETASK_CLOSURE3(task, obj, fun, p1, p2, p3) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun, (p1), (p2), (p3))), task)

#define MAKETASK_CLOSURE4(task, obj, fun, p1, p2, p3, p4) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun, (p1), (p2), (p3), (p4))), task)

#define MAKETASK_CLOSURE5(task, obj, fun, p1, p2, p3, p4, p5) \
  (task->SetClass(fastdelegate::MakeDelegateEntity(&(obj), &fun, (p1), (p2), (p3), (p4), (p5))), task)

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// It's possible for functions that use a va_list, such as StringPrintf, to
// invalidate the data in it upon use.  The fix is to make a copy of the
// structure before using it and use that copy instead.  va_copy is provided
// for this purpose.  MSVC does not provide va_copy, so define an
// implementation here.  It is not guaranteed that assignment is a copy, so the
// StringUtil.VariableArgsFunc unit test tests this capability.
#if defined(COMPILER_GCC)
#define GG_VA_COPY(a, b) (va_copy(a, b))
#elif defined(COMPILER_MSVC)
#define GG_VA_COPY(a, b) (a = b)
#endif

#define DCHECK_GE(a, b) assert((a) >= (b));
#define DCHECK_LE(a, b) assert((a) <= (b));
#define DCHECK_NE(a, b)  assert((a) != (b));
#define DCHECK_EQ(a, b)         assert((a) == (b));
#define CHECK(x)        assert(x);

//For Debug
//#define FRAMEDEBUG_
//#define FORCE_PRINT_LOG_