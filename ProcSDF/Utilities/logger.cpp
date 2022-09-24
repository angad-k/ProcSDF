#pragma once
#include<string>
#include<iostream>

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
		std::string output = "WARNING: " + message + "\n";
		std::cout << output;
	}
	static void log_error(std::string message)
	{
		std::string output = "ERROR: " + message + "\n";
		std::cout << output;
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