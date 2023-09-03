#pragma once

#include "../EngineInterface/Application.h"



class BasicTestingApp : public Application{
public:
	BasicTestingApp(gde::EngineInterface& _iEngine):Application(_iEngine) {};

	std::string getType() const override { return typeid(BasicTestingApp).name(); };

	void setup() override;
	void update(float dt) override;
private:
};