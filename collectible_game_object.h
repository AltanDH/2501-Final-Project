#ifndef COLLECTIBLE_GAME_OBJECT_H_
#define COLLECTIBLE_GAME_OBJECT_H_

#include <string>

#include "game_object.h"

namespace game {

	// Inherits from GameObject
	class CollectibleGameObject : public GameObject {

		public:
			CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			// Getters
			inline const std::string& GetSubType(void) const { return sub_type_; }

			// Overrided function inherited from GameObject for custom reaction to collisions
			void Collide(GameObject* object) override;

			// Overrided update function for custom behavior
			void Update(double delta_time) override;

		protected:
			// Identifier for the kind of collectible
			std::string sub_type_;

	}; // class CollectibleGameObject

} // namespace game

#endif // COLLECTIBLE_GAME_OBJECT_H_