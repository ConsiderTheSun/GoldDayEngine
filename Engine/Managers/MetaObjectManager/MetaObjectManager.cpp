#include "EngineIncludes.h"

#include "MetaObjectManager.h"

namespace gde::manager {
	MOM::MetaObjectManager(GoldDayEngine& _engine) :Manager(_engine), sum(_engine),gom(_engine),cam(_engine) {

	}
	MOM::~MetaObjectManager() {

	}

	GameObject& MOM::makeGameObject() {
		return gom.addGameObject();
	}
}