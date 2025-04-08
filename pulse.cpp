
#include "pulse.h"

namespace game {

	// Constructor
	Pulse::Pulse(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* owner)
		: GameObject(position, geom, shader, texture) {
		
		type_ = "Pulse";
		
		max_scale_ = 2.0f;
		lifespan_ = Timer();
		lifespan_.Start(max_scale_);

		owner_ = owner;
	}

	// Update function for moving the expanding pulse
	void Pulse::Update(double delta_time) {
		// Check if pulse lifespan ended or owner died
		if (lifespan_.Finished() && !is_destroyed_ || owner_->isDestroyed()) {
			// Destroy pulse
			hitpoints_ = 0;
			is_destroyed_ = true;
		}

		// Keep expanding pulse as long as lifespan hasn't ended
		if (!is_destroyed_) {
			// Expand scale depending on lifespan left
			SetScale(1.0f + max_scale_ - lifespan_.TimeLeft());
			
			// Increase radius to adjust to new hitbox (this formula was found when experimenting with the relation between ...
			// ...a circle's radius and its scale)
			radius_ = 0.4 + (0.4 * scale_.x); // Note that we keep an identical scale for x and y for pulses (so either value can be used)
			
			// Reposition for a cooler pulsing effect that follows entity that launched it
			position_ = owner_->GetPosition();
		}
	}

	// Overrided collision for custom behavior
	void Pulse::Collide(GameObject* other) {
		// Make sure pulse isn't colliding with the entity that fired it
		if (other == owner_) {
			return;
		}

		// Check if a player or enemy collided with the pulse (thus collision valid)
		if ((other->GetType() == "Enemy" || other->GetType() == "Player") && !(other->isDestroyed())) {
			
			// Impact the other object if it isn't invincible and doesn't have a cooldown on taking damage
			if (!other->isInvincible() && other->GetDmgCooldown().Finished()) {
				// Lower other object HP
				other->SetHitpoints(other->GetHitpoints() - 1);

				// Make other object move away from pulse center (push them away)
				glm::vec3 away_from_center = glm::normalize(other->GetPosition() - position_);
				other->SetVelocity(away_from_center * 4.0f);

				// Reset damage cooldown
				other->GetDmgCooldown().Start(0.6f);
			}
		}

		// Pulses will only hurt mothership if fired by player (though there won't be knockback)
		if (other->GetType() == "Mothership" && owner_->GetType() == "Player" && !(other->isDestroyed())) {
			// Lower mothership HP while respecting the damage cooldown
			if (other->GetDmgCooldown().Finished()) {
				other->SetHitpoints(other->GetHitpoints() - 1);

				other->GetDmgCooldown().Start(0.6f);
			}
		}
	}

} // namespace game
