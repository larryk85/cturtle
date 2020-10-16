#pragma once

#include "term_color.hpp"

#define DEFINE_LOGGER(NAME, PRED, COLOR, BASE_LOGGER) \
   template <typename... Ts>             \
   inline void NAME(detail::string_view_wrapper f, Ts&&... ts) { if constexpr (PRED) detail::log(BASE_LOGGER, detail::format<COLOR>(f.value, #NAME, f.info), std::forward<Ts>(ts)...); } \
   template <typename... Ts>             \
   inline void NAME(call_info ci, std::string_view f, Ts&&... ts) { if constexpr (PRED) detail::log(BASE_LOGGER, detail::format<COLOR>(f, #NAME, ci), std::forward<Ts>(ts)...); }

namespace bluegrass::feller {
   namespace detail {
      struct string_view_wrapper {
         constexpr inline string_view_wrapper(std::string_view t, call_info ci={})
            : value(t), info(ci) {}
         template <std::size_t N>
         constexpr inline string_view_wrapper(const char (&s)[N], call_info ci={})
           : value(s), info(ci) {}
         constexpr inline operator std::string_view() const { return value; }

         std::string_view value;
         call_info info;
      };
   } // ns bluegrass::feller::detail

   template <typename Stream>
   struct logger {
      inline logger(Stream& s)
         : stream(&s) {}

      template <typename... Ts>
      void write(std::string_view format, Ts&&... ts) {
         (*stream) << fmt::format(format, std::forward<Ts>(ts)...);
         flush();
      }

      void flush() { (*stream).flush(); }

      Stream* stream;
   };

   inline static logger cout_logger = {std::cout};
   inline static logger clog_logger = {std::clog};
   inline static logger cerr_logger = {std::cerr};

   namespace detail {
      template <typename Logger, typename... Ts>
      inline void log(Logger& l, std::string_view f, Ts&&... ts) {
         l.write(f, std::forward<Ts>(ts)...);
      }
      template <uint8_t FG>
      inline auto format(std::string_view f, std::string_view nm, call_info ci) {
         constexpr std::string_view form = "{0}[{1}]{2} :: {3}{4}:{5}:{6}{7} :: {8}\n";
         return fmt::format(form, color::set(FG), nm, color::reset(),
               color::set(color::bright_cyan), ci.file_name(), ci.func_name(), ci.line_num(), color::reset(), f);
      }
   } // ns bluegrass::feller::detail

   DEFINE_LOGGER(debug_log, is_debug_build, 94, cout_logger);
   DEFINE_LOGGER(info_log,  true,           92, cout_logger);
   DEFINE_LOGGER(warn_log,  true,           93, clog_logger);
   DEFINE_LOGGER(error_log, true,           91, cerr_logger);
} // ns bluegrass::feller

#undef DEFINE_LOGGER
