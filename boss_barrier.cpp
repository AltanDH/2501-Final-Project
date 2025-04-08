
#include "boss_barrier.h"
#include "mothership_boss.h"

namespace game {

	// Constructor
	BossBarrier::BossBarrier(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Mothership* mothership)
		: GameObject(position, geom, shader, texture) {

		type_ = "Barrier";
		mothership_ = mothership;
		dmg_cooldown_ = Timer();
	}

	// Override collide for custom behavior
	void BossBarrier::Collide(GameObject* other) {

		// Check if a player or enemy collided with the barrier (thus first collision check valid)
		if ((other->GetType() == "Enemy" || other->GetType() == "Player") && !(other->isDestroyed())) {

			// Perform circle on wall collision check to be sure
			glm::vec3 direction = GetBearing();
			glm::vec3 wall_to_obj = other->GetPosition() - position_;
			glm::vec3 proj_to_wall = position_ + direction * glm::dot(wall_to_obj, direction);

			// Confirm collision
			if (glm::length(other->GetPosition() - proj_to_wall) <= other->GetRadius() + 0.4) {
				// Lower other object HP respecting the damage cooldown if it isn't invincible
				if (dmg_cooldown_.Finished() && !(other->isInvincible())) {
					other->SetHitpoints(other->GetHitpoints() - 1);
					dmg_cooldown_.Start(1.2f);
				}

				// Make other object move away from barrier (push them away)
				glm::vec3 away_from_wall = glm::normalize(other->GetPosition() - proj_to_wall);
				other->SetVelocity(away_from_wall * 4.0f);
			}
		}
	}


	// Override Update method for custom behavior
	void BossBarrier::Update(double delta_time) {
		
		if (mothership_ != nullptr && !mothership_->isDestroyed()) {
			// Calculate resulting velocity from Mothership
			glm::vec3 velocity = glm::normalize(mothership_->GetDirection() * mothership_->GetSpeed());
			// Apply it to the current position so Barriers keep following mothership
			position_ += velocity * (float)delta_time;
		}
	}

} // namespace game