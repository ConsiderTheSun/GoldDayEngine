#pragma once

#include <typeinfo>
#include <string>

namespace gde { class GoldDayEngine; }

namespace gde::system {
	class System {
	public:
		System(GoldDayEngine& _engine):engine(_engine) {};

		System(const System&) = delete;
		System& operator=(const System&) = delete;

		virtual std::string getType() const = 0;

	
		GoldDayEngine& engine;
	};
}