#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "enemy_game_object.h"

namespace game {
	/*
		EnemyGameObject inherits from GameObject
		It overrides GameObject's constructor to set a custom type and additional attributes, as well as overriding the Update method
	*/
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* target)
		: GameObject(position, geom, shader, texture) {
		type_ = "Enemy";
		state_ = "patrolling";
		// characteristics of ellipse
		width_ = 1.95;
		height_ = 1.2;
		center_ = position;
		angular_movement_ = 0.0;
		
		// Trackers for target information
		target_ = target;
		direction_ = target->GetPosition() - position;
		recalibration_interval_ = Timer();
	}

	// Overrided Update function for moving the enemy object around
	void EnemyGameObject::Update(double delta_time) {
		// Change enemy movement pattern depending on state
		if (state_ == "patrolling" && !is_destroyed_) {
			// If close enough to target change state
			float distance = glm::length(position_ - target_->GetPosition());
			if (distance <= 2.4) {
				state_ = "intercepting";
			}
			// Otherwise keep patrolling
			else {
				angular_movement_ += 0.6 * delta_time;
				// Parametric equations for elliptical motion
				float x = width_ / 2 * cos(angular_movement_) + center_.x;
				float y = height_/ 2 * sin(angular_movement_) + center_.y;

				// Update position
				position_ = glm::vec3(x, y, position_.z);
				// Modify enemy object angle to synchronize with movement in ellipse
				angle_ += 0.6 * delta_time;
			}
		}
		// Only other case to consider
		else if (state_ == "intercepting" && !is_destroyed_) {
			// Euler integration
			// Check if its time to recalculate desired/target direction
			if (recalibration_interval_.Finished()) {
				direction_ = target_->GetPosition() - position_;
				recalibration_interval_.Start(2.0f);
			}
			// Apply the vector to our position with a modified speed (slowed down)
			position_ += direction_ * 0.2f * (float)delta_time;

			// Calculate the angle the enemy should turn to face
			float target_angle = atan2(direction_.y, direction_.x);
			// Get the difference
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
	}
}