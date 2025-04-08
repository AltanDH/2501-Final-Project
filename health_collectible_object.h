#ifndef HEALTH_COLLECTIBLE_OBJECT_H_
#define HEALTH_COLLECTIBLE_OBJECT_H_

#include "collectible_game_object.h"

namespace game {

	// Health Collectibles heal the player a certain amount for every piece collected
	class HealthCollectibleObject : public CollectibleGameObject {

	public:
		HealthCollectibleObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

	}; // class HealthCollectibleObject

} // namespace game

#endif // HEALTH_COLLECTIBLE_OBJECT_H_