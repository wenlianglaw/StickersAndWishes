#ifndef _UI_H_
#define _UI_H_

#include <stack>
#include <string>
#include <vector>

#include "record_structure.h"
#include "settings.h"

class StickersAndWishes;

class Ui {

public:
  Ui(StickersAndWishes *core);
  ~Ui() = default;

  void DisplayUi(int ui_id);
  void Info(const std::string &info);
  std::string GetCommand();
  void HandleCommand(const std::string &cmd);

private:
  // The UI Implementation.
  class UiImpl;
  std::unique_ptr<UiImpl> pimpl_;
  // The caller program.  UI does the data modification pull/request with.
  StickersAndWishes *core_;
};

class Ui::UiImpl {
public:
  UiImpl() = default;
  ~UiImpl() = default;

  void DisplayUi(const Ui &ui, int ui_id);
  void DisplayMenu(const Ui &ui, int ui_id);
  void Info(const Ui &ui, const std::string &info);
  void GetWishesAndStickers(const Ui &ui);
  std::string GetCommand() const;
  void HandleCommand(const Ui &ui, const std::string &cmd);

private:
  void DisplayWishesSection(const Ui &ui, const std::vector<WishRecord> &w,
                            bool details,
                            const std::vector<int> no_override = {});
  void DisplayStickersSection(const Ui &ui);

  void DisplayInitialUi(const Ui &ui, const std::vector<WishRecord> &w,
                        bool details = false,
                        const std::vector<int> no_override = {});
  void DisplayInitialMenu(const Ui &ui);

  // Wish UI, menus and processings.
  void UseAWish(const Ui &ui);
  void AddAWish(const Ui &ui);
  void ConvertStickersToWish(const Ui &ui);
  void ModifyWish(const Ui &ui);

  void DisplayViewStickersUi(const Ui &ui);
  void DisplayViewStickersMenu(const Ui &ui);
  bool InputStickerChange(const Ui &ui, int *cnt, std::string *notes);
  void AddStickers(const Ui &ui);
  void RemoveStickers(const Ui &ui);

  void DisplaySearchWishesUi(const Ui &ui);

  void SearchWishes(const Ui &ui, const std::string &search);

  void DisplayCommand();

  void RefreshUi(const Ui &ui);

  void ExitProgram(const Ui &ui);

  // Handle command functions.
  void HandleCommandInitialUi(const Ui &ui, const std::string &cmd);

  // Essiential data.
  std::vector<WishRecord> wishes_;
  int total_stickers_;

  // Which ui is currently being displayed.
  int current_ui_id_ = 0;
};
#endif // _UI_H_
