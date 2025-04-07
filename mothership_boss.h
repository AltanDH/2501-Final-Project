#ifndef MOTHERSHIP_BOSS_H_
#define MOTHERSHIP_BOSS_H_

#include <vector>

#include "game_object.h"
#include "boss_barrier.h"
#include "enemy_game_object.h"
#include "boomer_enemy.h"
#include "dreadnought_enemy.h"
#include "fighter_enemy.h"
#include "timer.h"

namespace game {

	/*
		"Mothership" is a boss type entity that spawns barriers around the player and encloses them within an area where enemies
		continuously spawn until defeated.
	*/
	class Mothership : public EnemyGameObject {

		public:
			// Constructor
			Mothership(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint* textures, GameObject* player);

			// Getters
			inline const glm::vec3& GetDirection(void) const { return direction_; }
			inline float GetSpeed(void) const { return speed_; }

			// Loads in the barriers for the boss area
			void LoadBarriers(void);

			// Spawns an Enemy
			void SpawnEnemy(void);

			// Override collide method for custom behavior
			void Collide(GameObject* object) override;

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Variables to define boss zone
			int width_;
			int height_;
			GameObject* player_;

			// Desired direction vector for movement
			glm::vec3 direction_;

			// Mothership speed
			float speed_;

			// Values to track enemy spawning regularly
			Timer enemy_spawn_timer_;
			int total_enemy_count_;

			// Timer for shifting directions
			Timer shift_cooldown_;

			// References to textures (many are needed)
			GLuint* tex_;

	}; // class Mothership

} // namespace game

#endif // MOTHERSHIP_BOSS_H_