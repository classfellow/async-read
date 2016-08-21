#pragma once
#include "stringprintf.h"
#include "macros.h"

namespace base {
  std::string StringPrintf(const char* format, ...);
  std::wstring StringPrintf(const wchar_t* format, ...);
  void StringAppendV(std::string* dst, const char* format, va_list ap);
  void StringAppendV(std::wstring* dst,
    const wchar_t* format, va_list ap);
} //end base