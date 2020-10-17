#pragma once

#include "utils.hpp"

#include <fmt/compile.h>

#include <limits>
#include <stdexcept>

namespace bluegrass::feller {
   struct error {
      constexpr inline error(std::string_view msg, std::size_t ec)
         : error_msg(msg), error_code(ec) {}
      template <std::size_t N>
      constexpr inline error(const char (&msg)[N], std::size_t ec)
         : error_msg(msg, N-1), error_code(ec) {}
      constexpr inline std::string_view msg() const noexcept { return error_msg; }
      constexpr inline std::size_t code() const noexcept { return error_code; }
      std::string_view error_msg;
      std::size_t error_code;
   };

   class runtime_error : std::exception {
      public:
         inline runtime_error(error err)
            : err(err), what_msg(fmt::format("runtime_error<{0}::{1}>", err.msg(), err.code())) {}
         inline runtime_error& operator=(const runtime_error& o) {
            err = o.err;
            return *this;
         }
         virtual inline const char* what() const noexcept { return what_msg.c_str(); }
         inline std::size_t code() const noexcept { return err.code(); }
      private:
         error err;
         std::string what_msg;
   };

} // ns bluegrass::feller

#define BLUEGRASS_ERROR(NAME) constexpr static inline error NAME = {#NAME, __COUNTER__};
