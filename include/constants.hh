#pragma once

// formatting
#include <cstdio>
constexpr const char ERROR_COLOR[] = "\033[38;5;160m";
constexpr const char WARNING_COLOR[] = "\033[38;5;160m";
constexpr const char INFO_COLOR[] = "\033[38;5;51m";
constexpr const char DEBUG_COLOR[] = "\033[38;5;35m";
constexpr const char END_COLOR[] = "\033[0m";
constexpr const char BOLD[] = "\033[1m";
inline void clear_screen() { printf("\033c"); }
inline void print_debug(const char *message) { printf("%s%s%s\n", DEBUG_COLOR, message, END_COLOR); }
inline void print_info(const char *message) { printf("%s%s%s\n", INFO_COLOR, message, END_COLOR); }
inline void print_warning(const char *message) { printf("%s%s%s\n", WARNING_COLOR, message, END_COLOR); }
inline void print_error(const char *message) { printf("%s%s%s\n", ERROR_COLOR, message, END_COLOR); }

// # of pixels considered (including 0)
constexpr int MAX_PSF_ELEMENTS = 2;
