#include "EngineIncludes.h"


#include "GameObjectInterface.h"

namespace gde {
	GOInterface::GameObjectInterface(GoldDayEngine& _engine) :engine{ _engine } {

	}
	const GOID& GOInterface::makeEmptyGameObject() {
		return engine.getMOM().makeGameObject();
	}
}