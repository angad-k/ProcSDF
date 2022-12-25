#include <vector>
#include <string>
#include <regex>
#pragma once
namespace custom
{
	std::vector<std::string> static tokenizeWithDelimiters(std::string p_string, std::string p_delimiter)
	{
		std::regex l_reg(p_delimiter);

		std::sregex_token_iterator l_iter(p_string.begin(), p_string.end(), l_reg, -1);
		std::sregex_token_iterator l_end;

		std::vector<std::string> l_tokens(l_iter, l_end);

		return l_tokens;
	}
}
