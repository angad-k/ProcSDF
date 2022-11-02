#pragma once
#include<string>
#include<iostream>

#define YELLOW_ANSI "\033[93m"
#define WHITE_ANSI "\033[0m"
#define RED_ANSI "\033[91m"

namespace logger
{
	static void log(std::string p_message)
	{
		printf(p_message.c_str());
	}
	static void logWarning(std::string p_message)
	{
		std::string l_output = "WARNING: " + p_message + "\n";
		std::cout.clear();
		std::cout << YELLOW_ANSI;
		std::cout << l_output;
		std::cout << WHITE_ANSI;
	}
	static void logError(std::string p_message)
	{
		std::string l_output = "ERROR: " + p_message + "\n";
		std::cout.clear();
		std::cout << RED_ANSI;
		std::cout << l_output;
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
		::logger::logWarning(msg);                                                                                    \
	}

#define ERR(m_Msg)                                                                                                  \
	{                                                                                                               \
		std::string msg = std::string(__FILE__) + ":" + std::to_string(__LINE__) + ":" + std::string(__FUNCTION__) + ": " + m_Msg; \
		::logger::logError(msg);                                                                                      \
	}