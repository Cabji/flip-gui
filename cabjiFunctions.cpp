#include "cabjiFunctions.h"

bool fnIgnoreLine(const std::string &str)
{
	// checks if a string is a comment or a blank line
	// return false if its a comment or blank, true if anything else
	return str.size() >= 2 && (str[0] == '/' && str[1] == '/') || str[0] == '#' || str == "";
}

void fnStrNormalizeNewLineChars(std::string &s)
{
	size_t pos = 0;
	// Replace all \r\n with \n
	while ((pos = s.find("\r\n", pos)) != std::string::npos)
	{
		s.replace(pos, 2, "\n");
		pos += 1; // Move past the '\n' we just inserted
	}
	// Replace all remaining \r with \n
	pos = 0;
	while ((pos = s.find("\r", pos)) != std::string::npos)
	{
		s.replace(pos, 1, "\n");
		pos += 1; // Move past the '\n' we just inserted
	}
}
