#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "boomer_enemy.h"
#include "celestial_body.h"
#include "mothership_boss.h"

namespace game {

	// Constructor
	BoomerEnemy::BoomerEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* target, Mothership* mothership)
		: EnemyGameObject(position, geom, shader, texture, mothership) {
		
		// Trackers for target pursuit
		target_ = target;
		direction_ = target->GetPosition() - position;
		speed_ = 0.01f;
		inOrbit_ = false;
		velocity_ = direction_ * speed_;
	}

	// Override update method for custom movement behavior
	void BoomerEnemy::Update(double delta_time) {
		// Euler integration
		// Calculate resulting velocity
		// Apply it to the current position
		if (!is_destroyed_ && !inOrbit_) {
			direction_ = glm::normalize(target_->GetPosition() - position_);
			if (glm::length(velocity_) > 6) { velocity_ = glm::normalize(velocity_)*6.0f; }
			velocity_ += direction_ * speed_;
			position_ += velocity_ * (float)delta_time;

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

			// Also update position based on mothership (if it exists), to remain within boss area
			if (mothership_ != nullptr && !mothership_->isDestroyed()) {
				// Calculate resulting velocity from Mothership
				glm::vec3 velocity = glm::normalize(mothership_->GetDirection() * mothership_->GetSpeed());
				// Apply it to the current position so enemy keeps following mothership
				position_ += velocity * (float)delta_time;
			}
		}
	}

} // namespace game