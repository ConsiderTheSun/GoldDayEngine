#include "EngineIncludes.h"

#include "GoldDayEngine.h"

#include "Managers/GameObjectManager/GameObjectManager.h"
#include "Managers/GameObjectManager/GameObjectManager.inl"


namespace gde {

	GoldDayEngine::GoldDayEngine(std::string windowName, glm::vec2 windowDimentions) :
			debugManager(*this), 
			graphicsManager(*this, windowName, windowDimentions),
            humanInterfaceManager(*this, graphicsManager.getWindow()),
            metaObjectManager(*this) {

		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Instantiated");
        
        // TEMP (should be done in app layer through interface
        metaObjectManager.registerComponent<component::Transform>();
        metaObjectManager.registerComponent<component::Render>();
        metaObjectManager.registerComponent<component::PointLight>();

        Signature rendererSignature;
        rendererSignature.set(metaObjectManager.getComponentType<component::Transform>());
        rendererSignature.set(metaObjectManager.getComponentType<component::Render>());

        Signature lightRendererSignature;
        lightRendererSignature.set(metaObjectManager.getComponentType<component::Transform>());
        lightRendererSignature.set(metaObjectManager.getComponentType<component::PointLight>());

        Signature lightSignature;
        lightSignature.set(metaObjectManager.getComponentType<component::Transform>());
        lightSignature.set(metaObjectManager.getComponentType<component::PointLight>());

        metaObjectManager.registerSystem<system::Renderer>(rendererSignature);
        metaObjectManager.registerSystem<system::LightRenderer>(lightRendererSignature);
        metaObjectManager.registerSystem<system::Light>(lightSignature);

		
        humanInterfaceManager.getInput().addTracking(6, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
        humanInterfaceManager.getInput().addTracking(3, GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_ESCAPE, GLFW_KEY_L);
        humanInterfaceManager.getInput().lockMouse();

        loadGameObjectsTEMP();
    }

	GoldDayEngine::~GoldDayEngine(){
		debugManager.getLogger().log(Logger::Verbose, "GoldDayEngine Freed");

	}

	void GoldDayEngine::start() {
		debugManager.getLogger().log(Logger::Info, "GoldDayEngine Starting");

        // TODO: gross, will be fixed w/ camera component 
        auto& viewerObject = metaObjectManager.makeGameObject();

        metaObjectManager.addComponent<component::Transform>(viewerObject.getId(), 
            component::Transform{glm::vec3(0,0,-2.5), glm::vec3(0), 1.f});

        //auto viewerObject = GameObject(-1);
        //viewerObject.transform.translation.z = -2.5f;

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
        auto& flatVase = metaObjectManager.makeGameObject();

        metaObjectManager.addComponent<component::Transform>(flatVase.getId(), 
            component::Transform{ glm::vec3(-.5f, .5f, 0.f ), glm::vec3(0),1.5f });
        metaObjectManager.addComponent<component::Render>(flatVase.getId(),
            component::Render{ model });

        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/smooth_vase.obj");
        auto& smoothVase = metaObjectManager.makeGameObject();

        metaObjectManager.addComponent<component::Transform>(smoothVase.getId(),
            component::Transform{ glm::vec3(.5f, .5f, 0.f ), glm::vec3(0),1.5f });
        metaObjectManager.addComponent<component::Render>(smoothVase.getId(),
            component::Render{ model });

        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/quad.obj");
        auto& floor = metaObjectManager.makeGameObject();

        metaObjectManager.addComponent<component::Transform>(floor.getId(),
            component::Transform{ glm::vec3( 0.f, .5f, 0.f), glm::vec3(0), 3.f});
        metaObjectManager.addComponent<component::Render>(floor.getId(),
            component::Render{ model });

        model = Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/colored_cube.obj");
        auto& cube = metaObjectManager.makeGameObject();
        
        metaObjectManager.addComponent<component::Transform>(cube.getId(),
            component::Transform{ glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0), 0.25f });
        metaObjectManager.addComponent<component::Render>(cube.getId(),
            component::Render{ model });
        

        std::vector<glm::vec3> lightColors{
          {1.f, .1f, .1f},
          {.1f, .1f, 1.f},
          {.1f, 1.f, .1f},
          {1.f, 1.f, .1f},
          {.1f, 1.f, 1.f},
          {1.f, 1.f, 1.f} 
        };

        for (int i = 0; i < lightColors.size(); i++) {

            auto& pointLight = metaObjectManager.makeGameObject();

            metaObjectManager.addComponent<component::PointLight>(pointLight.getId(), component::PointLight{ 0.2,lightColors[i] });

            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
                { 0.f, -1.f, 0.f });

            glm::vec3 position = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
            
            metaObjectManager.addComponent<component::Transform>(pointLight.getId(),
                component::Transform{ position, glm::vec3(0), 0.1f });
        }
    }
    

    void GoldDayEngine::customControllerTEMP(float dt, GameObject& gameObject) {
        static bool locked = true;

        if (humanInterfaceManager.getInput().keyDown(GLFW_KEY_ESCAPE) == Input::State::Enter) {
            end();
        }

        component::Transform& cameraTransform = metaObjectManager.getComponent<component::Transform>(gameObject.getId());

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
                cameraTransform.rotation += lookSpeed * dt * glm::normalize(rotate);
            }
            cameraTransform.rotation.x = glm::clamp(cameraTransform.rotation.x, -1.5f, 1.5f);
            cameraTransform.rotation.y = glm::mod(cameraTransform.rotation.y, glm::two_pi<float>());
        }
        

        float yaw = cameraTransform.rotation.y;

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
            cameraTransform.translation += moveSpeed * dt * glm::normalize(moveDir);
        }

        graphicsManager.mainCamera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);

        Signature customSignature;
        customSignature.set(metaObjectManager.getComponentType<component::Transform>());
        customSignature.set(metaObjectManager.getComponentType<component::PointLight>());

        auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * dt, { 0.f, -1.f, 0.f });
        int lightIndex = 0;
        for (auto& kv : metaObjectManager.getGOM().gameObjects) {
            GOID id = kv.first;
            if ((metaObjectManager.getSignature(id) & customSignature) != customSignature) continue;

            assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

            // update light position
            component::Transform& transform = metaObjectManager.getComponent<component::Transform>(id);
            transform.translation = glm::vec3(rotateLight * glm::vec4(transform.translation, 1.f));
        }
    }


}