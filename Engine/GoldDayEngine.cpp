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

        

        gde::GOIDItr itr;
        gde::GOIDItr end;
        metaObjectManager.getRelevantGOIDs(metaObjectManager.getSystem<system::CameraUpdate>()->getSignature(), itr, end);

        int cameraCount = 0;
        for (; itr != end; itr++) {
            ++cameraCount;
        }

        if (cameraCount > 1) {
            debugManager.getLogger().log(Logger::Warning, "Multicamera not implemented, using first camera");
        }
        if (cameraCount == 0) {
            debugManager.getLogger().log(Logger::Error, "No camera in scene");
        }

        // sets all cameras to the initial aspect ratio
        metaObjectManager.getSystem<system::CameraUpdate>()->setCameraAspectRatio();

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