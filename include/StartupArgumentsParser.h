#ifndef STARTUP_ARGUMENTS_PARSER_H
#define STARTUP_ARGUMENTS_PARSER_H

#include <map>
#include <optional>
#include <string>
#include <vector>

// Alias for storing parsed arguments
using ArgMap = std::map<std::string, std::string>;

class StartupArgumentsParser
{
private:
	ArgMap m_arguments;
	std::vector<std::string> m_errors;

	bool IsInteger(const std::string &str) const;
	bool IsSwitch(const std::string &arg) const;
	std::string NormalizeSwitchName(const std::string &arg) const;
	int SafeStringToInt(const std::string &str, int defaultValue = 0) const;

public:
	// Constructors and operator=
	StartupArgumentsParser();
	StartupArgumentsParser(int argc, char *argv[]);
	StartupArgumentsParser(const StartupArgumentsParser &other);
	StartupArgumentsParser &operator=(const StartupArgumentsParser &other);

	void DisplayAllSwitches() const;
	const ArgMap &GetArguments() const;
	const std::vector<std::string> &GetErrors() const;
	std::optional<std::string> GetSwitchValue(const std::string &switchName) const;
	std::string GetValue(const std::string &switchName, const std::string &defaultValue = "") const;
	bool HasSwitch(const std::string &switchName) const;
	bool IsSwitchPresent(const std::string &switchName) const;
	void Parse(int argc, char *argv[]);
	std::vector<int> ParsePageRange(const std::string &pagesInput) const;
};

#endif // STARTUP_ARGUMENTS_PARSER_H
