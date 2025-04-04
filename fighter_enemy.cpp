#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "fighter_enemy.h"

namespace game {

	// Constructor
	FighterEnemy::FighterEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint bullet_tex, PlayerGameObject* target, GameObject* mothership)
		: EnemyGameObject(position, geom, shader, texture) {
		
		// Fighter default state
		state_ = "wandering";
		
		// Trackers for target pursuit
		target_ = target;
		direction_ = target->GetPosition() - position;
		recalibration_interval_ = Timer();

		// Timer for wandering update
		wandering_update_ = Timer();

		// Speed
		speed_ = 0.2f;
		velocity_ = glm::vec3(0);

		// Texture for the projectiles
		bullet_texture_ = bullet_tex;

		// Shooting timer initialization
		shooting_cooldown_ = Timer();
	}

	// Function that fires projectile and returns its pointer, or nullptr if none were fired
	Projectile* FighterEnemy::Fire() {
		// If firing cooldown hasn't ended there is nothing to return (since nothing was fired)
		if (!shooting_cooldown_.Finished()) {
			return nullptr;
		}

		// Create projectile to fire
		Projectile* projectile = new Projectile(position_, geometry_, shader_, bullet_texture_, GetBearing());
		// Rotate it to face the direction the player is looking in
		projectile->SetRotation(angle_);

		// Reset shooting cooldown
		shooting_cooldown_.Start(1.0f);

		// Return resulting projectile pointer
		return projectile;
	}

	// Override update method for custom behavior
	void FighterEnemy::Update(double delta_time) {
		
		// Check if enemy in pursuit
		if (state_ == "in pursuit" && !is_destroyed_) {
			// If close enough to target change state to chase
			float distance = glm::length(position_ - target_->GetPosition());
			if (distance <= 0.4) {
				state_ = "chase";
			}
			
			// Otherwise continune normal pursuit behavior
			else {
				// Get Target predicted (future) position
				glm::vec3 target = target_->GetPosition() + target_->GetVelocity();
				// Steering to target
				glm::vec3 steering = target - (position_ + velocity_);
				steering /= glm::length(steering);
				steering *= 0.1; // Adjust force magnitude
				// Update velocity
				velocity_ += steering;

				// Fire when possible (function handles logic)
				Fire();
			}
		}
		
		// Check if enemy is chasing target more directly
		if (state_ == "chase" && !is_destroyed_) {
			// Get Target precise position
			glm::vec3 target = target_->GetPosition();
			// Create valid Steering to target
			glm::vec3 steering = target - (position_ + velocity_);
			steering /= glm::length(steering);
			steering *= 0.1; // Adjust force magnitude
			// Update velocity
			velocity_ += steering;

			// Fire when possible (function handles logic)
			Fire();
		}
		
		// Check if enemy needs to wander
		else if (state_ == "wandering" && !is_destroyed_) {
			// Wander mechanic
			if (wandering_update_.Finished()) {
				// Random point in angle opening
				float r_num = ((float)rand()) / ((float)RAND_MAX);
				float opening = 5.0 * 3.141592 / 180.0; // Add PI from the glmlibrary
				float r_angle = r_num * 2.0 * opening + angle_ - opening;
				float r = 0.25;
				glm::vec3 target(r * cos(r_angle), r * sin(r_angle), 0.0);
				
				// Steering to target
				glm::vec3 desired = target;
				glm::vec3 steering = desired + velocity_;
				steering /= glm::length(steering);
				steering *= 0.1; // Adjust force magnitude
				// Update velocity
				velocity_ += steering;
				
				// Reset timer to only update wander every 1s
				wandering_update_.Start(1.0);
			}

			// If close enough to target change state to pursuit
			float distance = glm::length(position_ - target_->GetPosition());
			if (distance <= 2.0) {
				state_ = "in pursuit";
			}
		}
		
		// Apply new velocity as long as enemy not destroyed
		if (!is_destroyed_) {
			//glm::vec3 velocity = glm::normalize(direction_ * speed_);
			// Apply it to the current position
			position_ += velocity_ * (float)delta_time;

			// Calculate the angle the enemy should turn to face
			float target_angle = atan2(velocity_.y, velocity_.x);
			// Get the difference with current angle
			float angle_diff = target_angle - angle_;

			// Check if we'll need to rotate clockwise or counter-clockwise
			if (angle_diff > glm::pi<float>()) {
				// Ensure that the angle diff is within range [-pi, pi] for shortest rotation angle
				angle_diff -= 2.0 * glm::pi<float>();
			}
			else if (angle_diff < glm::pi<float>()) {
				angle_diff += 2.0 * glm::pi<float>();
			}

			// Clamp rotation angle so that it does't happen too quickly
			angle_ += glm::clamp(angle_diff, -1.5f * (float)delta_time, 1.5f * (float)delta_time);
		}

		/* Check if enemy is in pursuit
		else if (state_ == "in pursuit" && !is_destroyed_) {
			// Euler integration
			// Check if its time to recalculate desired/target direction
			if (recalibration_interval_.Finished()) {
				direction_ = target_->GetPosition() - position_;
				recalibration_interval_.Start(2.0f);
			}
			
			// Pursuit Mechanic
			// Calculate resulting velocity
			glm::vec3 velocity = glm::normalize(direction_ * speed_);
			// Apply it to the current position
			position_ += velocity * (float)delta_time;

			// Calculate the angle the enemy should turn to face
			float target_angle = atan2(direction_.y, direction_.x);
			// Get the difference with current angle
			float angle_diff = target_angle - angle_;

			// Check if we'll need to rotate clockwise or counter-clockwise
			if (angle_diff > glm::pi<float>()) {
				// Ensure that the angle diff is within range [-pi, pi] for shortest rotation angle
				angle_diff -= 2.0 * glm::pi<float>();
			}
			else if (angle_diff < glm::pi<float>()) {
				angle_diff += 2.0 * glm::pi<float>();
			}

			// Clamp rotation angle so that it does't happen too quickly
			angle_ += glm::clamp(angle_diff, -1.5f * (float)delta_time, 1.5f * (float)delta_time);

			// Fire when possible (function handles logic)
			Fire();
		}*/
	}

} // namespace game