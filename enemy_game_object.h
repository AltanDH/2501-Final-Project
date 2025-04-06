#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

#include <vector>

namespace game {

	// Template used for enemy types. Inherits from GameObject.
	class EnemyGameObject : public GameObject {

		public:
			// Constructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* mothership = nullptr);

			// Setter for the game objects pointer
			inline void SetGameObjectsRef(std::vector<GameObject*>* game_objects) { game_objects_ref_ = game_objects; }

		protected:
			// Target for enemy objects to track so as to remain within its vicinity
			GameObject* mothership_;

			// Pointer to game objects stored in game (some enemy types require access to it to add relevant data)
			std::vector<GameObject*>* game_objects_ref_;

		}; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_