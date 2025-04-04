#include "collectible_game_object.h"

namespace game {

	/*
		CollectibleGameObject inherits from GameObject
		It overrides GameObject's constructor to set a custom type
	*/

	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = "Collectible";
	}

	// Override the Collide method for custom collectible behavior
	void CollectibleGameObject::Collide(GameObject* object) {
		// Only allow collisions with Player
		if (object->GetType() == "Player") {
			// lower hitpoint which by default is 1 (inherited)
			hitpoints_--;
			if (hitpoints_ <= 0 && !is_destroyed_) {
				// set destroyed state to true to allow deletion
				is_destroyed_ = true;
				ghost_ = true;
			}
		}
	}

} // namespace game