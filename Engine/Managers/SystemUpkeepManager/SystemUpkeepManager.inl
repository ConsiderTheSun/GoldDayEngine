#include "SystemUpkeepManager.h"

namespace gde::manager {
	template<typename T>
	void SUM::registerSystem()
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

		// Create a pointer to the system
		auto system = std::make_shared<T>(engine);
		system->setSignature();
		systems.insert({ typeid(T).name(), system });

		registerRelevantGOIDsTracking(system->getSignature());
	}

	template<typename T>
	std::shared_ptr<T> SUM::getSystem()
	{

		return std::static_pointer_cast<T>(systems[typeid(T).name()]);
	}
}