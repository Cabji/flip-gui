#ifndef CABJI_FUNCTIONS_H
#define CABJI_FUNCTIONS_H

#include <string>

/**
 * @brief Checks if a std::string should be ignored by calling processing code
 * @param str std::string - The string to check
 * @return bool - true: the string should be ignored; false: the string should not be ignored
 */
bool fnIgnoreLine(const std::string &str);

/**
 * @brief Converts newline characters in a std::string to UNIX newlines
 * @param s std::string - String to convert
 * @return void
 * @details Use this function as part of input data sanitization process, to help minimise cross-platform issues.
 */
void fnStrNormalizeNewLineChars(std::string &s);
#endif // CABJI_FUNCTIONS_H
