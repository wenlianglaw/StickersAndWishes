#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "settings.h"
#include "stickers_and_wishes.h"
#include "utils.h"

StickersAndWishes::StickersAndWishes() { pui_ = std::make_unique<Ui>(this); }

void StickersAndWishes::Run() {
  if (!LoadFromDisk())
    return;
    
  pui_->DisplayUi(0);
  while (is_running_) {
    auto cmd = pui_->GetCommand();
    pui_->HandleCommand(cmd);
  }
}

void StickersAndWishes::SaveToDisk() {
  std::ofstream os(settings::data_path, std::ofstream::trunc);
  os << total_stickers_ << std::endl;

  for (const auto &wish : wishes_) {
    auto t_c = std::chrono::system_clock::to_time_t(wish.time);
    os << std::put_time(std::localtime(&t_c), settings::time_fmt) << std::endl;
    os << wish.content.size() << " " << wish.content << std::endl;
    os << wish.notes.size() << " " << wish.notes << std::endl;
  }
  os.close();
}

bool StickersAndWishes::LoadFromDisk() {
  std::cerr << "Loading data.." << std::endl;
  // Reads data.
  std::ifstream is(settings::data_path);
  if (!is) {
    std::ofstream out(settings::data_path);
    out.close();
    is.open(settings::data_path);
  }

  is >> total_stickers_;
  is.ignore();
  std::string time_str;
  while (std::getline(is, time_str)) {
    WishRecord wish;
    // Loads time.
    std::tm tm = {};
    tm.tm_isdst = -1;
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, settings::time_fmt);
    auto time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    wish.time = time_point;

    // Loads content and notes.
    int cnt;
    is >> cnt;
    is.ignore();
    char ch;
    while (cnt--) {
      ch = is.get();
      wish.content.push_back(ch);
    }
    is >> cnt;
    is.ignore();
    while (cnt--) {
      ch = is.get();
      wish.notes.push_back(ch);
    }
    is.ignore();
    wishes_.push_back(wish);
  }
  return true;
}

void StickersAndWishes::ModifyWish(int no, const std::string &content,
                                   const std::string &notes) {
  if (no < 0 || no > wishes_.size()) {
    pui_->Info(FormatString("ERROR: wish no. ", no, " out of range.\n"));
    return;
  }

  WishRecord new_wish(content, notes);
  wishes_[no] = new_wish;
  SaveToDisk();
  
  Log(FormatString("Modified wish: \n", new_wish.ToString(), "\n"), false,
      settings::log_path);
}

void StickersAndWishes::AddWish(const std::string &content,
                                const std::string &notes) {
  WishRecord new_wish(content, notes);
  wishes_.push_back(new_wish);
  SaveToDisk();

  Log(FormatString("Add wish: \n", new_wish.ToString(), "\n"), false,
      settings::log_path);
}

void StickersAndWishes::RemoveAWish(int no) {
  if (no < 0 || no > wishes_.size()) {
    pui_->Info(FormatString("ERROR: wish no. ", no, " out of range.", "\n"));
    return;
  }

  Log(FormatString("Removed wish: \n", wishes_[no].ToString(), "\n"), true,
      settings::log_path);
  wishes_.erase(wishes_.begin() + no);
  SaveToDisk();
}

void StickersAndWishes::AddStickers(int cnt, const std::string &notes) {
  total_stickers_ += cnt;
  SaveToDisk();

  StickerRecord change;
  change.delta = cnt;
  change.notes = notes;
  

  Log(FormatString("Added stickers: \n", change.ToString(), "\n"), false,
      settings::log_path);
}

int StickersAndWishes::GetStickers() const { return total_stickers_; }

std::vector<WishRecord> StickersAndWishes::GetWishes() const { return wishes_; }

void StickersAndWishes::Exit() {
  Print("Saving changes and exiting.\n");
  SaveToDisk();
  is_running_ = false;
}
