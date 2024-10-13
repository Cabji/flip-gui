#include "StartupArgumentsParser.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

// Utility functions
std::vector<std::string> Split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::istringstream stream(str);
	std::string token;
	while (std::getline(stream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

std::string ToLower(const std::string &str)
{
	std::string lowerStr = str;
	std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
	return lowerStr;
}

// Constructors and operator=
StartupArgumentsParser::StartupArgumentsParser() {};

StartupArgumentsParser::StartupArgumentsParser(int argc, char *argv[])
{
	Parse(argc, argv);
}

StartupArgumentsParser::StartupArgumentsParser(const StartupArgumentsParser &other)
	: m_arguments(other.m_arguments), m_errors(other.m_errors) {}

StartupArgumentsParser &StartupArgumentsParser::operator=(const StartupArgumentsParser &other)
{
	if (this != &other)
	{
		m_arguments = other.m_arguments;
		m_errors = other.m_errors;
	}
	return *this;
}

// // Constructor
// StartupArgumentsParser::StartupArgumentsParser(int argc, char *argv[])
// {
// 	std::string currentSwitch;
// 	for (int i = 1; i < argc; ++i)
// 	{
// 		std::string arg = argv[i];

// 		if (IsSwitch(arg))
// 		{
// 			currentSwitch = NormalizeSwitchName(arg);
// 			m_arguments[currentSwitch] = ""; // Initialize with empty value
// 		}
// 		else if (!currentSwitch.empty())
// 		{
// 			m_arguments[currentSwitch] = arg;
// 			currentSwitch.clear(); // Reset switch after assigning value
// 		}
// 	}
// }

// Class methods
void StartupArgumentsParser::DisplayAllSwitches() const
{
	if (m_arguments.empty())
	{
		std::cout << "No switches provided." << std::endl;
		return;
	}

	std::cout << "Parsed switches and their values:" << std::endl;
	for (const auto &entry : m_arguments)
	{
		std::cout << "--" << entry.first << " = " << (entry.second.empty() ? "(empty)" : entry.second) << std::endl;
	}
}

bool StartupArgumentsParser::HasSwitch(const std::string &switchName) const
{
	return m_arguments.find(ToLower(switchName)) != m_arguments.end();
}

bool StartupArgumentsParser::IsInteger(const std::string &str) const
{
	if (str.empty())
		return false;
	for (char c : str)
	{
		if (!isdigit(c))
			return false;
	}
	return true;
}

bool StartupArgumentsParser::IsSwitch(const std::string &arg) const
{
	return (arg.starts_with("--") || arg.starts_with("-") || arg.starts_with("/"));
}

bool StartupArgumentsParser::IsSwitchPresent(const std::string &switchName) const
{
	std::string normalizedSwitch = NormalizeSwitchName(switchName);
	return m_arguments.find(normalizedSwitch) != m_arguments.end();
}

const ArgMap &StartupArgumentsParser::GetArguments() const
{
	return m_arguments;
}

const std::vector<std::string> &StartupArgumentsParser::GetErrors() const
{
	return m_errors;
}

std::string StartupArgumentsParser::GetValue(const std::string &switchName, const std::string &defaultValue) const
{
	auto it = m_arguments.find(ToLower(switchName));
	if (it != m_arguments.end())
	{
		return it->second;
	}
	return defaultValue;
}

std::optional<std::string> StartupArgumentsParser::GetSwitchValue(const std::string &switchName) const
{
	std::string normalizedSwitch = NormalizeSwitchName(switchName);
	auto it = m_arguments.find(normalizedSwitch);
	if (it != m_arguments.end() && !it->second.empty())
	{
		return it->second;
	}
	return std::nullopt;
}

void StartupArgumentsParser::Parse(int argc, char *argv[])
{
	std::string currentSwitch;

	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (IsSwitch(arg))
		{
			currentSwitch = NormalizeSwitchName(arg);

			// Ensure normalized switch name is used consistently
			if ((i + 1) < argc && !IsSwitch(argv[i + 1]))
			{
				m_arguments[currentSwitch] = argv[++i];
			}
			else
			{
				m_arguments[currentSwitch] = "true";
			}
		}
		else
		{
			m_errors.push_back("Unexpected argument: " + arg);
		}
	}
}

std::vector<int> StartupArgumentsParser::ParsePageRange(const std::string &pagesInput) const
{
	std::vector<int> pageNumbers;
	std::istringstream stream(pagesInput);
	std::string token;

	while (std::getline(stream, token, ','))
	{
		if (token.find('-') != std::string::npos)
		{
			// Handle range: e.g., "3-5"
			size_t dashPos = token.find('-');
			std::string startStr = token.substr(0, dashPos);
			std::string endStr = token.substr(dashPos + 1);

			int start = SafeStringToInt(startStr, -1);
			int end = SafeStringToInt(endStr, -1);

			if (start > 0 && end >= start)
			{
				for (int i = start; i <= end; ++i)
				{
					pageNumbers.push_back(i);
				}
			}
			else
			{
				std::cerr << "Invalid page range: " << token << std::endl;
			}
		}
		else
		{
			// Handle single page numbers
			int page = SafeStringToInt(token, -1);
			if (page > 0)
			{
				pageNumbers.push_back(page);
			}
			else
			{
				std::cerr << "Invalid page number: " << token << std::endl;
			}
		}
	}
	return pageNumbers;
}

std::string StartupArgumentsParser::NormalizeSwitchName(const std::string &arg) const
{
	std::string switchName = arg;
	if (switchName.starts_with("--") || switchName.starts_with("-") || switchName.starts_with("/"))
	{
		switchName = switchName.substr(switchName.find_first_not_of("-/"));
	}
	return ToLower(switchName);
}

int StartupArgumentsParser::SafeStringToInt(const std::string &str, int defaultValue) const
{
	try
	{
		return std::stoi(str);
	}
	catch (const std::invalid_argument &e)
	{
		std::cerr << "Invalid argument for integer conversion: " << str << std::endl;
		return defaultValue;
	}
	catch (const std::out_of_range &e)
	{
		std::cerr << "Out of range error for integer conversion: " << str << std::endl;
		return defaultValue;
	}
}
