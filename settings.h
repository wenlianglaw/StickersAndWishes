#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <string>
#include <unordered_map>

// Some layouts
namespace settings {
constexpr int wish_no_width = 10;
constexpr int wish_summary_width = 20;
constexpr int sticker_history_limit = 20;

const constexpr char *data_path = "./data";
const constexpr char *history_path = "./history";
const constexpr char *log_path = "./log";
constexpr int sticker_wish_rate = 10;

const char* const time_fmt = "%F %T";

const std::unordered_map<int, std::string> ui_id_name = {
    {0, "Initial UI"},
};

} // namespace settings

#endif // _SETTINGS_H_
