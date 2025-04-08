#ifndef SHIELD_COLLECTIBLE_OBJECT_H_
#define SHIELD_COLLECTIBLE_OBJECT_H_

#include "collectible_game_object.h"

namespace game {

	// Shield Collectibles grant the player invincibility once a sufficient amount has been collected
	class ShieldCollectibleObject : public CollectibleGameObject {

		public:
			ShieldCollectibleObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

	}; // class ShieldCollectibleObject

} // namespace game

#endif // SHIELD_COLLECTIBLE_OBJECT_H_