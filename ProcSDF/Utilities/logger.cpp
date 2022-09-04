#pragma once
#include<string>

namespace logger
{
	static void log(std::string message)
	{
		printf(message.c_str());
	}
	static void log(std::string message, bool is_debug)
	{
		printf(message.c_str());
	}
	static void log_warning(std::string message)
	{
		printf(message.c_str());
	}
	static void log_error(std::string message)
	{
		printf(message.c_str());
	}
}