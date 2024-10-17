#include "cabjiFunctions.h"

bool fnIgnoreLine(const std::string &str)
{
	// checks if a string is a comment or a blank line
	// return false if its a comment or blank, true if anything else
	return str.size() >= 2 && (str[0] == '/' && str[1] == '/') || str[0] == '#' || str == "";
}

std::set<int> fnParsePageSelection(const std::string &pageSelection, std::string *outString)
{
	std::set<int> pages;

	// Check if pageSelection contains only allowed characters: digits, commas, dashes, and spaces
	std::regex validPattern("^[0-9,\\-\\s]*$");
	if (!std::regex_match(pageSelection, validPattern))
	{
		if (outString != nullptr)
		{
			*outString += "Error: Invalid characters in page selection string.\n";
		}
		else
		{
			std::cout << "Error: Invalid characters in page selection string.\n";
		}

		return pages; // Return empty set if invalid characters are found
	}

	if (pageSelection.empty())
	{
		if (outString != nullptr)
		{
			*outString += "Error: Empty page selection string.\n";
		}
		else
		{
			std::cout << "Error: Empty page selection string.\n";
		}
		return pages; // Return empty set for empty input
	}

	// Split the input string by commas to get the elements
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
				if (outString != nullptr)
				{
					*outString += "Error: Invalid range in page selection: " + trimmedElement + "\n";
				}
				else
				{
					std::cout << "Error: Invalid range in page selection: " << trimmedElement << "\n";
				}
				return pages; // Return empty set if range is invalid
			}

			try
			{
				int start = std::stoi(range[0]);
				int end = std::stoi(range[1]);

				if (start > end)
					std::swap(start, end); // Ensure start is less than or equal to end

				for (int i = start; i <= end; ++i)
				{
					pages.insert(i);
				}
			}
			catch (const std::invalid_argument &e)
			{
				if (outString != nullptr)
				{
					*outString += "Error: Invalid number format in range: " + std::string(e.what()) + "\n";
				}
				else
				{
					std::cout << "Error: Invalid number format in range: " << std::string(e.what()) << "\n";
				}
				return pages;
			}
			catch (const std::out_of_range &e)
			{
				if (outString != nullptr)
				{
					*outString += "Error: Number out of range: " + std::string(e.what()) + "\n";
				}
				else
				{
					std::cout << "Error: Number out of range: " << std::string(e.what()) << "\n";
				}
				return pages;
			}
		}
		else
		{
			// Process single number
			try
			{
				pages.insert(std::stoi(trimmedElement));
			}
			catch (const std::invalid_argument &e)
			{
				if (outString != nullptr)
				{
					*outString += "Error: Invalid number format: " + std::string(e.what()) + "\n";
				}
				else
				{
					std::cout << "Error: Invalid number format: " << std::string(e.what()) << "\n";
				}
				return pages;
			}
			catch (const std::out_of_range &e)
			{
				if (outString != nullptr)
				{
					*outString += "Error: Number out of range: " + std::string(e.what()) + "\n";
				}
				else
				{
					std::cout << "Error: Number out of range: " << std::string(e.what()) << "\n";
				}
				return pages;
			}
		}
	}

	return pages;
}

std::set<int> fnParsePageSelection(const std::string &pageSelection)
{
	std::string fakeStr;
	return fnParsePageSelection(pageSelection, &fakeStr);
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

std::string fnStrStripExcessiveWhitespace(const std::string &input)
{
	std::stringstream result;
	std::string::const_iterator it = input.begin();
	bool inWhitespace = false;

	// Skip leading whitespace
	while (it != input.end() && std::isspace(*it))
	{
		++it;
	}

	while (it != input.end())
	{
		if (std::isspace(*it))
		{
			if (!inWhitespace)
			{
				result << ' ';
				inWhitespace = true;
			}
		}
		else
		{
			result << *it;
			inWhitespace = false;
		}
		++it;
	}

	std::string output = result.str();

	// Remove trailing whitespace
	if (!output.empty() && std::isspace(output.back()))
	{
		output.pop_back();
	}

	return output;
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
