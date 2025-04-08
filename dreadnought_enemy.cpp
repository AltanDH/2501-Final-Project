
#include "dreadnought_enemy.h"
#include "mothership_boss.h"

namespace game {
	
	// Constructor
	DreadnoughtEnemy::DreadnoughtEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint pulse_tex, Mothership* mothership)
		: EnemyGameObject(position, geom, shader, texture, mothership) {
		
		// Characteristics of ellipse
		width_ = 4;
		height_ = 1.8;
		center_ = position;
		
		// Tracks progression along the ellipse
		angular_movement_ = 0.0;

		// Texture for the pulse waves
		pulse_texture_ = pulse_tex;

		// Shooting timer initialization
		shooting_cooldown_ = Timer();
	}

	// Function that fires pulse
	void DreadnoughtEnemy::Fire() {
		// If firing cooldown hasn't ended there is nothing to do (since nothing was fired)
		if (!shooting_cooldown_.Finished() || is_destroyed_) {
			return;
		}

		// Create pulse to fire
		Pulse* pulse = new Pulse(position_, geometry_, shader_, pulse_texture_, this);

		// Reset shooting cooldown
		shooting_cooldown_.Start(2.1f);

		// Adds resulting pulse pointer to game objects (for collision checks)
		game_objects_ref_->insert(game_objects_ref_->end() - 1, pulse);
	}

	// Override update method for custom behavior
	void DreadnoughtEnemy::Update(double delta_time) {
		
		// Don't do anything if entity destroyed
		if (is_destroyed_) {
			return;
		}

		// Update center of elliptic trajectory based on mothership (if it exists), to stay within boss area
		if (mothership_ != nullptr && !mothership_->isDestroyed()) {
			// Calculate resulting velocity from Mothership
			glm::vec3 velocity = glm::normalize(mothership_->GetDirection() * mothership_->GetSpeed());
			// Apply it to the current position so enemy keeps following mothership
			center_ += velocity * (float)delta_time;
		}

		// Progress along elliptic trajectory
		angular_movement_ += 2.5f * delta_time;

		// Parametric equations for elliptical motion
		float x = (width_ / 2.0f) * cos(angular_movement_) + center_.x;
		float y = (height_ / 2.0f) * sin(angular_movement_) + center_.y;

		// Update position
		position_ = glm::vec3(x, y, 0);

		// Fire pulse (function will check whether it's time to or not)
		Fire();
	}

} // namespace game