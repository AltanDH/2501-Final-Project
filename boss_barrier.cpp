
#include "boss_barrier.h"
#include "mothership_boss.h"

namespace game {

	// Constructor
	BossBarrier::BossBarrier(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Mothership* mothership)
		: GameObject(position, geom, shader, texture) {

		type_ = "Barrier";
		mothership_ = mothership;
	}

	// Override collide for custom behavior
	void BossBarrier::Collide(GameObject* object) {

		// Check if a player or enemy collided with the barrier (thus first collision valid)
		if ((object->GetType() == "Enemy" || object->GetType() == "Player") && !(object->isDestroyed())) {

			// Perform circle on wall collision check to be sure
			glm::vec3 direction = GetBearing();
			glm::vec3 proj_to_wall = direction * glm::dot(object->GetPosition() - position_, direction);

			// Confirm collision
			if (glm::length(object->GetPosition() - proj_to_wall) <= object->GetRadius()) {
				// Lower other object HP
				object->SetHitpoints(object->GetHitpoints() - 1);

				// Make other object move away from barrier (push them away)
				glm::vec3 away_from_wall = glm::normalize(object->GetPosition() - proj_to_wall);
				object->SetVelocity(away_from_wall * 2.0f);
			}
		}
	}

	// Override Update method for custom behavior
	void BossBarrier::Update(double delta_time) {
		
		// Calculate resulting velocity from Mothership
		glm::vec3 velocity = glm::normalize(mothership_->GetDirection() * mothership_->GetSpeed());
		// Apply it to the current position so Barriers keep following mothership
		position_ += velocity * (float)delta_time;
	}

} // namespace game