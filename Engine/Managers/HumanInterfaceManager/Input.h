#pragma once

#include <unordered_map>
#include <cstdarg>

#include "../Graphics/Window.h"

namespace gde::manager { class HumanInterfaceManager; }
namespace gde {

	class Input {
	public:
		enum State { Enter, Down, Release, Up };
		struct ButtonState {
			bool isDown;
			bool prevIsDown;
			State currentState;
		};


		Input(Window& _window);

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		State keyDown(int key) const;

		void addTracking(int count ...);

		void lockMouse();
		void unlockMouse();

		void hideMouse();
		void showMouse();

		const glm::vec2 getDeltaMouse() const { return deltaMouse; }
		float getDeltaScroll() const { return deltaScroll; };

		glm::vec2 getMousePos() { return mousePosition; }

		void reset();
	private:

		void update();

		Window& window;

		glm::vec2 mousePosition;
		glm::vec2 deltaMouse{ 0,0 };

		bool scrollChange = false;
		float deltaScroll = 0;

		std::unordered_map<int, ButtonState>  buttonTracker;

		friend class manager::HumanInterfaceManager;
	};
}