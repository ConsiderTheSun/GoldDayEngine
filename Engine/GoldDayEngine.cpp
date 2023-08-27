
#include "GoldDayEngine.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <chrono>

namespace gde {

	GoldDayEngine::GoldDayEngine(std::string windowName, glm::vec2 windowDimentions) :
			debugManager(*this), 
			graphicsManager(*this, windowName, windowDimentions),
            humanInterfaceManager(*this, graphicsManager.getWindow()) {
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Instantiated");

		loadGameObjectsTEMP();
        humanInterfaceManager.getInput().addTracking(6, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
        humanInterfaceManager.getInput().addTracking(2, GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_ESCAPE);
        humanInterfaceManager.getInput().lockMouse();
    }

	GoldDayEngine::~GoldDayEngine(){
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Freed");

	}

	void GoldDayEngine::start() {
		debugManager.getLogger().log(Logger::Info, "GoldDayEngine Starting");

        // gross, will be fixed w/ camera component
        auto viewerObject = GameObject::createGameObject();

        auto currentTime = std::chrono::high_resolution_clock::now();
		while (!graphicsManager.getWindow().shouldClose()) {
			graphicsManager.getWindow().pollEvents();

            // put into frame rate controller later
            auto newTime = std::chrono::high_resolution_clock::now();
            deltaTime =
                std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            humanInterfaceManager.update();

            cameraControllerTEMP(deltaTime, viewerObject);

            graphicsManager.updateCamera();
			graphicsManager.drawFrame();

		}

		graphicsManager.waitIdle();
	}

    void GoldDayEngine::end() { 
        graphicsManager.getWindow().setClose();
    }


	void GoldDayEngine::loadGameObjectsTEMP() {
        


        std::shared_ptr<Model> model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/flat_vase.obj");
        auto flatVase = GameObject::createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = { -.5f, .5f, 2.5f };
        flatVase.transform.scale = 1.5f;
        gameObjects.push_back(std::move(flatVase));

        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/smooth_vase.obj");
        auto smoothVase = GameObject::createGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = { .5f, .5f, 2.5f };
        smoothVase.transform.scale = 1.5f;
        gameObjects.push_back(std::move(smoothVase));
	}
    

    void GoldDayEngine::cameraControllerTEMP(float dt, GameObject& gameObject) {
        static bool locked = true;

        if (humanInterfaceManager.getInput().keyDown(GLFW_KEY_ESCAPE) == Input::State::Enter) {
            end();
        }

        // random rotation
        gameObjects[0].transform.rotation.y = glm::mod(gameObjects[0].transform.rotation.y + 0.0001f, glm::two_pi<float>());
        gameObjects[0].transform.rotation.x = glm::mod(gameObjects[0].transform.rotation.x + 0.00005f, glm::two_pi<float>());


        Input& input = humanInterfaceManager.getInput();
        float moveSpeed{ 3.f };
        float lookSpeed{ 30.0f };

        if (humanInterfaceManager.getInput().keyDown(GLFW_MOUSE_BUTTON_LEFT) == Input::State::Enter) {
            if (locked) {
                input.unlockMouse();
                locked = false;
            }
            else {
                input.lockMouse();
                locked = true;
            }
        }
        
        graphicsManager.mainCamera.fovy -= 0.05f * input.getDeltaScroll();

        glm::vec3 rotate{ 0 };
        glm::vec2 deltaMouse = input.getDeltaMouse();
        rotate.x += deltaMouse.y;
        rotate.y += deltaMouse.x;

        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
        gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

        float yaw = gameObject.transform.rotation.y;
        const glm::vec3 forwardDir{ sin(yaw), 0.f, cos(yaw) };
        const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
        const glm::vec3 upDir{ 0.f, -1.f, 0.f };

        glm::vec3 moveDir{ 0.f };

        if (input.keyDown(GLFW_KEY_W) == Input::State::Down) moveDir += forwardDir;
        if (input.keyDown(GLFW_KEY_S) == Input::State::Down) moveDir -= forwardDir;
        if (input.keyDown(GLFW_KEY_D) == Input::State::Down) moveDir += rightDir;
        if (input.keyDown(GLFW_KEY_A) == Input::State::Down) moveDir -= rightDir;
        if (input.keyDown(GLFW_KEY_SPACE) == Input::State::Down) moveDir += upDir;
        if (input.keyDown(GLFW_KEY_LEFT_SHIFT) == Input::State::Down) moveDir -= upDir;

        if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
            gameObject.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }

        graphicsManager.mainCamera.setViewYXZ(gameObject.transform.translation, gameObject.transform.rotation);
    }


}