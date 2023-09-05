#pragma once
#include "System.h"

#include <typeinfo>
namespace gde{ class GoldDayEngine; }
namespace gde::system {
	
	class Renderer : public System{
	public:
		Renderer(GoldDayEngine& _engine);
		~Renderer();

		std::string getType() const override { return typeid(Renderer).name(); };

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		 void setSignature() override;

		void renderGameObjects();
	private:

		int pipelineIndex;
	};
}