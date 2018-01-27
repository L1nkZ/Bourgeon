#include "session.h"

ITEM_INFO Session::GetItemInfoById(int nameid) const {
  auto ilist = item_list();

  for (const auto iinfo : ilist) {
    if (atoi(iinfo.item_name_.c_str()) == nameid) return iinfo;
  }

  return ITEM_INFO();
}