#pragma once
#include "../Manager.h"
#include "Logger.h"
namespace gde::manager {
	// curently just seems to be a unnecessary encapsulation of logger but will
	// contain other debugging tools in the future
	// (ImGui stuff, data tracking, debug drawing, engine pause, debug camera, etc.)

	class DebugManager : public Manager {
	public:
		DebugManager(GoldDayEngine& _engine);
		~DebugManager();
		
		std::string getType() const override { return typeid(DebugManager).name(); };

		Logger& getLogger() { return logger; }

		bool isLightRenderingEnabled();
		void setLightRendering(bool _renderLights);
	private:
		Logger logger{};

		bool lightRenderingEnabled = false;
	};
}