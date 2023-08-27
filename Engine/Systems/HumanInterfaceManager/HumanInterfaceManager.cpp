#include "HumanInterfaceManager.h"

namespace gde::system {
	HIM::HumanInterfaceManager(GoldDayEngine& _engine, Window& _window) :System(_engine), input(_window) {

	}

	HIM::~HumanInterfaceManager() {

	}

	void HIM::update() {
		input.update();
	}
}