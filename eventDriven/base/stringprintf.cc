#include "StdAfx.h"
#include "stringprintf.h"
#include "string_util_win.h"

namespace base{

  inline int vsnprintfT(char* buffer,
    size_t buf_size,
    const char* format,
    va_list argptr) {
      return base::vsnprintf(buffer, buf_size, format, argptr);
  }

  inline int vsnprintfT(wchar_t* buffer,
    size_t buf_size,
    const wchar_t* format,
    va_list argptr) {
      return base::vswprintf(buffer, buf_size, format, argptr);
  }

  template <class StringType>
  static void StringAppendVT(StringType* dst,
    const typename StringType::value_type* format,
    va_list ap) {
      // First try with a small fixed size buffer.
      // This buffer size should be kept in sync with StringUtilTest.GrowBoundary
      // and StringUtilTest.StringPrintfBounds.
      typename StringType::value_type stack_buf[1024];

      va_list ap_copy;
      GG_VA_COPY(ap_copy, ap);


      int result = vsnprintfT(stack_buf, arraysize(stack_buf), format, ap_copy);
      va_end(ap_copy);

      if (result >= 0 && result < static_cast<int>(arraysize(stack_buf))) {
        // It fit.
        dst->append(stack_buf, result);
        return;
      }

      // Repeatedly increase buffer size until it fits.
      int mem_length = arraysize(stack_buf);
      while (true) {
        if (result < 0) {
          {
            return;
          }
          // Try doubling the buffer size.
          mem_length *= 2;
        } else {
          // We need exactly "result + 1" characters.
          mem_length = result + 1;
        }

        if (mem_length > 32 * 1024 * 1024) {
          // That should be plenty, don't try anything larger.  This protects
          // against huge allocations when using vsnprintfT implementations that
          // return -1 for reasons other than overflow without setting errno.
          return;
        }

        std::vector<typename StringType::value_type> mem_buf(mem_length);

        // NOTE: You can only use a va_list once.  Since we're in a while loop, we
        // need to make a new copy each time so we don't use up the original.
        GG_VA_COPY(ap_copy, ap);
        result = vsnprintfT(&mem_buf[0], mem_length, format, ap_copy);
        va_end(ap_copy);

        if ((result >= 0) && (result < mem_length)) {
          // It fit.
          dst->append(&mem_buf[0], result);
          return;
        }
      }
  }

  std::string StringPrintf(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    std::string result;
    StringAppendV(&result, format, ap);
    va_end(ap);
    return result;
  }

  std::wstring StringPrintf(const wchar_t* format, ...) {
    va_list ap;
    va_start(ap, format);
    std::wstring result;
    StringAppendV(&result, format, ap);
    va_end(ap);
    return result;
  }

  void StringAppendV(std::string* dst, const char* format, va_list ap) {
    StringAppendVT(dst, format, ap);
  }

  void StringAppendV(std::wstring* dst, const wchar_t* format, va_list ap) {
    StringAppendVT(dst, format, ap);
  }


}//end base