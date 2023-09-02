#pragma once

#include <set>

#include "../Managers/Graphics/Camera.h"
#include "../GameObject/GameObject.h"

namespace gde { class GoldDayEngine; }
namespace gde::system {
	class Light {
	public:
		Light(GoldDayEngine& _engine);
		~Light();

		Light(const Light&) = delete;
		Light& operator=(const Light&) = delete;

		void updateLightData(const Camera& camera);
	private:
		GoldDayEngine& engine;

		std::set<Entity> mEntities;
	};
}