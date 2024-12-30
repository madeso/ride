#include "ride/tab.h"

#include <ride/wx.h>
#include <cassert>

#include "ride/fileedit.h"
#include "ride/startpage.h"

Tab::Tab(FileEdit* edit)
	: edit_(edit)
	, start_(nullptr)
	, data_(nullptr)
{
	assert(edit);
}

Tab::Tab(StartPageTab* start)
	: edit_(nullptr)
	, start_(start)
	, data_(nullptr)
{
	assert(start);
}

Tab::Tab(TabData* data)
	: edit_(nullptr)
	, start_(nullptr)
	, data_(data)
{
	assert(data);
}

Tab::~Tab()
{
}

FileEdit* Tab::ToFileEdit()
{
	assert(this);
	return edit_;
}

StartPageTab* Tab::ToStartPage()
{
	assert(this);
	return start_;
}

TabData* Tab::ToData()
{
	assert(this);
	return data_;
}

wxWindow* Tab::ToControl()
{
	if (start_)
		return start_;
	else if (edit_)
		return edit_;
	else if (data_)
		return data_->ToControl();
	return nullptr;
}
