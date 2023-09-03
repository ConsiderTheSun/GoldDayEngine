#include "EngineIncludes.h"

#include "GoldDayEngine.h"

#include "Managers/GameObjectManager/GameObjectManager.h"
#include "Managers/GameObjectManager/GameObjectManager.inl"


#include "../EngineInterface/Application.h" // TODO: remove when scripting is added


namespace gde {

	GoldDayEngine::GoldDayEngine(const std::string& windowName, glm::vec2 windowDimentions) :
			debugManager(*this), 
			graphicsManager(*this, windowName, windowDimentions),
            humanInterfaceManager(*this, graphicsManager.getWindow()),
            metaObjectManager(*this) {

		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Instantiated");
    }

	GoldDayEngine::~GoldDayEngine(){
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Freed");

	}

	void GoldDayEngine::start() {
		debugManager.getLogger().log(Logger::Info, "GoldDayEngine Starting");

        // TODO: gross, will be fixed w/ camera component 
        cameraID = metaObjectManager.makeGameObject();

        metaObjectManager.addComponent<component::Transform>(cameraID,
            component::Transform{glm::vec3(0,0,-2.5), glm::vec3(0), 1.f});

        // TODO: prob wanna move this function to the camera component class
        graphicsManager.setCameraAspectRatio(graphicsManager.getVkInterface().getAspectRatio());

        // TODO: remove this, have it set in application layer
        graphicsManager.mainCamera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

        currentTime = std::chrono::high_resolution_clock::now();
		while (!graphicsManager.getWindow().shouldClose()) {
			graphicsManager.getWindow().pollEvents();

            float deltaTime = getDeltaTime();

            humanInterfaceManager.update();

            appPtr->update(deltaTime); // TODO: replace with scripting

            graphicsManager.drawFrame(deltaTime);
		}

		graphicsManager.waitIdle();
	}

    void GoldDayEngine::end() { 
        graphicsManager.getWindow().setClose();
    }

    float GoldDayEngine::getDeltaTime() {
        auto newTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;
        
        return dt;
    }
}