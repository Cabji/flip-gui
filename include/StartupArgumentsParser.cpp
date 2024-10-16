#include "StartupArgumentsParser.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

// Utility Functions
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

// Constructors and Assignment Operator
StartupArgumentsParser::StartupArgumentsParser() {}

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

// Class Methods
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
		std::cout << "--" << entry.first << " = "
				  << (entry.second.empty() ? "(empty)" : entry.second) << std::endl;
	}
}

bool StartupArgumentsParser::HasSwitch(const std::string &switchName) const
{
	std::string normalizedSwitch = ToLower(switchName);
	return std::any_of(m_arguments.begin(), m_arguments.end(),
					   [&](const auto &entry)
					   { return entry.first == normalizedSwitch; });
}

bool StartupArgumentsParser::IsInteger(const std::string &str) const
{
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

bool StartupArgumentsParser::IsSwitch(const std::string &arg) const
{
	return arg.starts_with("--") || arg.starts_with("-") || arg.starts_with("/");
}

bool StartupArgumentsParser::IsSwitchPresent(const std::string &switchName) const
{
	std::string normalizedSwitch = NormalizeSwitchName(switchName);
	return std::any_of(m_arguments.begin(), m_arguments.end(),
					   [&](const auto &entry)
					   { return entry.first == normalizedSwitch; });
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
	auto it = std::find_if(m_arguments.begin(), m_arguments.end(),
						   [&](const auto &entry)
						   { return entry.first == ToLower(switchName); });
	return it != m_arguments.end() ? it->second : defaultValue;
}

std::optional<std::string> StartupArgumentsParser::GetSwitchValue(const std::string &switchName) const
{
	std::string normalizedSwitch = NormalizeSwitchName(switchName);
	auto it = std::find_if(m_arguments.begin(), m_arguments.end(),
						   [&](const auto &entry)
						   { return entry.first == normalizedSwitch; });
	return (it != m_arguments.end() && !it->second.empty()) ? std::optional(it->second) : std::nullopt;
}

void StartupArgumentsParser::Parse(int argc, char *argv[])
{
	std::string currentSwitch;

	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];

		if (IsSwitch(arg))
		{
			currentSwitch = NormalizeSwitchName(arg);

			if ((i + 1) < argc && !IsSwitch(argv[i + 1]))
			{
				m_arguments.emplace_back(currentSwitch, argv[i + 1]);
			}
			else
			{
				m_arguments.emplace_back(currentSwitch, "true");
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
			size_t dashPos = token.find('-');
			int start = SafeStringToInt(token.substr(0, dashPos), -1);
			int end = SafeStringToInt(token.substr(dashPos + 1), -1);

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
	catch (...)
	{
		return defaultValue;
	}
}
