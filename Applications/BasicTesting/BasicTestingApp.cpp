#include "EngineIncludes.h"

#include "BasicTestingApp.h"

#include <iostream>

#include "../EngineInterface/EngineInterface.h"
#include "../Engine/Managers/GameObjectManager/GameObjectManager.inl" // TODO: remove when interface is done

void BasicTestingApp::setup() {
	gde::GoldDayEngine& engine = iEngine.getEngine(); // TODO: change
	auto& metaObjectManager = engine.getMOM();
	auto& graphicsManager = engine.getGraphicsManager();


	metaObjectManager.registerComponent<gde::component::Transform>();
	metaObjectManager.registerComponent<gde::component::Render>();
	metaObjectManager.registerComponent<gde::component::PointLight>();

	metaObjectManager.registerSystem<gde::system::Renderer>();
	metaObjectManager.registerSystem<gde::system::LightRenderer>();
	metaObjectManager.registerSystem<gde::system::Light>();

	engine.getHIM().getInput().addTracking(6, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE);
	engine.getHIM().getInput().addTracking(3, GLFW_MOUSE_BUTTON_LEFT, GLFW_KEY_ESCAPE, GLFW_KEY_L);
	engine.getHIM().getInput().lockMouse();


	std::shared_ptr<gde::Model> model = gde::Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/flat_vase.obj");
	auto& flatVaseID = metaObjectManager.makeGameObject();

	metaObjectManager.addComponent<gde::component::Transform>(flatVaseID,
		gde::component::Transform{ glm::vec3(-.5f, .5f, 0.f), glm::vec3(0),1.5f });
	metaObjectManager.addComponent<gde::component::Render>(flatVaseID,
		gde::component::Render{ model });

	model = gde::Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/smooth_vase.obj");
	auto& smoothVase = metaObjectManager.makeGameObject();

	metaObjectManager.addComponent<gde::component::Transform>(smoothVase,
		gde::component::Transform{ glm::vec3(.5f, .5f, 0.f), glm::vec3(0),1.5f });
	metaObjectManager.addComponent<gde::component::Render>(smoothVase,
		gde::component::Render{ model });

	model = gde::Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/quad.obj");
	auto& floor = metaObjectManager.makeGameObject();

	metaObjectManager.addComponent<gde::component::Transform>(floor,
		gde::component::Transform{ glm::vec3(0.f, .5f, 0.f), glm::vec3(0), 3.f });
	metaObjectManager.addComponent<gde::component::Render>(floor,
		gde::component::Render{ model });


	model = gde::Model::createModelFromFile(graphicsManager.getVkInterface().getDevice(), "Applications/SharedModels/colored_cube.obj");
	auto& cube = metaObjectManager.makeGameObject();
	
	metaObjectManager.addComponent<gde::component::Transform>(cube,
		gde::component::Transform{ glm::vec3(0.f, -1.5f, 0.f), glm::vec3(0), 0.25f });
	metaObjectManager.addComponent<gde::component::Render>(cube,
		gde::component::Render{ model });
	
	std::vector<glm::vec3> lightColors{
		  {1.f, .1f, .1f},
		  {.1f, .1f, 1.f},
		  {.1f, 1.f, .1f},
		  {1.f, 1.f, .1f},
		  {.1f, 1.f, 1.f},
		  {1.f, 1.f, 1.f}
	};

	for (int i = 0; i < lightColors.size(); i++) {

		auto& pointLight = engine.getMOM().makeGameObject();

		engine.getMOM().addComponent<gde::component::PointLight>(pointLight, gde::component::PointLight{ 0.2,lightColors[i] });

		auto rotateLight = glm::rotate(
			glm::mat4(1.f),
			(i * glm::two_pi<float>()) / lightColors.size(),
			{ 0.f, -1.f, 0.f });

		glm::vec3 position = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));

		engine.getMOM().addComponent<gde::component::Transform>(pointLight,
			gde::component::Transform{ position, glm::vec3(0), 0.1f });
	}
};




void BasicTestingApp::update(float dt) {
	// TODO: this shouldn't work, still building interface
	static gde::GoldDayEngine& engine = iEngine.getEngine();
	static auto& metaObjectManager = engine.getMOM();
	static auto& humanInterfaceManager = engine.getHIM();
	static auto& graphicsManager = engine.getGraphicsManager();
	static gde::Input& input = humanInterfaceManager.getInput();

	static bool locked = true;
	static float moveSpeed{ 3.f };
	static float lookSpeed{ 30.0f };

	if (input.keyDown(GLFW_KEY_ESCAPE) == gde::Input::State::Enter) {
		engine.end();
	}

	gde::component::Transform& cameraTransform = engine.getMOM().getComponent<gde::component::Transform>(engine.cameraID);

	

	if (input.keyDown(GLFW_KEY_L) == gde::Input::State::Enter) engine.getDebugManager().setLightRendering(!engine.getDebugManager().isLightRenderingEnabled());


	if (humanInterfaceManager.getInput().keyDown(GLFW_MOUSE_BUTTON_LEFT) == gde::Input::State::Enter) {
		if (locked) {
			input.unlockMouse();
			locked = false;
		}
		else {
			input.lockMouse();
			locked = true;
		}
	}

	// TODO: encapsulate in camera component member changeFovy()
	graphicsManager.mainCamera.fovy -= 0.05f * input.getDeltaScroll(); 
	graphicsManager.setCameraAspectRatio(graphicsManager.getVkInterface().getAspectRatio());


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

	graphicsManager.mainCamera.setViewYXZ(cameraTransform.translation, cameraTransform.rotation);


	gde::Signature customSignature;
	customSignature.set(metaObjectManager.getComponentType<gde::component::Transform>());
	customSignature.set(metaObjectManager.getComponentType<gde::component::PointLight>());

	auto rotateLight = glm::rotate(glm::mat4(1.f), 0.5f * dt, { 0.f, -1.f, 0.f });
	int lightIndex = 0;

	gde::GOIDItr itr;
	gde::GOIDItr end;
	metaObjectManager.getRelevantGOIDs(customSignature, itr, end); // dangerous, should check that customSignature is tracked
	for (; itr != end; itr++) {
		gde::GOID id = *itr;

		assert(lightIndex < MAX_LIGHTS && "Point lights exceed maximum specified");

		// update light position
		gde::component::Transform& transform = metaObjectManager.getComponent<gde::component::Transform>(id);
		transform.translation = glm::vec3(rotateLight * glm::vec4(transform.translation, 1.f));
	}
}