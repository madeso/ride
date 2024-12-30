// Euphoria - Copyright (c) Gustav

/** @file
String utility functions
 */

#ifndef RIDE_STRINGUTILS_H_
#define RIDE_STRINGUTILS_H_

#include <string>
#include <vector>

/** @defgroup string String utility functions.
@{
 */

/** Space characters.
@returns the space characters.
 */
const std::string kSpaceCharacters = " \n\r\t";

/** Remove characters from the right, stops at a invalid character.
@param stringToTrim the string to remove characters from.
@param trimCharacters the characters to remove.
@returns the trimmed string.
 */
std::string TrimRight(
	const std::string& stringToTrim, const std::string& trimCharacters = kSpaceCharacters
);

/** Remove characters from the left, stops at invalid character.
@param stringToTrim the string to remove characters from.
@param trimCharacters the characters to remove.
@returns the trimmed string.
 */
std::string TrimLeft(
	const std::string& stringToTrim, const std::string& trimCharacters = kSpaceCharacters
);

/** Remove characters from both the start and the end.
@param stringToTrim the string to remove characters from.
@param trimCharacters the characters to remove.
@returns the trimmed string.
 */
std::string Trim(
	const std::string& stringToTrim, const std::string& trimCharacters = kSpaceCharacters
);

/** Tests if a string starts with another string.
@param stringToTest the string to test.
@param start the start of the string.
@returns true if the start match, false if not.
 */
bool StartsWith(const std::string& stringToTest, const std::string& start);

/** Tests if a string ends with another string.
@param stringToTest the string to test.
@param end the end of the string.
@returns true if the end match, false if not.
 */
bool EndsWith(const std::string& stringToTest, const std::string& end);

/** Generate a string containing only lower characters.
@param string the string to lower.
@returns the lowered string.
 */
std::string ToLower(const std::string& string);

/** Replace all occurrences in a string.
@param string the source string.
@param toFind the string to find.
@param toReplace the string to replace it.
 */
void StringReplace(std::string* string, const std::string& toFind, const std::string& toReplace);

/** Replace all occurrences in a string.
@param string the source string.
@param toFind the string to find.
@param toReplace the string to replace it.
@returns the modified string.
 */
const std::string StringReplace(
	const std::string& string, const std::string& toFind, const std::string& toReplace
);

/** Copy a string to a character buffer, adding null terminator at the end.
@param dst the destination buffer.
@param src the source string.
@param count the maximum number of characters to copy (including null
terminator)
 */
void Copy(char* dst, const std::string& src, const std::string::size_type& count);

std::string RemoveFromEnd(const std::string& str, const std::string& end);

std::vector<std::string> Tokenize(
	const std::string& input, const std::string& delimiters, bool remove_empties
);

/** @} */

#endif	// RIDE_STRINGUTILS_H_
