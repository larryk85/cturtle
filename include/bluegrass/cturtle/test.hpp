#pragma once

#include "errors.hpp"
#include "log.hpp"

#include <stdexcept>

namespace bluegrass::cturtle {
   template <typename... Ts>
   inline void test(bool pred, error err, detail::string_wrapper err_msg, Ts&&... ts) {
      if (!UNLIKELY(pred)) {
         error_log(err_msg.info, err_msg.value, std::forward<Ts>(ts)...);
         throw runtime_error(err);
      }
   }

   inline void test(bool pred, error err, call_info ci={}) {
      test(pred, err, {"assertion failure", ci});
   }
} // ns bluegrass::cturtle
