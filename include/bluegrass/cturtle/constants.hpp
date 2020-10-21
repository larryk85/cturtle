#pragma once

namespace bluegrass::cturtle {

#ifdef NDEBUG
   constexpr inline static bool is_debug_build = false;
#else
   constexpr inline static bool is_debug_build = true;
#endif

} // ns bluegrass::cturtle
