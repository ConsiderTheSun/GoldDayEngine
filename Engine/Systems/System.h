#pragma once

#include <typeinfo>
#include <string>

namespace gde { class GoldDayEngine; }

namespace gde::system {
	class System {
	public:
		System(GoldDayEngine& e):engine(e) {};

		System(const System&) = delete;
		System& operator=(const System&) = delete;

		virtual std::string GetType() const = 0;

	
		GoldDayEngine& engine;
	};
}