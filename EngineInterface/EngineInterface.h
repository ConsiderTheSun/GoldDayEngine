#pragma once

#include "../GoldDayEngine.h"

#include "Application.h"

#include "GameObjectInterface.h"
#include "GameObjectInterface.inl"

#include "ComponentInterface/ComponentInterface.h"
#include "ComponentInterface/TransformInterface.h"
#include "ComponentInterface/RenderInterface.h"
#include "ComponentInterface/PointLightInterface.h"
#include "ComponentInterface/CameraInterface.h"




namespace gde {
	constexpr int COMPONENT_COUNT = 4;
	struct InterfaceData {
		std::pair<const char*, const char*> componentToI[COMPONENT_COUNT] = {
			std::pair<const char*, const char*>(typeid(TransformInterface).name(),typeid(component::Transform).name()),
			std::pair<const char*, const char*>(typeid(RenderInterface).name(),typeid(component::Render).name()),
			std::pair<const char*, const char*>(typeid(PointLightInterface).name(),typeid(component::PointLight).name()),
			std::pair<const char*, const char*>(typeid(CameraInterface).name(),typeid(component::Camera).name()),
		};
	};
	

	class EngineInterface {
	public:
		EngineInterface(const std::string& windowName, glm::vec2 windowDimentions) 
			:engine{ windowName, windowDimentions }, iGO{ engine },
			componentToInterfaceMap{ iData.componentToI, iData.componentToI + COMPONENT_COUNT } {
			componentInterfaces.insert({ typeid(component::Transform).name(), std::make_unique<TransformInterface>(engine) });
			componentInterfaces.insert({ typeid(component::Render).name(), std::make_unique<RenderInterface>(engine) });
			componentInterfaces.insert({ typeid(component::PointLight).name(), std::make_unique<PointLightInterface>(engine) });
			componentInterfaces.insert({ typeid(component::Camera).name(), std::make_unique<CameraInterface>(engine) });
		}

		EngineInterface(const EngineInterface&) = delete;
		EngineInterface& operator=(const EngineInterface&) = delete;

		// TODO: remove when scripting is added
		void setApp(Application* appPtr) {
			engine.appPtr = appPtr;
		};

		void start() { engine.start(); }
		void end() { engine.end(); }

		template<typename T>
		void registerComponent() { 
			engine.getMOM().registerComponent<T>(); 
		}
		template<typename T>
		void registerSystem() { engine.getMOM().registerSystem<T>(); }

		GOInterface& getGOInterface() { return iGO; }

		template<typename T1>
		std::shared_ptr<T1> getComponentInterface() {
			auto component = componentToInterfaceMap.find(typeid(T1).name())->second;
			return std::static_pointer_cast<T1>(componentInterfaces.find(component)->second);
		}

		// these are simple enough that they doesn't need an interface and can safely be passes to the app layer, for now
		Input& getInput() { return engine.getHIM().getInput(); }
		manager::DebugManager& getDebugger() { return engine.getDebugManager(); }

		template<typename T>
		Signature getSignature() {
			return engine.getMOM().getSystem<T>()->getSignature();
		}

		GoldDayEngine& getEngine() { return engine; } // TODO: replace with robust interface 

	private:
		GoldDayEngine engine;
		InterfaceData iData;
		GOInterface iGO;

		std::unordered_map<const char*, std::shared_ptr<ComponentInterface>> componentInterfaces{};

		

		std::unordered_map<const char*, const char*>componentToInterfaceMap;
	};
}