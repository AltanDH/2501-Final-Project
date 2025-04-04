#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "enemy_game_object.h"

namespace game {
	
	// Constructor
	EnemyGameObject::EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* mothership)
		: GameObject(position, geom, shader, texture) {
		
		// Default enemy info
		type_ = "Enemy";
		mothership_ = mothership;
	}

} // namespace game