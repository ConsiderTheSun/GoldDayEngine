#pragma once
#include "../GoldDayEngine.h"

namespace gde {
	class GameObjectInterface {
	public:
		GameObjectInterface(GoldDayEngine& _engine);

		GameObjectInterface(const GameObjectInterface&) = delete;
		GameObjectInterface& operator=(const GameObjectInterface&) = delete;

		const GOID& makeEmptyGameObject();
		//const GOID& makeGameObjectFromArchetype(const std::string& archetypeName); // TODO: make this

		void destroyGameObject(GOID id); //TODO: implement

		template<typename T>
		inline void addComponent(GOID goID, const T& component);


		void getRelevantGOIDs(Signature signature, GOIDItr& begin, GOIDItr& end) {
			return engine.getMOM().getRelevantGOIDs(signature, begin, end);
		}

	private:
		GoldDayEngine& engine;
	};
	typedef GameObjectInterface GOInterface;
}