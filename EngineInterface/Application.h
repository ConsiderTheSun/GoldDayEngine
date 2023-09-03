#pragma once

#include <string>
#include <typeinfo>


namespace gde { class EngineInterface; }

class Application {
public:
	Application(gde::EngineInterface& _iEngine) : iEngine{ _iEngine } {};

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

	virtual std::string getType() const = 0;

	virtual void setup() {};
	virtual void update(float dt) {}; // TODO: replace with scripting

protected:
	gde::EngineInterface& iEngine;

};