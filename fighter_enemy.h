#ifndef FIGHTER_ENEMY_H_
#define FIGHTER_ENEMY_H_

#include <string>

#include "game_object.h"
#include "enemy_game_object.h"
#include "player_game_object.h"
#include "projectile.h"

// Forward declare Mothership class (needed because of circular includes problems if not done)
namespace game {
	class Mothership;
}

namespace game {

	/*
		"Fighter" is a ranged enemy type that wanders until the player enters its range. Afterwards, it begins pursuit and fires
		projectiles at the player.
	*/
	class FighterEnemy : public EnemyGameObject {
		
		public:
			// Constructor
			FighterEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GLuint bullet_tex, PlayerGameObject* target, Mothership* mothership = nullptr);

			// Function that fires projectile and returns its pointer, or nullptr if none were fired
			void Fire();

			// Implement function inherited from EnemyGameObject <- GameObject
			inline void SetVelocity(const glm::vec3& velocity) override { velocity_ = velocity; }

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Target for enemy objects to pursue
			PlayerGameObject* target_;

			// Enemy state (can be either "wandering", "in pursuit", or "chase")
			std::string state_;

			// Desired direction vector for pursuit
			glm::vec3 direction_;

			// Velocity for movement physics
			glm::vec3 velocity_;

			// Enemy speed
			float speed_;

			// Timer for trajectory correction/target recalculation during "pursuit"
			Timer recalibration_interval_;

			// Timer for updating the "wandering" state target
			Timer wandering_update_;

			// Texture for projectiles
			GLuint bullet_texture_;

			// Timer to track projectile firing cooldown
			Timer shooting_cooldown_;
	
	}; // class FighterEnemy

} // namespace game

#endif // FIGHTER_ENEMY_H_