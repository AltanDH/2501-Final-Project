#include "player_game_object.h"
#include <iostream>

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's constructor, update, and collide methods
*/

// Overrided the constructor for additional attribute initialization
PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
	hitpoints_ = 3;
	type_ = "Player";
	collectible_count_ = 0;
	is_invincible_ = false;
	invincibility_duration_ = Timer();
	shooting_cooldown_ = Timer();
	velocity_ = glm::vec3(0.005f, 0.005f, 0.0f);
	acceleration_ = 0.003;
	max_velocity_ = 8.0;
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	std::cout << "Player pos: " << this->GetPosition().x << ", " << this->GetPosition().y << std::endl;

	// Special player updates go here
	// Check if player collected enough collectibles to trigger invincibility
	if (collectible_count_ >= 5 && is_invincible_ == false) {
		collectible_count_ -= 5;
		is_invincible_ = true;
		invincibility_duration_.Start(10.0f);
	}

	// Make sure to revert to normal when invinvibility duration is over
	if (invincibility_duration_.Finished()) {
		is_invincible_ = false;
	}

	// Check if velocity exceeds its maximum (by getting its length)
	if (glm::length(velocity_) >= max_velocity_) {
		// Scale it down to max_velocity
		velocity_ = glm::normalize(velocity_) * max_velocity_;
	}

	// Stop all player movement when destroyed
	if (is_destroyed_) {
		velocity_ = glm::vec3(0.0f, 0.0f, 0.0f);
		acceleration_ = 0.0;
	}

	// Modify player pos using velocity
	position_ += (velocity_ * (float)delta_time);
}

// Override the Collide method for custom Player behavior
void PlayerGameObject::Collide(GameObject* object) {
	// Check if collided/picked up a collectible
	if (object->GetType() == "Collectible") {
		collectible_count_++;
	}
	// Otherwise deal with consequences of collision with enemy (as long as player is not invincible)
	else if (object->GetType() == "Enemy" && !is_invincible_) {
		// lower hitpoints
		hitpoints_--;
		// change state to 'destroyed' if necessary
		if (hitpoints_ <= 0 && !is_destroyed_) {
			is_destroyed_ = true;
			// set timer for duration until object deletion
			timer_.Start(5.0f);
		}
	}
}

// Function that fires projectile and returns its pointer, or nullptr if none were fired
Projectile* PlayerGameObject::Fire(GLuint texture) {
	// If firing cooldown hasn't ended there is nothing to return (since nothing was fired)
	if (!shooting_cooldown_.Finished()) {
		return nullptr;
	}

	// Create projectile to fire
	Projectile* projectile = new Projectile(position_, geometry_, shader_, texture, GetBearing(), this);
	// Rotate it to face the direction the player is looking in
	projectile->SetRotation(angle_);

	// Reset shooting cooldown
	shooting_cooldown_.Start(1.0f);

	// Return resulting projectile pointer
	return projectile;
}

} // namespace game
