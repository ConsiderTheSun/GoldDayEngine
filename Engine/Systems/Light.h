#pragma once

#include "../Managers/Graphics/Camera.h"
// std
#include <memory>
#include <vector>
#include <set>
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