#ifndef CABJI_FUNCTIONS_H
#define CABJI_FUNCTIONS_H

#include <string>

/**
 * @brief Checks is a string should be ignored by calling processing code
 * @param str std::string - The string to check
 * @return bool - true: the string should be ignored; false: the string should not be ignored
 */
bool fnIgnoreLine(const std::string &str);

#endif CABJI_FUNCTIONS_H

bool fnIgnoreLine(const std::string &str)
{
	// checks if a string is a comment or a blank line
	// return false if its a comment or blank, true if anything else
	return str.size() >= 2 && (str[0] == '/' && str[1] == '/') || str[0] == '#' || str == "";
}