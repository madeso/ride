#include "ride/loadresult.h"

LoadResult LoadResult::Ok() {
  LoadResult ret;
  ret.ok_ = true;
  return ret;
}

LoadResult LoadResult::Error(const wxString& message) {
  LoadResult ret;
  ret.message_ = message;
  ret.ok_ = false;
  return ret;
}

const wxString& LoadResult::message() const { return message_; }

bool LoadResult::IsOk() const { return ok_; }

LoadResult::LoadResult() {}
