#ifndef DREADNOUGHT_ENEMY_H_
#define DREADNOUGHT_ENEMY_H_

#include "game_object.h"
#include "enemy_game_object.h"
#include "pulse.h"

namespace game {

	/*
		Dreadnought is a ranged enemy type that moves in an elliptic formation whilst pulsating waves damaging the player and pushing
		them away.
	*/
	class DreadnoughtEnemy : public EnemyGameObject {
		
		public:
			// Constructor
			DreadnoughtEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint pulse_tex, GameObject* mothership = nullptr);

			// Function that fires projectile and returns its pointer, or nullptr if none were fired
			void Fire();

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Values representing ellipse for the creation of an elliptical trajectory
			float width_;
			float height_;
			glm::vec3 center_;

			// Tracks progression along the ellipse
			float angular_movement_;

			// Texture for the pulse waves
			GLuint pulse_texture_;

			// Timer to track pulse firing cooldown
			Timer shooting_cooldown_;
	
	}; // class DreadnoughtEnemy

} // namespace game

#endif // DREADNOUGHT_ENEMY_H_