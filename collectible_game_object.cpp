#include "collectible_game_object.h"

namespace game {

	/*
		CollectibleGameObject inherits from GameObject
		It overrides GameObject's constructor to set a custom type
	*/

	CollectibleGameObject::CollectibleGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		
		type_ = "Collectible";
		sub_type_ = "Empty";
		// Start a timer for collectible lifespan
		timer_.Start(10.0f);
	}

	// Override the Collide method for custom collectible behavior
	void CollectibleGameObject::Collide(GameObject* object) {
		
		// Only allow collisions with Player
		if (object->GetType() == "Player") {
			// Destroy object once collected
			hitpoints_ = 0;
			if (hitpoints_ <= 0 && !is_destroyed_) {
				// Set destroyed state to true to allow deletion
				is_destroyed_ = true;
				ghost_ = true;
				timer_.Start(2.0f);
			}
		}
	}

	// Overrided update function for custom behavior
	void CollectibleGameObject::Update(double delta_time) {
		
		// Automatically destroy collectible if it's lifespan ended
		if (timer_.Finished() && !is_destroyed_) {
			hitpoints_ = 0;
			is_destroyed_ = true;
			ghost_ = true;
		}
	}

} // namespace game