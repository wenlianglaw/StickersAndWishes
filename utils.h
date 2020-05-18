#ifndef _UTILS_H_
#define _UTILS_H_

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string_view>

#include "rlutil.h"
#include "settings.h"

void Log(const std::string &log, bool output_date,
         const std::string &file_name);

bool IsPostiveNum(const std::string &str);

// For debugging.
void PrintTimePoint(const std::chrono::system_clock::time_point &t);

bool PromptYesNo(std::string_view str);

template <typename... Ts> std::string FormatString(Ts... args) {
  [[maybe_unused]] std::stringstream ss;
  (ss << ... << args);
  return ss.str();
}

template <typename... Ts> void Print(Ts... args) {
  std::cout << FormatString(args...);
}

template <typename... Ts> void SetStringOnScreen(Ts... args) {
  rlutil::setString(FormatString(args...));
}

#endif // _UTILS_H_
