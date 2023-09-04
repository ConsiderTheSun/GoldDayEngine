#include "EngineIncludes.h"
#include "Logger.h"

#include <iostream>

namespace gde {

	void Logger::log(const Channel& outChannel, const std::string& msg) const{

		if (!channelEnabled[outChannel]) return;

		switch (outChannel)
		{
		case Verbose:
			SetConsoleTextAttribute(hConsole, 8);
			std::cout << "[vInfo] " << msg << std::endl;
			break;
		case Info:
			std::cout << "[info] " << msg << std::endl;
			return; // returns here to avoid unnecessary reset to text color
		case Warning:
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "[warning] " << msg << std::endl;
			break;
		case Error:
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "[error] " << msg << std::endl;
			break;
		}
		SetConsoleTextAttribute(hConsole, 15);
	}
}