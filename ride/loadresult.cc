#include "ride/loadresult.h"

LoadResult LoadResult::Ok()
{
  LoadResult ret;
  return ret;
}

LoadResult LoadResult::Error(const wxString& message) {
  LoadResult ret;
  ret.message_ = message;
  return ret;
}

const wxString& LoadResult::message() const {
  return message_;
}

bool LoadResult::IsOk() const {
  return false == message_.IsEmpty();
}

LoadResult::LoadResult() { }
