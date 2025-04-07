#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "boomer_enemy.h"
#include "mothership_boss.h"

namespace game {

	// Constructor
	BoomerEnemy::BoomerEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* target, Mothership* mothership)
		: EnemyGameObject(position, geom, shader, texture, mothership) {
		
		// Trackers for target pursuit
		target_ = target;
		direction_ = target->GetPosition() - position;
		recalibration_interval_ = Timer();
		speed_ = 0.5f;
	}

	// Override update method for custom movement behavior
	void BoomerEnemy::Update(double delta_time) {
		// Euler integration
		// Check if its time to recalculate desired/target direction
		if (recalibration_interval_.Finished()) {
			direction_ = target_->GetPosition() - position_;
			recalibration_interval_.Start(2.0f);
		}

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
	}

} // namespace game