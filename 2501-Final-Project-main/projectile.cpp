#include "projectile.h"

namespace game {

	/*
		Projectile inherits from GameObject
		It overrides GameObject's constructor, update, and collide methods
	*/

	// Overrided the constructor for additional attribute initialization
	Projectile::Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, const glm::vec3& direction)
		: GameObject(position, geom, shader, texture) {
		hitpoints_ = 1;
		type_ = "Projectile";
		lifespan_ = Timer();
		direction_ = direction;
		speed_ = 14;
		lifespan_.Start(3.0f);
	}

	// Update function for moving the player object around
	void Projectile::Update(double delta_time) {
		// Check if projectile lifespan ended
		if (lifespan_.Finished() && !is_destroyed_) {
			// Destroy projectile
			hitpoints_ = 0;
			is_destroyed_ = true;
		}

		// Modify projectile position into desired direction as long as it's not destroyed
		if (!is_destroyed_) {
			position_ += ((float)delta_time * speed_ * direction_);
		}
	}

	// Override the Collide method for custom projectile behavior
	void Projectile::Collide(GameObject* object) {
		// Only deal with consequences of collision with enemy object
		if (object->GetType() == "Enemy") {
			// Lower hitpoints
			hitpoints_--;
			// Change state to 'destroyed' if necessary
			if (hitpoints_ <= 0 && !is_destroyed_) {
				is_destroyed_ = true;
			}
		}
	}

} // namespace game
