#pragma once

#include "ComponentInterface.h"
namespace gde {
	class RenderInterface : public ComponentInterface {
	public:
		RenderInterface(GoldDayEngine& _engine) : ComponentInterface{ _engine } {};

		std::string getType() const override { return typeid(RenderInterface).name(); };

		component::Render makeComponent(const std::string& modelFile) {
			std::shared_ptr<Model> model = 
				gde::Model::createModelFromFile(engine.getGraphicsManager().getVkInterface().getDevice(), modelFile);
			return component::Render{ model };
		}
	};
}