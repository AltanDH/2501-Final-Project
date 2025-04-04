#include "pulse.h"

namespace game {

	// Constructor
	Pulse::Pulse(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		type_ = "Pulse";
		max_scale_ = 2.0f;
		lifespan_ = Timer();
		lifespan_.Start(max_scale_);
	}

	// Update function for moving the expanding pulse
	void Pulse::Update(double delta_time) {
		// Check if pulse lifespan ended
		if (lifespan_.Finished() && !is_destroyed_) {
			// Destroy pulse
			hitpoints_ = 0;
			is_destroyed_ = true;
		}

		// Keep expanding pulse as long as lifespan hasn't ended
		if (!is_destroyed_) {
			// Expand scale depending on lifespan left
			SetScale(1 + max_scale_ - lifespan_.TimeLeft());
			
			// Increase radius to adjust to new hitbox (this formula was found when experimenting with the relation between ...
			// ...a circle's radius and its scale)
			radius_ = 0.4 + (0.4 * scale_.x); // Note that we keep an identical scale for x and y for pulses (so either value can be used)
			
			// Reposition because of scaling offset
		}
	}

	// Overrided collision for custom behavior
	void Pulse::Collide(GameObject* object) {
		// Make other object move away from pulse center if Enemy or Player
	}

} // namespace game
