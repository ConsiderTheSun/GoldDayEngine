#include "EngineIncludes.h"

#include "BasicTestingApp.h"

#include <iostream>

#include "../EngineInterface/EngineInterface.h"
#include "../Engine/Managers/GameObjectManager/GameObjectManager.inl" // TODO: remove when interface is done

void BasicTestingApp::setup() {
	auto& iGO = iEngine.getGOInterface();
	auto& iInput = iEngine.getInput();
	auto iTransform = iEngine.getComponentInterface<gde::TransformInterface>();
	auto iRender = iEngine.getComponentInterface<gde::RenderInterface>();
	auto iPointLight = iEngine.getComponentInterface<gde::PointLightInterface>();

	iEngine.registerComponent<gde::component::Transform>();
	iEngine.registerComponent<gde::component::Render>();
	iEngine.registerComponent<gde::component::PointLight>();
	iEngine.registerComponent<gde::component::Camera>();

	iEngine.registerSystem<gde::system::Renderer>();
	iEngine.registerSystem<gde::system::LightRenderer>();
	iEngine.registerSystem<gde::system::Light>();

	// TODO: remove
	std::cout << "iTransform: " << iTransform->getType() << std::endl;
	std::cout << "iRender: " << iRender->getType() << std::endl;
	std::cout << "iPointLight: " << iPointLight->getType() << std::endl;

	
	iInput.addTracking(6, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
	iInput.addTracking(3, GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_ESCAPE, GLFW_KEY_L);
	iInput.lockMouse();

	
	auto& flatVaseID = iGO.makeEmptyGameObject();
	iGO.addComponent<gde::component::Transform>(flatVaseID,
		iTransform->makeComponent(glm::vec3(-.5f, .5f, 0.f), glm::vec3(), 1.5f));
	iGO.addComponent<gde::component::Render>(flatVaseID,
		iRender->makeComponent("Applications/SharedModels/flat_vase.obj"));

	auto& smoothVase = iGO.makeEmptyGameObject();
	iGO.addComponent<gde::component::Transform>(smoothVase,
		iTransform->makeComponent(glm::vec3(.5f, .5f, 0.f), glm::vec3(), 1.5f));
	iGO.addComponent<gde::component::Render>(smoothVase,
		iRender->makeComponent("Applications/SharedModels/smooth_vase.obj"));
	
	auto& floor = iGO.makeEmptyGameObject();
	iGO.addComponent<gde::component::Transform>(floor,
		iTransform->makeComponent(glm::vec3(0.f, .5f, 0.f), glm::vec3(), 3.f));
	iGO.addComponent<gde::component::Render>(floor,
		iRender->makeComponent("Applications/SharedModels/quad.obj"));

	auto& cube = iGO.makeEmptyGameObject();
	iGO.addComponent<gde::component::Transform>(cube,
		iTransform->makeComponent(glm::vec3(0.f, -1.5f, 0.f), glm::vec3(), 0.25f));
	iGO.addComponent<gde::component::Render>(cube,
		iRender->makeComponent("Applications/SharedModels/colored_cube.obj"));
	
	std::vector<glm::vec3> lightColors{
		  {1.f, .1f, .1f},
		  {.1f, .1f, 1.f},
		  {.1f, 1.f, .1f},
		  {1.f, 1.f, .1f},
		  {.1f, 1.f, 1.f},
		  {1.f, 1.f, 1.f}
	};

	for (int i = 0; i < lightColors.size(); i++) {

		auto& pointLight = iGO.makeEmptyGameObject();

		iGO.addComponent<gde::component::PointLight>(pointLight, iPointLight->makeComponent(0.2, lightColors[i]));
		
		auto rotateLight = glm::rotate(
			glm::mat4(1.f),
			(i * glm::two_pi<float>()) / lightColors.size(),
			{ 0.f, -1.f, 0.f });

		glm::vec3 position = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
		
		iGO.addComponent<gde::component::Transform>(pointLight,
			iTransform->makeComponent(position, glm::vec3(0), 0.1f));
	}
};




void BasicTestingApp::update(float dt) {

	static auto& iGO = iEngine.getGOInterface();
	static gde::Input& input = iEngine.getInput();
	static auto iTransform = iEngine.getComponentInterface<gde::TransformInterface>();
	static auto iRender = iEngine.getComponentInterface<gde::RenderInterface>();
	static auto iPointLight = iEngine.getComponentInterface<gde::PointLightInterface>();
	static auto iCamera = iEngine.getComponentInterface<gde::CameraInterface>();
	static auto& iDebug = iEngine.getDebugger();	

	static bool locked = true;
	static float moveSpeed{ 3.f };
	static float lookSpeed{ 30.0f };

	if (input.keyDown(GLFW_KEY_ESCAPE) == gde::Input::State::Enter) {
		iEngine.end();
	}
	
	gde::component::Transform& cameraTransform = iGO.getComponent<gde::component::Transform>(iEngine.mainCamera());

	

	if (input.keyDown(GLFW_KEY_L) == gde::Input::State::Enter) 
		iDebug.setLightRendering(!iDebug.isLightRenderingEnabled());


	if (input.keyDown(GLFW_MOUSE_BUTTON_LEFT) == gde::Input::State::Enter) {
		if (locked) {
			input.unlockMouse();
			locked = false;
		}
		else {
			input.lockMouse();
			locked = true;
		}
	}

	float newFov = iCamera->getFovY() - 0.05f * input.getDeltaScroll();
	iCamera->setFovY(newFov);


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

	if (input.keyDown(GLFW_KEY_W) == gde::Input::State::Down) moveDir += forwardDir;
	if (input.keyDown(GLFW_KEY_S) == gde::Input::State::Down) moveDir -= forwardDir;
	if (input.keyDown(GLFW_KEY_D) == gde::Input::State::Down) moveDir += rightDir;
	if (input.keyDown(GLFW_KEY_A) == gde::Input::State::Down) moveDir -= rightDir;
	if (input.keyDown(GLFW_KEY_SPACE) == gde::Input::State::Down) moveDir += upDir;
	if (input.keyDown(GLFW_KEY_LEFT_SHIFT) == gde::Input::State::Down) moveDir -= upDir;

	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
		cameraTransform.translation += moveSpeed * dt * glm::normalize(moveDir);
	}

	iCamera->setViewYXZ(cameraTransform.translation, cameraTransform.rotation);
	

	auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * dt, { 0.f, -1.f, 0.f });
	int lightIndex = 0;

	gde::GOIDItr itr;
	gde::GOIDItr end;
	gde::Signature customSignature;
	iEngine.setSignature<gde::component::Transform>(customSignature);
	iEngine.setSignature<gde::component::PointLight>(customSignature);

	iGO.getRelevantGOIDs(customSignature, itr, end);// dangerous, should check that customSignature is tracked TODO: allow register custom signature

	for (; itr != end; itr++) {
		gde::GOID id = *itr;

		assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

		// update light position
		gde::component::Transform& transform = iGO.getComponent<gde::component::Transform>(id);
		transform.translation = glm::vec3(rotateLight * glm::vec4(transform.translation, 1.f));
	}
}