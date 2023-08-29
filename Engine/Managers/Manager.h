#pragma once

#include <typeinfo>
#include <string>

namespace gde { class GoldDayEngine; }

namespace gde::manager {
	class Manager {
	public:
		Manager(GoldDayEngine& _engine):engine(_engine) {};

		Manager(const Manager&) = delete;
		Manager& operator=(const Manager&) = delete;

		virtual std::string getType() const = 0;

	
		GoldDayEngine& engine;
	};
}