#pragma once

#include "../System.h"
#include <glm/glm.hpp>

#include "Input.h"

namespace gde::system {
	class HumanInterfaceManager : public System {
	public:
		HumanInterfaceManager(GoldDayEngine& _engine, Window& _window);
		~HumanInterfaceManager();

		HumanInterfaceManager(const HumanInterfaceManager&) = delete;
		HumanInterfaceManager& operator=(const HumanInterfaceManager&) = delete;

		std::string getType() const override { return typeid(HumanInterfaceManager).name(); };

		Input& getInput() { return input; }

		void update();
	private:
		Input input;
	};

	typedef HumanInterfaceManager HIM;
}