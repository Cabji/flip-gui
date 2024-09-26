#include "cabjiFunctions.h"

bool fnIgnoreLine(const std::string &str)
{
	// checks if a string is a comment or a blank line
	// return false if its a comment or blank, true if anything else
	return str.size() >= 2 && (str[0] == '/' && str[1] == '/') || str[0] == '#' || str == "";
}

std::set<int> fnParsePageSelection(const std::string &pageSelection)
{
	std::set<int> pages;
	std::vector<std::string> elements = fnStrSplitToVector(pageSelection, ",");

	for (const auto &element : elements)
	{
		std::string trimmedElement = fnTrim(element);

		if (trimmedElement.find('-') != std::string::npos)
		{
			// Process range
			std::vector<std::string> range = fnStrSplitToVector(trimmedElement, "-");
			if (range.size() != 2)
			{
				throw std::invalid_argument("Invalid range: " + trimmedElement);
			}

			int start = std::stoi(range[0]);
			int end = std::stoi(range[1]);
			if (start > end)
				std::swap(start, end); // Ensure start is less than end

			for (int i = start; i <= end; ++i)
			{
				pages.insert(i);
			}
		}
		else
		{
			// Process single number
			pages.insert(std::stoi(trimmedElement));
		}
	}
	return pages;
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

// split a string on a delimiter and return it as a vector
std::vector<std::string> fnStrSplitToVector(const std::string &s, const std::string &delimiter)
{
	std::vector<std::string> tokens;
	std::string::size_type start = 0;
	std::string::size_type end = s.find(delimiter);

	while (end != std::string::npos)
	{
		tokens.push_back(s.substr(start, end - start));
		start = end + delimiter.length();
		end = s.find(delimiter, start);
	}

	tokens.push_back(s.substr(start));
	return tokens;
}

// Function to trim whitespace from both ends of a string
std::string fnTrim(const std::string &str)
{
	std::string result = str;

	// Trim from the start
	result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch)
											  { return !std::isspace(ch); }));

	// Trim from the end
	result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch)
							  { return !std::isspace(ch); })
					 .base(),
				 result.end());

	return result;
}
