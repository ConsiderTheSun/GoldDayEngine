#pragma once
#include "System.h"
namespace gde{ class GoldDayEngine; }
namespace gde::system {
	

	class LightRenderer : public System {
	public:
		LightRenderer(GoldDayEngine& _engine);
		~LightRenderer();
		std::string getType() const override { return typeid(LightRenderer).name(); };

		LightRenderer(const LightRenderer&) = delete;
		LightRenderer& operator=(const LightRenderer&) = delete;

		void setSignature() override;

		void renderLights();
	private:
		int pipelineIndex;
	};
}