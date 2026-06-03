#include "ride/settings.proto.h"

#include <unordered_set>

#include "settings.h"

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

wxString Settings::LookupSetting(const wxString& name) const
{
	// todo(Gustav): implement recursive aliases

	if (name == "auto.fold")
	{
		return foldEnable ? "1" : "0";
	}

	return name;
}

wxString string_from_chain(const std::vector<wxString>& chain)
{
	wxString result;
	for (size_t i = 0; i < chain.size(); ++i)
	{
		if (i > 0) result += " -> ";
		result += chain[i];
	}
	return result;
}

std::pair<wxString, std::vector<wxString>> resolve_name(const std::unordered_map<wxString, wxString>& alias, const wxString& name)
{
	auto lookup = name;

	std::unordered_set<wxString> visited;
	std::vector<wxString> alias_chain;
	alias_chain.push_back(name);
	int step = 0;

	while (true)
	{
		step += 1;
		auto it = alias.find(lookup);
		if (it == alias.end())
		{
			return {lookup, alias_chain};
		}
		if (visited.contains(lookup))
		{
			const auto chain = string_from_chain(alias_chain);
			wxLogWarning(
				_("Cyclic alias detected for %s at step %d (%s) resolved to %s"),
				name,
				step,
				chain,
				lookup
			);
			return {lookup, alias_chain};
		}
		visited.insert(lookup);
		alias_chain.push_back(it->second);
		lookup = it->second;
	}
}

std::optional<Style> FontsAndColors::GetStyle(const wxString& name) const
{
	const auto [resolved_name, alias_chain] = resolve_name(alias, name);
	const auto it = styles.find(resolved_name);
	if (it == styles.end()) return std::nullopt;
	return it->second;
}

std::optional<wxColor> FontsAndColors::GetColor(const wxString& name, const char* const display_name) const
{
	const auto [resolved_name, alias_chain] = resolve_name(alias, name);
	const auto it = colors.find(resolved_name);
	if (it == colors.end())
	{
		const auto chain = string_from_chain(alias_chain);
		wxLogWarning(
			_("Unable to find color for %s (%s) resolved to %s"),
			display_name, chain, resolved_name
		);
		return std::nullopt;
	}
	return C(it->second);
}

}

