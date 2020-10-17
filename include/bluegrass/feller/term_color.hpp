#pragma once

#include "utils.hpp"

#include <fmt/format.h>

#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string_view>
#include <sstream>

namespace bluegrass::feller::color {

   namespace style {
      constexpr static inline uint8_t reset     = 0;
      constexpr static inline uint8_t bold      = 1;
      constexpr static inline uint8_t dim       = 2;
      constexpr static inline uint8_t italic    = 3;
      constexpr static inline uint8_t underline = 4;
      constexpr static inline uint8_t blink     = 5;
      constexpr static inline uint8_t rblink    = 6;
      constexpr static inline uint8_t reversed  = 7;
      constexpr static inline uint8_t conceal   = 8;
      constexpr static inline uint8_t crossed   = 9;
   }

   constexpr static inline uint8_t black             = 30;
   constexpr static inline uint8_t red               = 31;
   constexpr static inline uint8_t green             = 32;
   constexpr static inline uint8_t yellow            = 33;
   constexpr static inline uint8_t blue              = 34;
   constexpr static inline uint8_t magenta           = 35;
   constexpr static inline uint8_t cyan              = 36;
   constexpr static inline uint8_t gray              = 37;
   constexpr static inline uint8_t bright_black      = 90;
   constexpr static inline uint8_t bright_red        = 91;
   constexpr static inline uint8_t bright_green      = 92;
   constexpr static inline uint8_t bright_yellow     = 93;
   constexpr static inline uint8_t bright_blue       = 94;
   constexpr static inline uint8_t bright_magenta    = 95;
   constexpr static inline uint8_t bright_cyan       = 96;
   constexpr static inline uint8_t bright_gray       = 97;
   constexpr static inline uint8_t bg_black          = 40;
   constexpr static inline uint8_t bg_red            = 41;
   constexpr static inline uint8_t bg_green          = 42;
   constexpr static inline uint8_t bg_yellow         = 43;
   constexpr static inline uint8_t bg_blue           = 44;
   constexpr static inline uint8_t bg_magenta        = 45;
   constexpr static inline uint8_t bg_cyan           = 46;
   constexpr static inline uint8_t bg_gray           = 47;
   constexpr static inline uint8_t bg_bright_black   = 100;
   constexpr static inline uint8_t bg_bright_red     = 101;
   constexpr static inline uint8_t bg_bright_green   = 102;
   constexpr static inline uint8_t bg_bright_yellow  = 103;
   constexpr static inline uint8_t bg_bright_blue    = 104;
   constexpr static inline uint8_t bg_bright_magenta = 105;
   constexpr static inline uint8_t bg_bright_cyan    = 106;
   constexpr static inline uint8_t bg_bright_gray    = 107;

   static inline auto is_in_terminal() noexcept {
      static bool in_terminal = isatty(fileno(stdout)) != 0;
      return in_terminal;
   }

   static inline std::string set(uint8_t fc, uint8_t bc, uint8_t s) noexcept {
      if (LIKELY(is_in_terminal()))
         return fmt::format("\033[{};{};{}m", s, fc, bc);
      else
         return "";
   }

   static inline std::string set(uint8_t fc, uint8_t bc) noexcept {
      if (LIKELY(is_in_terminal()))
         return fmt::format("\033[{};{}m", fc, bc);
      else
         return "";
   }

   static inline std::string set(uint8_t c) noexcept {
      if (LIKELY(is_in_terminal()))
         return fmt::format("\033[{}m", c);
      else
         return "";
   }

   static inline std::string reset() noexcept {
      if (LIKELY(is_in_terminal()))
         return "\033[0m";
      else
         return "";
   }

   static inline bool is_supported() noexcept {
      constexpr std::array<const char*, 14> terms =
            { "ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm", "linux", "msys", "putty",
              "rxvt", "screen", "vt100", "xterm" };

      const char* term = std::getenv("TERM");
      return term && std::any_of(terms.begin(), terms.end(), [&](const char* t) { return std::strstr(term, t) != nullptr; });
   }

   struct redirect {
      redirect(bool cout, bool clog, bool cerr) {
         if (cout)
            cout_old = std::cout.rdbuf(cout_buffer.rdbuf());
         if (clog)
            clog_old = std::clog.rdbuf(clog_buffer.rdbuf());
         if (cerr)
            cerr_old = std::cerr.rdbuf(cerr_buffer.rdbuf());
      }

      ~redirect() {
         if (cout_old)
            std::cout.rdbuf(cout_old);
         if (clog_old)
            std::clog.rdbuf(clog_old);
         if (cerr_old)
            std::cerr.rdbuf(cerr_old);
      }

      std::string get_stdout() {
         std::string s = cout_buffer.str();
         cout_buffer.str("");
         return s;
      }

      std::string get_stdlog() {
         std::string s = clog_buffer.str();
         clog_buffer.str("");
         return s;
      }

      std::string get_stderr() {
         std::string s = cerr_buffer.str();
         cerr_buffer.str("");
         return s;
      }

      std::stringstream cout_buffer;
      std::stringstream clog_buffer;
      std::stringstream cerr_buffer;
      std::streambuf* cout_old = nullptr;
      std::streambuf* clog_old = nullptr;
      std::streambuf* cerr_old = nullptr;
   };
} // ns bluegrass::feller::color
