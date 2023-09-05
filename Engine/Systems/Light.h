#pragma once

#include <set>
#include "System.h"
#include "../GameObject/GameObject.h"

namespace gde { class GoldDayEngine; }
namespace gde::system {
	class Light : public System {
	public:
		Light(GoldDayEngine& _engine);
		~Light();
		std::string getType() const override { return typeid(Light).name(); };

		Light(const Light&) = delete;
		Light& operator=(const Light&) = delete;

		void setSignature() override;

		void updateLightData(GlobalUbo& ubo);
	private:

		std::set<GOID> mEntities;
	};
}