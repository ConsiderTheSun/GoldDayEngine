#include "EngineIncludes.h"

#include "SystemUpkeepManager.h"


#include "../../GoldDayEngine.h" // TODO: rmv

namespace gde::manager {
	SystemUpkeepManager::SystemUpkeepManager(GoldDayEngine& _engine) : Manager(_engine) {

	}

	void SUM::registerRelevantGOIDsTracking(Signature signature) {
		auto a = relevantGOIDsMap.emplace(signature, GOIDContainer{});
		if (a.second == false) return;

		GOIDItr itr;
		GOIDItr end;
		engine.getMOM().getAllGOIDS(itr, end);
		for (; itr != end; itr++) {
			const GOID& id = *itr;
			if ((engine.getMOM().getSignature(id) & signature) == signature) {
				a.first->second.emplace(id);
			}

		}
	}

	void SUM::getRelevantGOIDs(Signature signature, GOIDContainer::const_iterator& begin, GOIDContainer::const_iterator& end) {
		assert(relevantGOIDsMap.find(signature) != relevantGOIDsMap.end() && "trying to get relevant GOIDS with unregistered signature");

		GOIDContainer& relevantGOIDS = relevantGOIDsMap.find(signature)->second;
		begin = relevantGOIDS.begin();
		end = relevantGOIDS.end();
	}

	void SUM::updateRelevantGOIDs(GOID id) {

		for (auto& kv : relevantGOIDsMap) {
			const Signature& relevantSignature = kv.first;
			if ((engine.getMOM().getSignature(id) & relevantSignature) == relevantSignature) {
				kv.second.emplace(id);
			}
			else {
				kv.second.erase(id);
			}
		}
	}
}