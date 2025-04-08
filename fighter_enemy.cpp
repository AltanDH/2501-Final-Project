#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "fighter_enemy.h"
#include "mothership_boss.h"

namespace game {

	// Constructor
	FighterEnemy::FighterEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint bullet_tex, PlayerGameObject* target, Mothership* mothership)
		: EnemyGameObject(position, geom, shader, texture, mothership) {
		
		// Fighter default state
		state_ = "wandering";
		
		// Trackers for target pursuit
		target_ = target;
		direction_ = target->GetPosition() - position;

		// Timer for wandering update
		wandering_update_ = Timer();

		// Speed
		speed_ = 1.5f;
		velocity_ = glm::vec3(0);

		// Texture for the projectiles
		bullet_texture_ = bullet_tex;

		// Shooting timer initialization
		shooting_cooldown_ = Timer();
	}

	// Function that fires projectile
	void FighterEnemy::Fire() {
		// If firing cooldown hasn't ended there is nothing to do (since nothing was fired)
		if (!shooting_cooldown_.Finished()) {
			return;
		}

		// Create projectile to fire
		Projectile* projectile = new Projectile(position_, geometry_, shader_, bullet_texture_, GetBearing(), this);
		// Rotate it to face the direction the player is looking in
		projectile->SetRotation(angle_);

		// Reset shooting cooldown
		shooting_cooldown_.Start(2.4f);

		// Adds resulting projectile pointer to game objects (for collision checks)
		game_objects_ref_->insert(game_objects_ref_->end() - 1, projectile);
	}

	// Override update method for custom behavior
	void FighterEnemy::Update(double delta_time) {
		
		// Do nothing if destroyed
		if (is_destroyed_) {
			return;
		}

		// Calculate distance to player
		float distance_to_target = glm::length(position_ - target_->GetPosition());

		// Handle state transitions
		if (state_ == "wandering" && distance_to_target <= 8.0f) {
			state_ = "in pursuit";
			speed_ = 2.5f;
		}
		else if (state_ == "in pursuit" && distance_to_target <= 2.0f) {
			state_ = "chase";
			speed_ = 3.0f;
		}

		// Declare variable for the position we want to reach
		glm::vec3 target_position;

		// State machine logic
		// Wandering mechanic (selecting target)
		if (state_ == "wandering") {
			if (wandering_update_.Finished()) {
				// Get a random angle in opening
				float random = ((float)rand()) / ((float)RAND_MAX);
				float opening = glm::radians(45.0f);
				float random_angle = random * 2.0f * opening + angle_ - opening;
				float radius = 0.3f;
				// Create a vector to it
				direction_ = glm::vec3(radius * cos(random_angle), radius * sin(random_angle), 0.0f);

				// Reset wandering update
				wandering_update_.Start(1.0f);
			}
			// Create resulting target position
			target_position = position_ + direction_;
		}
		// Pursuit mechanic (selecting target)
		else if (state_ == "in pursuit") {
			target_position = target_->GetPosition() + target_->GetVelocity();
		} 
		// Chase mechanic (selecting target)
		else if (state_ == "chase") {
			target_position = target_->GetPosition();
		}

		// Calculate resulting velocity
		glm::vec3 desired_velocity = target_position - position_;
		if (glm::length(desired_velocity) > 0.0f) {
			desired_velocity = glm::normalize(desired_velocity) * speed_;
			// Smoothly interpolate to desired velocity
			velocity_ = glm::mix(velocity_, desired_velocity, 0.05f); 
		}
				
		// Rotate towards movement direction if necessary (ignore tiny rotations)
		if (glm::length(velocity_) > 0.01f) {
			// Determine desired angle based on velocity
			float target_angle = atan2(velocity_.y, velocity_.x);

			// Calculate the smallest difference between angles (-pi to pi)
			float delta_angle = glm::mod(target_angle - angle_ + glm::pi<float>(), (2*glm::pi<float>())) - glm::pi<float>();

			// Rotation speed in radians
			float rotation_speed = 2.0f;

			// Clamp rotation to turn speed
			delta_angle = glm::clamp(delta_angle, -rotation_speed * (float)delta_time, rotation_speed * (float)delta_time);

			// Apply rotation
			angle_ += delta_angle;
		}

		// Attempt to fire at player
		if (state_ != "wandering") {
			Fire();
		}

		// Apply new velocity
		position_ += velocity_ * (float)delta_time;

		// Also update position based on mothership (if it exists), to remain within boss zone in all cases
		if (mothership_ != nullptr && !mothership_->isDestroyed()) {
			// Calculate resulting velocity from Mothership
			glm::vec3 velocity = glm::normalize(mothership_->GetDirection() * mothership_->GetSpeed());
			// Apply it to the current position so enemy keeps following mothership
			position_ += velocity * (float)delta_time;
		}
	}

} // namespace game