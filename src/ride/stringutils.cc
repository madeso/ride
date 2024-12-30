// Euphoria - Copyright (c) Gustav

#include "ride/stringutils.h"

#include <cassert>
#include <string>
#include <algorithm>
#include <cstring>
#include <vector>

std::string TrimRight(const std::string& stringToTrim, const std::string& trimCharacters)
{
	return std::string(stringToTrim).erase(stringToTrim.find_last_not_of(trimCharacters) + 1);
}

std::string TrimLeft(const std::string& stringToTrim, const std::string& trimCharacters)
{
	return std::string(stringToTrim).erase(0, stringToTrim.find_first_not_of(trimCharacters));
}

std::string Trim(const std::string& stringToTrim, const std::string& trimCharacters)
{
	return TrimRight(TrimLeft(stringToTrim, trimCharacters), trimCharacters);
}

bool StartsWith(const std::string& stringToTest, const std::string& start)
{
	const std::string::size_type length = start.length();
	const std::string::size_type otherLength = stringToTest.length();
	if (otherLength < length)
	{
		return false;
	}
	const std::string actualStart = stringToTest.substr(0, length);
	return start == actualStart;
}

bool EndsWith(const std::string& stringToTest, const std::string& end)
{
	const std::string::size_type length = end.length();
	const std::string::size_type otherLength = stringToTest.length();
	if (otherLength < length)
	{
		return false;
	}
	const std::string actualEnd = stringToTest.substr(otherLength - length, length);
	return end == actualEnd;
}

std::string ToLower(const std::string& string)
{
	std::string result = string;
	std::transform(result.begin(), result.end(), result.begin(), tolower);
	return result;
}

void StringReplace(std::string* string, const std::string& toFind, const std::string& toReplace)
{
	std::size_t index = string->find(toFind);
	const std::size_t findLength = toFind.length();
	while (index != std::string::npos)
	{
		string->erase(index, findLength);
		string->insert(index, toReplace);
		index = string->find(toFind, index);
	}
}

const std::string StringReplace(
	const std::string& string, const std::string& toFind, const std::string& toReplace
)
{
	std::string temp = string;
	StringReplace(&temp, toFind, toReplace);
	return temp;
}

void Copy(char* dst, const std::string& src, const std::string::size_type& count)
{
	strncpy(dst, src.c_str(), count - 1);
	dst[count - 1] = 0;
}

std::string RemoveFromEnd(const std::string& str, const std::string& end)
{
	if (EndsWith(str, end))
	{
		const auto new_length = str.length() - end.length();
		if (new_length == 0) return "";
		assert(new_length > 0);
		return str.substr(0, new_length);
	}
	else
	{
		return str;
	}
}

namespace
{
// source: http://stackoverflow.com/a/9676623
template<typename T>
std::vector<T> Split(const T& str, const T& delimiters, bool remove_empties)
{
	std::vector<T> v;
	std::size_t start = 0;
	auto pos = str.find_first_of(delimiters, start);
	while (pos != T::npos)
	{
		if (pos == start && remove_empties)
		{
			// ignore empty tokens
		}
		else
		{
			v.emplace_back(str, start, pos - start);
		}
		start = pos + 1;
		pos = str.find_first_of(delimiters, start);
	}
	if (start < str.length())  // ignore trailing delimiter
		v.emplace_back(str, start,
					   str.length() - start);  // add what's left of the string
	return v;
}
}  // namespace

std::vector<std::string> Tokenize(
	const std::string& input, const std::string& delimiters, bool remove_empties
)
{
	return Split(input, delimiters, remove_empties);
}
