#include "itemlist.h"

#include <wx/settings.h>
#include <wx/dcbuffer.h>

#include "style.h"

namespace switcher
{

ItemList::ItemList()
	: column_count_(0)
{
}

Group& ItemList::AddGroup(const Group& item, int* index)
{
	items_.push_back(item);
	if (index != nullptr)
	{
		*index = items_.size() - 1;
	}
	return *items_.rbegin();
}

Index ItemList::GetIndexForFocus() const
{
	for (size_t i = 0; i < items_.size(); i++)
	{
		int item = items_[i].GetIndexForFocus();
		if (item != -1)
		{
			return Index(i, item);
		}
	}

	return SWITCHER_NOT_FOUND;
}

Index ItemList::HitTest(const wxPoint& pt) const
{
	for (size_t i = 0; i < items_.size(); i++)
	{
		int item = items_[i].HitTest(pt);
		if (item != -1)
		{
			return std::make_pair(i, item);
		}
	}

	return SWITCHER_NOT_FOUND;
}

#define GET_ITEM() \
	do \
	{ \
		return GetGroup(i.first).GetItem(i.second); \
	} while (false)

const Item& ItemList::GetItem(Index i) const
{
	GET_ITEM();
}

Item& ItemList::GetItem(Index i)
{
	GET_ITEM();
}

#undef GET_ITEM

int ItemList::GetItemCount() const
{
	int count = 0;
	for (const Group& group: items_)
	{
		count += group.GetItemCount();
	}
	return count;
}

#define GET_GROUP() \
	do \
	{ \
		assert(i >= 0); \
		assert(i < items_.size()); \
		return items_[i]; \
	} while (false)

const Group& ItemList::GetGroup(int i) const
{
	GET_GROUP();
}

Group& ItemList::GetGroup(int i)
{
	GET_GROUP();
}

#undef GET_GROUP

int ItemList::GetGroupCount() const
{
	return items_.size();
}

void ItemList::set_column_count(int cols)
{
	column_count_ = cols;
}

int ItemList::column_count() const
{
	return column_count_;
}

void ItemList::PaintItems(wxDC* dc, const Style& style, Index selection, wxWindow* win)
{
	dc->SetLogicalFunction(wxCOPY);
	dc->SetBrush(wxBrush(style.background_color()));
	dc->SetPen(*wxTRANSPARENT_PEN);
	dc->DrawRectangle(win->GetClientRect());
	dc->SetBackgroundMode(wxTRANSPARENT);

	for (size_t i = 0; i < items_.size(); i++)
	{
		items_[i].PaintItems(dc, style, i == selection.first ? selection.second : -1);
	}
}

wxSize ItemList::CalculateItemSize(wxDC* dc, const Style& style)
{
	// Start off allowing for an icon
	wxSize sz(150, 16);

	for (Group& item: items_)
	{
		item.CalculateItemSize(dc, style, &sz);
	}

	if (sz == wxSize(16, 16))
	{
		sz = wxSize(100, 25);
	}
	else
	{
		sz.x += style.col_spacing() * 2;
		sz.y += style.row_spacing() * 2;
	}

	return sz;
}

}  // namespace switcher
