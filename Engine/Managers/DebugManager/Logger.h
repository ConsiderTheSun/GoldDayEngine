#pragma once

#include <string>
#include <vector>
#include <windows.h>

namespace gde {

	class Logger {
	public:
		enum Channel { Verbose, Info, Warning, Error };

		Logger() { log(Verbose, "Logger Instantiated"); };
		~Logger() { log(Verbose, "Logger Freed"); };

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		void log(const Channel& outChannel, const std::string& msg) const;

		// sets if logging to a channel will actually output
		void setChannel(const Channel& c, bool enabled) { channelEnabled[c] = enabled; };
	private:

		const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


#if defined (NDEBUG)
		std::vector<bool> channelEnabled{ false,false,false,false };
#else
		std::vector<bool> channelEnabled{ true,true,true,true };
#endif
	};
}