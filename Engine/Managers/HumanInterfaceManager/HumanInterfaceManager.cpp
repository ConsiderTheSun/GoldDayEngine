#include "EngineIncludes.h"

#include "HumanInterfaceManager.h"
namespace gde::manager {
	HIM::HumanInterfaceManager(GoldDayEngine& _engine, Window& _window) :Manager(_engine), input(_window) {

	}

	HIM::~HumanInterfaceManager() {

	}

	void HIM::update() {
		input.update();
	}
}