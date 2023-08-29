#pragma once

// std
#include <memory>
#include <vector>
namespace gde{ class GoldDayEngine; }
namespace gde::system {
	
	class Renderer {
	public:
		Renderer(GoldDayEngine& _engine);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void renderGameObjects();
	private:
		GoldDayEngine& engine;

		int pipelineIndex;
	};
}