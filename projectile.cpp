
#include "projectile.h"
#include "player_game_object.h"

namespace game {

	/*
		Projectile inherits from GameObject
		It overrides GameObject's constructor, update, and collide methods
	*/

	// Overrided the constructor for additional attribute initialization
	Projectile::Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, const glm::vec3& direction, GameObject* owner)
		: GameObject(position, geom, shader, texture) {
		
		hitpoints_ = 1;
		type_ = "Projectile";
		
		direction_ = direction;
		speed_ = 14;

		lifespan_ = Timer();
		lifespan_.Start(3.0f);

		owner_ = owner;
	}

	// Update function for moving the player object around
	void Projectile::Update(double delta_time) {
		// Check if projectile lifespan ended
		if (lifespan_.Finished() && !is_destroyed_) {
			// Destroy projectile
			hitpoints_ = 0;
			is_destroyed_ = true;
			
			// Let owner know a projectile has been destroyed (only relevant for the player)
			if (owner_->GetType() == "Player") {
				((PlayerGameObject*)owner_)->ProjectileDestroyed();
			}
		}

		// Modify projectile position into desired direction as long as it's not destroyed
		if (!is_destroyed_) {
			position_ += ((float)delta_time * speed_ * direction_);
		}
	}

	// Override the Collide method for custom projectile behavior
	void Projectile::Collide(GameObject* other) {
		// Make sure projectile isn't colliding with the entity that fired it
		if (other == owner_) {
			return;
		}

		// Check if a player or enemy collided with the projectile (thus collision valid)
		if ((other->GetType() == "Enemy" || other->GetType() == "Player") && !(other->isDestroyed())) {
			
			// Lower other object HP if it isn't invincible
			if (!other->isInvincible()) {
				other->SetHitpoints(other->GetHitpoints() - 1);
			}

			// Lower projectile hitpoints
			hitpoints_--;
			// Change state to 'destroyed' if necessary
			if (hitpoints_ <= 0 && !is_destroyed_) {
				is_destroyed_ = true;

				// Let owner know a projectile has been destroyed (only relevant for the player)
				if (owner_->GetType() == "Player") {
					((PlayerGameObject*)owner_)->ProjectileDestroyed();
				}
			}
		}

		// Mothership collision treated seperately in case of special changes for Boss interactions
		// All projectiles will hurt mothership (Friendly fire included)
		if (other->GetType() == "Mothership" && !(other->isDestroyed())) {
			// Lower mothership HP
			other->SetHitpoints(other->GetHitpoints() - 1);

			// Lower projectile hitpoints
			hitpoints_--;
			// Change state to 'destroyed' if necessary
			if (hitpoints_ <= 0 && !is_destroyed_) {
				is_destroyed_ = true;

				// Let owner know a projectile has been destroyed (only relevant for the player)
				if (owner_->GetType() == "Player") {
					((PlayerGameObject*)owner_)->ProjectileDestroyed();
				}
			}
		}
	}

} // namespace game
