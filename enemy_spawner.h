#ifndef ENEMY_SPAWNER_H_
#define ENEMY_SPAWNER_H_

#include <string>

#include "game_object.h"
#include "player_game_object.h"
#include "enemy_game_object.h"
#include "timer.h"

namespace game {
	
	class EnemySpawner : public GameObject {
		public:
			// Constructor
			EnemySpawner(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, PlayerGameObject* player);

			// Update

			// SpawnEnemy Function

			// render with empty png

		private:
			// Transform Variables
			glm::vec3 position_;

			// Timer for spawning cooldown

			// Flag to know if spawner is active (false when player too far)
			bool active_;
	};
}

#endif
