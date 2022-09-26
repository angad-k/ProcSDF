#pragma once
#include<string>
#include<iostream>

#define YELLOW_ANSI "\033[93m"
#define WHITE_ANSI "\033[0m"
#define RED_ANSI "\033[91m"

namespace logger
{
	static void log(std::string message)
	{
		printf(message.c_str());
	}
	static void log_warning(std::string message)
	{
		std::string output = "WARNING: " + message + "\n";
		std::cout.clear();
		std::cout << YELLOW_ANSI;
		std::cout << output;
		std::cout << WHITE_ANSI;
	}
	static void log_error(std::string message)
	{
		std::string output = "ERROR: " + message + "\n";
		std::cout.clear();
		std::cout << RED_ANSI;
		std::cout << output;
		std::cout << WHITE_ANSI;
	}
}

/// Following macros were picked up from the below repository and modified according to requirements
/// https://github.com/sdslabs/Rootex
#define PRINT(m_Msg)                                      \
	{                                                     \
		std::string msg = std::string(__FUNCTION__) + ": " + m_Msg; \
		::logger::log(msg);                                 \
	}

#define WARN(m_Msg)                                                                                                 \
	{                                                                                                               \
		std::string msg = std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(__FUNCTION__) + ": " + m_Msg; \
		::logger::log_warning(msg);                                                                                    \
	}

#define ERR(m_Msg)                                                                                                  \
	{                                                                                                               \
		std::string msg = std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(__FUNCTION__) + ": " + m_Msg; \
		::logger::log_error(msg);                                                                                      \
	}