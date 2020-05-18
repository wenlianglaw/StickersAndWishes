#ifndef _STICKER_AND_WISHES_H_
#define _STICKER_AND_WISHES_H_

#include <fstream>
#include <iostream>
#include <vector>

#include "record_structure.h"
#include "ui.h"

class StickersAndWishes {
public:
  StickersAndWishes();
  ~StickersAndWishes() = default;

  // Runs the program.
  void Run();

  // Saves all data to disk.
  // Saves wishes, stikcers.
  // The first line: total stickers.
  // The date time.
  // The size of the wish content, and following with the content.
  // The size of the wish notes, and following with the notes.
  void SaveToDisk();

  // Loads the records from disk.
  // Loads wishes, stickers and histories.
  void LoadFromDisk();

  // Modify a wish.
  void ModifyWish(int no, const std::string &content, const std::string &notes);
  // Hack: Add a wish directly without changeing the sticker num.
  void AddWish(const std::string &content, const std::string &notes);
  void RemoveAWish(int no);

  void AddStickers(int cnt, const std::string &notes);

  int GetStickers() const;
  std::vector<WishRecord> GetWishes() const;

  void Exit();

private:
  std::unique_ptr<Ui> pui_;
  std::vector<WishRecord> wishes_;

  int total_stickers_ = 0;
  bool is_running_ = true;
};

#endif // _STICKER_AND_WISHES_H_
