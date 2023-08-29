
#include "GoldDayEngine.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Managers/GameObjectManager/GameObjectManager.h"

namespace gde {

	GoldDayEngine::GoldDayEngine(std::string windowName, glm::vec2 windowDimentions) :
			debugManager(*this), 
			graphicsManager(*this, windowName, windowDimentions),
            humanInterfaceManager(*this, graphicsManager.getWindow()),
            gom(*this),
            gof(gom) {
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Instantiated");

        // TEMP
		loadGameObjectsTEMP();
        humanInterfaceManager.getInput().addTracking(6, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
        humanInterfaceManager.getInput().addTracking(3, GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_ESCAPE, GLFW_KEY_L);
        humanInterfaceManager.getInput().lockMouse();
    }

	GoldDayEngine::~GoldDayEngine(){
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Freed");

	}

	void GoldDayEngine::start() {
		debugManager.getLogger().log(Logger::Info, "GoldDayEngine Starting");

        // TODO: gross, will be fixed w/ camera component 
        auto viewerObject = GameObject(-1);
        viewerObject.transform.translation.z = -2.5f;

        // TODO: prob wanna move this function to the camera component class
        graphicsManager.setCameraAspectRatio(graphicsManager.getVkInterface().getAspectRatio());

        // TODO: remove this, have it set in application layer
        graphicsManager.mainCamera.setViewTarget(glm::vec3(-1.f, -2.f, -2.f), glm::vec3(0.f, 0.f, 2.5f));

        currentTime = std::chrono::high_resolution_clock::now();
		while (!graphicsManager.getWindow().shouldClose()) {
			graphicsManager.getWindow().pollEvents();

            float deltaTime = getDeltaTime();

            humanInterfaceManager.update();

            customControllerTEMP(deltaTime, viewerObject);

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


	void GoldDayEngine::loadGameObjectsTEMP() {

        std::shared_ptr<Model> model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/flat_vase.obj");
        auto& flatVase = gof.makeEmptyGameObject();
        flatVase.model = model;
        flatVase.transform.translation = { -.5f, .5f, 0.f };
        flatVase.transform.scale = 1.5f;


        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/smooth_vase.obj");
        auto& smoothVase = gof.makeEmptyGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = { .5f, .5f, 0.f };
        smoothVase.transform.scale = 1.5f;


        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/quad.obj");
        auto& floor = gof.makeEmptyGameObject();
        floor.model = model;
        floor.transform.translation = { 0.f, .5f, 0.f };
        floor.transform.scale = { 3.f };

        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/colored_cube.obj");
        auto& cube = gof.makeEmptyGameObject();
        cube.model = model;
        cube.transform.translation = { 0.f, -1.5f, 0.f };
        cube.transform.scale = { 0.25f };
        

        std::vector<glm::vec3> lightColors{
          {1.f, .1f, .1f},
          {.1f, .1f, 1.f},
          {.1f, 1.f, .1f},
          {1.f, 1.f, .1f},
          {.1f, 1.f, 1.f},
          {1.f, 1.f, 1.f} 
        };

        for (int i = 0; i < lightColors.size(); i++) {

            auto& pointLight = gof.makeEmptyGameObject();
            pointLight.color = glm::vec3(1.f);
            pointLight.transform.scale = 0.1f;
            pointLight.pointLight = std::make_unique<PointLightComponent>();
            pointLight.pointLight->lightIntensity = 0.2f;

            componentManager.addComponent(pointLight.getId(), component::PointLight{ 0.2 });

            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { 0.f, -1.f, 0.f });
            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
        }
	}
    

    void GoldDayEngine::customControllerTEMP(float dt, GameObject& gameObject) {
        static bool locked = true;

        if (humanInterfaceManager.getInput().keyDown(GLFW_KEY_ESCAPE) == Input::State::Enter) {
            end();
        }



        Input& input = humanInterfaceManager.getInput();

        if (input.keyDown(GLFW_KEY_L) == Input::State::Enter) debugManager.setLightRendering(!debugManager.isLightRenderingEnabled());


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
        if (locked) {
            if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
                gameObject.transform.rotation += lookSpeed * dt * glm::normalize(rotate);
            }
            gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
            gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());
        }
        

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


        auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * dt, { 0.f, -1.f, 0.f });
        int lightIndex = 0;
        for (auto& kv : gom.gameObjects) {
            auto& obj = kv.second;
            if (obj.pointLight == nullptr) continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            obj.transform.translation = glm::vec3(rotateLight * glm::vec4(obj.transform.translation, 1.f));

        }
    }


}