#pragma once

#include "../GoldDayEngine.h"

namespace gde {
	class ComponentInterface {
	public:
		ComponentInterface(GoldDayEngine& _engine): engine{_engine}{};

		ComponentInterface(const ComponentInterface&) = delete;
		ComponentInterface& operator=(const ComponentInterface&) = delete;

		virtual std::string getType() const = 0;

	protected:
		GoldDayEngine& engine;
	};
}