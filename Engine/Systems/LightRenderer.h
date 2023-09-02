#pragma once

namespace gde{ class GoldDayEngine; }
namespace gde::system {
	

	class LightRenderer {
	public:
		LightRenderer(GoldDayEngine& _engine);
		~LightRenderer();

		LightRenderer(const LightRenderer&) = delete;
		LightRenderer& operator=(const LightRenderer&) = delete;

		void renderLights();
	private:
		GoldDayEngine& engine;
		int pipelineIndex;
	};
}