#pragma once

#include "../Manager.h"
#include <glm/glm.hpp>

#include "Input.h"

namespace gde::manager {
	class HumanInterfaceManager : public Manager {
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