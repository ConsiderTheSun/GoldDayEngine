#pragma once

#include <array>
namespace gde {

	class AbstractComponentArray
	{
	public:
		virtual ~AbstractComponentArray() = default;
		//virtual void EntityDestroyed(Entity entity) = 0;
	};

template<typename T>
class ComponentArray : public AbstractComponentArray
{
public:
	void InsertData(GOID goID, const T& component)
	{
		assert(mGOIDToIndexMap.find(goID) == mGOIDToIndexMap.end() && "Component added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t newIndex = mSize;
		mGOIDToIndexMap[goID] = newIndex;
		mIndexToGOIDMap[newIndex] = goID;
		componentArray[newIndex] = component;
		++mSize;
	}

	void RemoveData(GOID goID)
	{
		assert(mGOIDToIndexMap.find(goID) != mGOIDToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedGOID = mGOIDToIndexMap[goID];
		size_t indexOfLastElement = mSize - 1;
		componentArray[indexOfRemovedGOID] = componentArray[indexOfLastElement];

		// Update map to point to moved spot
		GOID entityOfLastElement = mIndexToGOIDMap[indexOfLastElement];
		mGOIDToIndexMap[entityOfLastElement] = indexOfRemovedGOID;
		mIndexToGOIDMap[indexOfRemovedGOID] = entityOfLastElement;

		mGOIDToIndexMap.erase(goID);
		mIndexToGOIDMap.erase(indexOfLastElement);

		--mSize;
	}

	T& GetData(GOID goID)
	{
		assert(mGOIDToIndexMap.find(goID) != mGOIDToIndexMap.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return componentArray[mGOIDToIndexMap[goID]];
	}

private:
	std::array<T, MAX_ENTITIES> componentArray{}; // TODO: make into a vector to remove MAX_ENTITIES limit

	// Map from an entity ID to an array index.
	std::unordered_map<GOID, size_t> mGOIDToIndexMap;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, GOID> mIndexToGOIDMap;

	// Total size of valid entries in the array.
	size_t mSize;
};

}