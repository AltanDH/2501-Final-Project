#ifndef FUEL_COLLECTIBLE_OBJECT_H_
#define FUEL_COLLECTIBLE_OBJECT_H_

#include "collectible_game_object.h"

namespace game {

	// Fuel Collectibles refill the player's fuel tank by a certain amount for every piece collected (fuel is only used for nitro)
	class FuelCollectibleObject : public CollectibleGameObject {

	public:
		FuelCollectibleObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

	}; // class FuelCollectibleObject

} // namespace game

#endif // FUEL_COLLECTIBLE_OBJECT_H_