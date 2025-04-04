#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Template used for enemy types. Inherits from GameObject.
	class EnemyGameObject : public GameObject {

		public:
			// Constructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* mothership = nullptr);

		protected:
			// Target for enemy objects to track so as to remain within its vicinity
			GameObject* mothership_;

		}; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_