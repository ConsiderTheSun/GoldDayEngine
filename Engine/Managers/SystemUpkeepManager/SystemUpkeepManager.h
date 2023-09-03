#pragma once

#include "../Manager.h"
#include "../Systems/System.h"
#include "../Systems/Renderer.h"
#include "../Systems/Light.h"
#include "../Systems/LightRenderer.h"

namespace gde {
	using GOIDContainer = std::unordered_set<GOID>;
	using GOIDItr = GOIDContainer::const_iterator;
}

namespace gde::manager {
	class SystemUpkeepManager : public Manager {
	public:
		SystemUpkeepManager(GoldDayEngine& _engine);

		std::string getType() const override { return typeid(SystemUpkeepManager).name(); };

		template<typename T>
		void registerSystem();

		template<typename T>
		std::shared_ptr<T> getSystem();

		void registerRelevantGOIDsTracking(Signature signature);
		void getRelevantGOIDs(Signature signature, GOIDItr& begin, GOIDItr& end);
		void updateRelevantGOIDs(GOID id);

	private:

		std::unordered_map<const char*, std::shared_ptr<system::System>> systems{};

		std::unordered_map<Signature, GOIDContainer> relevantGOIDsMap;
	};

	typedef SystemUpkeepManager SUM;
}