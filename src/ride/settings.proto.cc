#include "ride/settings.proto.h"

namespace ride
{


const ride::FontsAndColors* Settings::find_current_theme() const
{
	for (const auto& theme: themes)
	{
		if (theme.name == current_theme)
		{
			return &theme.data;
		}
	}
	return nullptr;
}


}

