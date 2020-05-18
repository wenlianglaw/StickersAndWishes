#pragma once
// This file defines the wish and stickers.
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "settings.h"
#include "util.h"

struct StickerRecord {
  std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
  int delta = 0;
  std::string notes;

  std::string ToString() const {
    std::stringstream ss;
    auto t_c = std::chrono::system_clock::to_time_t(time);
    ss << std::put_time(std::localtime(&t_c), settings::time_fmt) << std::endl;
    ss << "Delta: " << delta << std::endl;
    ss << notes;
    return ss.str();
  }
};

struct WishRecord {
  std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
  std::string content;
  std::string notes;

  WishRecord() = default;
  ~WishRecord() = default;
  WishRecord(const std::string &content, const std::string &notes) {
    this->content = content;
    this->notes = notes;
  }

  std::string ToString() const {
    std::stringstream ss;
    auto t_c = std::chrono::system_clock::to_time_t(time);
    ss << std::put_time(std::localtime(&t_c), settings::time_fmt) << std::endl;
    ss << "Wish: " << content << std::endl;
    ss << "Notes: " << notes << std::endl;
    return ss.str();
  }

  std::string GetSummary() const {
    std::string ret = content.substr(0, settings::wish_summary_width);
    ret.append("...\n");
    return ret;
  }
};

struct HistoryRecord {
  StickerRecord sticker;
  WishRecord wish;
  enum Type { STICKER = 0, WISH };
  Type type;
  std::chrono::system_clock::time_point time = std::chrono::system_clock::now();
};
