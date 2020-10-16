#pragma once

#include <string_view>

#if !defined(LIKELY) && !defined(UNLIKELY)
#   if defined(__GNUC__)
#      if (__GNUC__ > 5) || defined(__clang__)
#         define LIKELY(x) __builtin_expect(!!(x), 1)
#         define UNLIKELY(x) __builtin_expect(!!(x), 0)
#      else
#         define LIKELY(x) !!(x)
#         define UNLIKELY(x) !!(x)
#      endif
#   else
#      define LIKELY(x) !!(x)
#      define UNLIKELY(x) !!(x)
#   endif
#endif

namespace bluegrass::feller {
   namespace detail {
      constexpr static inline std::string_view only_file_name(std::string_view f) {
         for (std::size_t i = f.size(); i >= 0; i--)
            if ( f[i] == '\\' || f[i] == '/' )
               return f.substr(i+1, f.size() - i);
         return f;
      }
   } // ns bluegrass::utils::detail

   struct call_info {
      constexpr inline call_info(std::string_view file=__builtin_FILE(), std::string_view func=__builtin_FUNCTION(), int line=__builtin_LINE())
         : file_n(detail::only_file_name(file)), func_n(func), line_n(line) {}
      constexpr inline auto file_name() const { return file_n; }
      constexpr inline auto func_name() const { return func_n; }
      constexpr inline auto line_num()  const { return line_n; }
      std::string_view file_n;
      std::string_view func_n;
      int line_n;
   };

#ifdef NDEBUG
   constexpr inline static bool is_debug_build = false;
#else
   constexpr inline static bool is_debug_build = true;
#endif

} // ns bluegrass::feller
