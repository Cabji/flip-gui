#ifndef CABJI_FUNCTIONS_H
#define CABJI_FUNCTIONS_H

#include <algorithm>
#include <cctype>
#include <iostream>
#include <regex>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Checks if a std::string should be ignored by calling processing code
 * @param str std::string - The string to check
 * @return bool - true: the string should be ignored; false: the string should not be ignored
 */
bool fnIgnoreLine(const std::string &str);

/**
 * @brief Parses the page selection string from user input
 * @param pageSelection std::string - String to parse
 * @param outString std::string - Output string
 */
std::set<int> fnParsePageSelection(const std::string &pageSelection, std::string *outString);

/**
 * @brief Parses the page selection string from user input
 * @param pageSelection std::string - String to parse
 */
std::set<int> fnParsePageSelection(const std::string &pageSelection);

/**
 * @brief Converts newline characters in a std::string to UNIX newlines
 * @param s std::string - String to convert
 * @return void
 * @details Use this function as part of input data sanitization process, to help minimise cross-platform issues.
 */
void fnStrNormalizeNewLineChars(std::string &s);

/**
 * @brief Splits a string on delimiter and returns results as a vector<string>
 * @param s std::string - String to split
 * @param delimiter std::string - String delimiter to split s on
 */
std::vector<std::string> fnStrSplitToVector(const std::string &s, const std::string &delimiter);

/**
 * @brief Strips excessive whitespace (including tabs, newlines) from a string
 * @param input std::string - String to normalize
 */
std::string fnStrStripExcessiveWhitespace(const std::string &input);

/**
 * @brief Trim excessive whitespace from start and end of std::string
 * @param str std::string - String to convert
 * @return std::string
 * @details This function is used by functions in the cabjiFunctions library.
 */
std::string fnTrim(const std::string &str);

#endif // CABJI_FUNCTIONS_H
