#include "EngineIncludes.h"
#include "Input.h"

namespace gde{
	Input::Input(Window& _window) : window(_window) {
		mousePosition = window.getCursorPos();
	}

	void Input::update() {

		// updates the mouse tracker
		glm::vec2 newMousePos = window.getCursorPos();
		deltaMouse.x = newMousePos.x - mousePosition.x;
		deltaMouse.y = mousePosition.y - newMousePos.y;
		mousePosition = newMousePos;

		auto scrollData = window.getScrollData();
		
		deltaScroll = scrollData.second;
		if (scrollData.first) {
			window.resetScrollData();
		}

		// updates the keys
		for (auto itr = buttonTracker.begin(); itr != buttonTracker.end(); ++itr) {
			
			itr->second.prevIsDown = itr->second.isDown;
			itr->second.isDown = window.getButton(itr->first);

			if (itr->second.isDown && !itr->second.prevIsDown) {
				itr->second.currentState = State::Enter;
			}
			else if (itr->second.isDown && itr->second.prevIsDown) {
				itr->second.currentState = State::Down;
			}
			else if (!itr->second.isDown && itr->second.prevIsDown) {
				itr->second.currentState = State::Release;
			}
			else if (!itr->second.isDown && !itr->second.prevIsDown) {
				itr->second.currentState = State::Up;
			}
		}
	}

	Input::State Input::keyDown(int key) const{
		auto button = buttonTracker.find(key);
		if (button == buttonTracker.end()) return State::Up;
		return button->second.currentState;
	}

	void Input::addTracking(int count ...) {
		va_list list;
		va_start(list, count);

		for (int i = 0; i < count; i++) {
			int key = va_arg(list, int);

			// checks if the key is already being tracked
			if (buttonTracker.find(key) != buttonTracker.end()) {
				return;
			}

			buttonTracker.insert(
				{ key,
				ButtonState{false,false, State::Up} });
		}
		va_end(list);
	}

	void Input::lockMouse() {
		window.setMouseMode(Window::MouseMode::LOCKED);

		mousePosition = window.getCursorPos();
	}

	void Input::unlockMouse() {
		window.setMouseMode(Window::MouseMode::NORMAL);
	}

	void Input::hideMouse() { window.setMouseMode(Window::MouseMode::HIDDEN); }
	void Input::showMouse() { window.setMouseMode(Window::MouseMode::NORMAL); }

	void Input::reset() {
		buttonTracker.clear();
		unlockMouse();
	}

}