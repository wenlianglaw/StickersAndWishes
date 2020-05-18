#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>

#include "utils.h"

void Log(const std::string &log, bool output_date,
         const std::string &file_name) {
  std::ofstream out(file_name, std::ofstream::app);
  if (output_date) {
    auto now = std::chrono::system_clock::now();
    auto t_c = std::chrono::system_clock::to_time_t(now);
    out << std::put_time(std::localtime(&t_c), settings::time_fmt) << std::endl;
  }
  out << log << std::endl;
}

bool IsPostiveNum(const std::string &str) {
  return std::all_of(str.begin(), str.end(),
                     [](char c) { return std::isdigit(c); });
}

bool PromptYesNo(std::string_view str) {
  Print(str, "  yes/No?\n");
  std::string ans;
  std::cin >> ans;
  // Ignore to the end of line
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return ans == "yes" || ans == "y";
}

void PrintTimePoint(const std::chrono::system_clock::time_point &t) {
    std::stringstream ss;
    auto t_c = std::chrono::system_clock::to_time_t(t);
    ss << std::put_time(std::localtime(&t_c), settings::time_fmt) << std::endl;
    Print(ss.str());
}
