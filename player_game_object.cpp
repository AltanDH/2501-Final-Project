
#include "player_game_object.h"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's constructor, update, and collide methods
*/

// Overrided the constructor for additional attribute initialization
PlayerGameObject::PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture)
	: GameObject(position, geom, shader, texture) {
	
	max_hitpoints_ = 14;
	hitpoints_ = 14;
	type_ = "Player";

	invincibility_duration_ = Timer();
	shield_collectible_count_ = 0;
	fuel_ = 100;

	use_boost_ = false;
	fuel_consumption_ = Timer();
	
	projectile_cooldown_ = Timer();
	pulse_cooldown_ = Timer();
	projectile_count_ = 0;

	velocity_ = glm::vec3(0.005f, 0.005f, 0.0f);
	acceleration_ = 0.003;
	max_velocity_ = 8.0;
}

// Function that fires projectile and returns its pointer, or nullptr if none were fired
Projectile* PlayerGameObject::FireProjectile(GLuint texture) {
	// If firing cooldown hasn't ended, or too many projectiles exist, then there is nothing to return (since nothing was fired)
	if (!projectile_cooldown_.Finished() || projectile_count_ >= 3) {
		return nullptr;
	}

	// Create projectile to fire
	Projectile* projectile = new Projectile(position_, geometry_, shader_, texture, GetBearing(), this);
	// Rotate it to face the direction the player is looking in
	projectile->SetRotation(angle_);

	// Reset shooting cooldown
	projectile_cooldown_.Start(0.15f);
	// Increase amount of projectiles fired
	projectile_count_++;

	// Return resulting projectile pointer
	return projectile;
}

// Function that fires pulse and returns its pointer, or nullptr if none were fired
Pulse* PlayerGameObject::FirePulse(GLuint texture) {
	// If firing cooldown hasn't ended then there is nothing to return (since nothing was fired)
	if (!pulse_cooldown_.Finished()) {
		return nullptr;
	}

	// Create pulse to fire
	Pulse* pulse = new Pulse(position_, geometry_, shader_, texture, this);

	// Reset shooting cooldown
	pulse_cooldown_.Start(2.1f);

	// Return resulting projectile pointer
	return pulse;
}

// Override the Collide method for custom Player behavior
void PlayerGameObject::Collide(GameObject* other) {
	
	// Check if collided/picked up a collectible
	if (other->GetType() == "Collectible") {
		// If Shield picked up
		if (((CollectibleGameObject*)other)->GetSubType() == "Shield") {
			// Increase count for invincibility
			shield_collectible_count_++;
		}
		// If Health picked up
		else if (((CollectibleGameObject*)other)->GetSubType() == "Health") {
			// Heal
			hitpoints_ += 2;
			if (hitpoints_ > 14) {
				hitpoints_ = 14;
			}
		}
		// If Fuel picked up
		else if (((CollectibleGameObject*)other)->GetSubType() == "Fuel") {
			// Refill tank by a certain amount
			fuel_ += 25;
			if (fuel_ > 100) {
				fuel_ = 100;
			}
		}
	}
	// Otherwise deal with consequences of collision with enemy (as long as player is not invincible)
	else if ((other->GetType() == "Enemy" || other->GetType() == "Mothership") && !is_invincible_) {
		
		// Lower Mothership HP respecting the damage cooldown
		if (dmg_cooldown_.Finished()) {
			hitpoints_--;

			// Change state to 'destroyed' if necessary
			if (hitpoints_ <= 0 && !is_destroyed_) {
				is_destroyed_ = true;
				// Set timer for duration until object deletion
				timer_.Start(3.0f);
			}

			// Reset dmg cooldown
			dmg_cooldown_.Start(0.8f);
		}
	}
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {

	// Modify acceleration value based on whether boost is active
	if (!use_boost_ || fuel_ <= 0) {
		acceleration_ = 0.003;
	}
	else if (use_boost_ && fuel_ > 0) {
		acceleration_ = 0.012;
		if (fuel_consumption_.Finished()) {
			fuel_ -= 1.0;

			fuel_consumption_.Start(0.2f);
		}
	}

	// Special player updates go here
	// Check if player collected enough collectibles to trigger invincibility
	if (shield_collectible_count_ >= 5 && is_invincible_ == false) {
		shield_collectible_count_ -= 5;
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

} // namespace game
