#pragma once

constexpr const char INFO_COLOR[] = "\033[38;5;51m";
constexpr const char ERROR_COLOR[] = "\033[38;5;160m";
constexpr const char DEBUG_COLOR[] = "\033[38;5;35m";
constexpr const char END_COLOR[] = "\033[0m";
constexpr const char BOLD[] = "\033[1m";
inline void clear_screen() { printf("\033c"); }

constexpr const char FILENAME_DEF[] = "output0.root";
constexpr int N_THRESHOLDS_DEF = 0;
constexpr double MIN_THRESHOLD_DEF = 0.0;
constexpr double MAX_THRESHOLD_DEF = 80.0;
constexpr bool VERBOSITY_DEF = false;
