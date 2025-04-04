#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Inherits from GameObject
	class CollectibleGameObject : public GameObject {

		public:
			CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			// Overrided function inherited from GameObject for custom reaction to collisions
			void Collide(GameObject* object) override;

	}; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_