#pragma once

#include "../Manager.h"
#include "../Systems/System.h"
#include "../Systems/Renderer.h"
#include "../Systems/Light.h"
#include "../Systems/LightRenderer.h"

namespace gde::manager {
	class SystemUpkeepManager : public Manager {
	public:
		SystemUpkeepManager(GoldDayEngine& _engine);

		std::string getType() const override { return typeid(SystemUpkeepManager).name(); };

		template<typename T>
		void registerSystem(Signature signature);

		template<typename T>
		std::shared_ptr<T> getSystem();


		//template<typename T>
		//std::shared_ptr<T> getSystem(){}
	private:

		std::unordered_map<const char*, std::shared_ptr<system::System>> systems{};
	};

	typedef SystemUpkeepManager SUM;
}