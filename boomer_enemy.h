#ifndef BOOMER_ENEMY_H_
#define BOOMER_ENEMY_H_

#include "game_object.h"
#include "enemy_game_object.h"

// Forward declare Mothership class (needed because of circular includes problems if not done)
namespace game {
	class Mothership;
}

namespace game {

	// "Boomer" is a melee enemy type that will chase down the player with the goal of crashing into them.
	class BoomerEnemy : public EnemyGameObject {
		
		public:
			// Constructor
			BoomerEnemy(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* target, Mothership* mothership = nullptr);

			// Override inherited function from EnemyGameObject <- GameObject
			inline void SetVelocity(const glm::vec3& velocity) override { direction_ = velocity; }

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Target for boomer to pursue
			GameObject* target_;

			// Desired direction vector for pursuit
			glm::vec3 direction_;

			// Boomer speed
			float speed_;

			// Timer for trajectory correction/target recalculation during pursuit
			Timer recalibration_interval_;
	
	}; // class BoomerEnemy

} // namespace game

#endif // BOOMER_ENEMY_H_
