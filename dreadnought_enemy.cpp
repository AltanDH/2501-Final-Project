#include "dreadnought_enemy.h"

namespace game {
	
	// Constructor
	DreadnoughtEnemy::DreadnoughtEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint pulse_tex, GameObject* mothership)
		: EnemyGameObject(position, geom, shader, texture, mothership) {
		
		// Characteristics of ellipse
		width_ = 1.95;
		height_ = 1.2;
		center_ = position;
		
		// Tracks progression along the ellipse
		angular_movement_ = 0.0;

		// Texture for the pulse waves
		pulse_texture_ = pulse_tex;

		// Shooting timer initialization
		shooting_cooldown_ = Timer();
	}

	// Function that fires pulse and returns its pointer, or nullptr if none were fired
	Pulse* DreadnoughtEnemy::Fire() {
		// If firing cooldown hasn't ended there is nothing to return (since nothing was fired)
		if (!shooting_cooldown_.Finished()) {
			return nullptr;
		}

		// Create pulse to fire
		Pulse* pulse = new Pulse(position_, geometry_, shader_, pulse_texture_);

		// Reset shooting cooldown
		shooting_cooldown_.Start(1.0f);

		// Return resulting pulse pointer
		return pulse;
	}

	// Override update method for custom behavior
	void DreadnoughtEnemy::Update(double delta_time) {
		angular_movement_ += 0.6 * delta_time;
		// Parametric equations for elliptical motion
		float x = width_ / 2 * cos(angular_movement_) + center_.x;
		float y = height_ / 2 * sin(angular_movement_) + center_.y;

		// Update position
		position_ = glm::vec3(x, y, 0);

		// Fire pulse (function will check whether it's time to or not)
		Fire();
	}

} // namespace game