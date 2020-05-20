#include <iomanip>
#include <iostream>

#include "rlutil.h"
#include "stickers_and_wishes.h"
#include "ui.h"
#include "utils.h"

namespace {
void TypeAWish(std::string *content, std::string *notes) {
  Print("Input Wish: ");
  std::getline(std::cin, *content);

  Print("Input Notes: ");
  std::getline(std::cin, *notes);
}
} // namespace

// UI
Ui::Ui(StickersAndWishes *core) {
  pimpl_ = std::make_unique<UiImpl>();
  core_ = core;
}

void Ui::DisplayUi(int ui_id) { pimpl_->DisplayUi(*this, ui_id); }

void Ui::Info(const std::string &info) { pimpl_->Info(*this, info); }

std::string Ui::GetCommand() { return pimpl_->GetCommand(); }

void Ui::HandleCommand(const std::string &cmd) {
  pimpl_->HandleCommand(*this, cmd);
}

// UiImpl
std::string Ui::UiImpl::GetCommand() const {
  std::string cmd;
  int ch = rlutil::getkey();
  cmd.push_back((char)ch);
  return cmd;
}

void Ui::UiImpl::RefreshUi(const Ui &ui) { DisplayUi(ui, current_ui_id_); }

void Ui::UiImpl::DisplayCommand() { Print("Command: "); }

void Ui::UiImpl::HandleCommand(const Ui &ui, const std::string &cmd) {
  // See doc.md
  switch (current_ui_id_) {
  case 0:
    HandleCommandInitialUi(ui, cmd);
    break;
  default:
    break;
  }
}

void Ui::UiImpl::DisplayInitialMenu(const Ui &ui) {
  Print("0: Refresh wishes.\n");
  Print("1: Lookup wishes.\n");
  Print("2: Use a wish.\n");
  Print("3: Convert stickers to wish.\n");
  Print("4: Add a new wish.\n");
  Print("5: Modify a wish.\n");
  Print("+: Add stickers.\n");
  Print("-: Remove stickers.\n");
  Print("q: Exit.\n");
  Print("\n\n");
}

void Ui::UiImpl::HandleCommandInitialUi(const Ui &ui, const std::string &cmd) {
  // Lookup wishes
  if (cmd == "0") {
    RefreshUi(ui);
  } else if (cmd == "1") {
    DisplaySearchWishesUi(ui);
  } else if (cmd == "2") {
    UseAWish(ui);
  } else if (cmd == "3") {
    ConvertStickersToWish(ui);
    RefreshUi(ui);
  } else if (cmd == "4") {
    AddAWish(ui);
    RefreshUi(ui);
  } else if (cmd == "5") {
    ModifyWish(ui);
  } else if (cmd == "q" || cmd == "Q") {
    ExitProgram(ui);
  } else if (cmd == "+") {
    AddStickers(ui);
  } else if (cmd == "-") {
    RemoveStickers(ui);
  }
}

void Ui::UiImpl::ExitProgram(const Ui &ui) {
  Print("\n");
  if (PromptYesNo("Quit?")) {
    ui.core_->Exit();
  } else {
    RefreshUi(ui);
  }
}

void Ui::UiImpl::SearchWishes(const Ui &ui, const std::string &search) {
  // Naive search.
  std::vector<WishRecord> search_rst;
  std::vector<int> no_override;
  for (int i = 0; i < wishes_.size(); i++) {
    if (wishes_[i].content.find(search) != wishes_[i].content.npos) {
      search_rst.push_back(wishes_[i]);
      no_override.push_back(i);
    }
  }

  // Display results.
  rlutil::cls();
  DisplayInitialUi(ui, search_rst, true, no_override);
  DisplayMenu(ui, current_ui_id_);
  DisplayCommand();
}

void Ui::UiImpl::ModifyWish(const Ui &ui) {
  GetWishesAndStickers(ui);
  Print("\nWish No. you want to modify:");
  std::string str_no;
  std::cin >> str_no;
  if (!IsPostiveNum(str_no)) {
    Info(ui, "Invalid input.  Expect a positive number.");
    DisplayCommand();
    return;
  }
  int no = std::stoi(str_no);
  if (no >= wishes_.size()) {
    Info(ui, "Invalid input.  Beyond range.");
    DisplayCommand();
    return;
  }

  // Refreshes the window
  DisplayInitialUi(ui, {wishes_[no]}, true, {no});
  Print("Modifing wish no.", no);

  // Inputs a new wish.
  Print("\n");
  std::string content, notes;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  TypeAWish(&content, &notes);
  if (PromptYesNo("Modify wish?")) {
    ui.core_->ModifyWish(no, content, notes);
  }

  // Refreshes the window
  RefreshUi(ui);
}

void Ui::UiImpl::AddAWish(const Ui &ui) {
  Print("\n");
  std::string content, notes;
  TypeAWish(&content, &notes);

  if (!PromptYesNo("Add this wish?"))
    return;
  ui.core_->AddWish(content, notes);
  Info(ui, "Wish has been added.");
}

void Ui::UiImpl::ConvertStickersToWish(const Ui &ui) {
  Info(ui, FormatString("Rate: ", settings::sticker_wish_rate));
  if (total_stickers_ < settings::sticker_wish_rate) {
    Info(ui, FormatString("Not enough stickers. Stickers: ", total_stickers_));
    DisplayCommand();
    return;
  }
  AddAWish(ui);
  ui.core_->AddStickers(-settings::sticker_wish_rate,
                        "Convert stickers to wish.");
}

void Ui::UiImpl::UseAWish(const Ui &ui) {
  Print("\n");
  Print("Which wish to use? Type q to go back.  Wish No.:");
  std::string wish_no;
  std::cin >> wish_no;
  if (wish_no == "q") {
    RefreshUi(ui);
    DisplayCommand();
    return;
  }

  if (!IsPostiveNum(wish_no)) {
    RefreshUi(ui);
    Info(ui, FormatString("Invalid input: ", wish_no));
    DisplayCommand();
    return;
  }

  int no = std::stoi(wish_no);
  if (no >= wishes_.size()) {
    RefreshUi(ui);
    Info(ui, FormatString("Wish No. out of boundary: ", wish_no));
    DisplayCommand();
    return;
  }

  DisplayWishesSection(ui, {wishes_[no]}, true, {no});
  if (PromptYesNo("Use this wish?")) {
    ui.core_->RemoveAWish(no);
  }
  RefreshUi(ui);
  Info(ui, FormatString("Wish[", no, "] has been used."));
  DisplayCommand();
}

void Ui::UiImpl::GetWishesAndStickers(const Ui &ui) {
  wishes_ = ui.core_->GetWishes();
  total_stickers_ = ui.core_->GetStickers();
}

void Ui::UiImpl::DisplayWishesSection(const Ui &ui,
                                      const std::vector<WishRecord> &w,
                                      bool details,
                                      const std::vector<int> no_override) {
  auto no = no_override;
  if (no_override.empty()) {
    for (int i = 0; i < w.size(); i++) {
      no.push_back(i);
    }
  }
  Print("--------------------Wishes----------------------\n");
  int i = 0;
  for (; i < w.size(); i++) {
    std::cout << "No." << std::left << std::setw(settings::wish_no_width)
              << no[i];
    if (details) {
      Print(w[i].ToString());
    } else {
      Print(w[i].GetSummary());
    }
    Print("\n");
  }
  Print("------------------------------------------------\n");
}

void Ui::UiImpl::DisplayUi(const Ui &ui, int ui_id) {
  GetWishesAndStickers(ui);
  rlutil::cls();
  switch (ui_id) {
  case 0:
    DisplayInitialUi(ui, wishes_, true);
    break;
  default:
    break;
  }
  DisplayMenu(ui, ui_id);
  DisplayCommand();
}

void Ui::UiImpl::DisplayMenu(const Ui &ui, int ui_id) {
  switch (ui_id) {
  case 0:
    DisplayInitialMenu(ui);
    break;
  default:
    break;
  }
}

void Ui::UiImpl::DisplayStickersSection(const Ui &ui) {
  Print("Stickers: ", total_stickers_, "\n");
  Print("\n");
}

bool Ui::UiImpl::InputStickerChange(const Ui &ui, int *cnt,
                                    std::string *notes) {
  Print("\nHow many stickers do you want to add?\n");
  std::string input;
  std::cin >> input;
  std::cin.ignore();
  if (!IsPostiveNum(input)) {
    RefreshUi(ui);
    Info(ui, "ERROR: must be a positive number.");
    return false;
  }
  *cnt = std::stoi(input);
  Print("Notes: ");
  std::getline(std::cin, *notes);
  return true;
}

void Ui::UiImpl::AddStickers(const Ui &ui) {
  int cnt;
  std::string notes;
  InputStickerChange(ui, &cnt, &notes);
  if (PromptYesNo(FormatString("Add ", cnt, " stickers."))) {
    ui.core_->AddStickers(cnt, notes);
  }
  RefreshUi(ui);
}

void Ui::UiImpl::RemoveStickers(const Ui &ui) {
  int cnt;
  std::string notes;
  InputStickerChange(ui, &cnt, &notes);
  if (PromptYesNo(FormatString("Remove ", cnt, " stickers."))) {
    ui.core_->AddStickers(-cnt, notes);
  }
  RefreshUi(ui);
}

void Ui::UiImpl::DisplayInitialUi(const Ui &ui,
                                  const std::vector<WishRecord> &w,
                                  bool details,
                                  const std::vector<int> no_override) {
  current_ui_id_ = 0;
  // Displays the stickers.
  DisplayStickersSection(ui);

  // Display wishes.
  DisplayWishesSection(ui, w, true, no_override);
}

void Ui::UiImpl::DisplaySearchWishesUi(const Ui &ui) {
  rlutil::cls();
  DisplayInitialUi(ui, wishes_, true);
  current_ui_id_ = 5;
  Print("Search wishes: ");
  std::string search;
  std::getline(std::cin, search);
  SearchWishes(ui, search);
}

void Ui::UiImpl::Info(const Ui &ui, const std::string &info) {
  Print(":", info, "\n");
}
