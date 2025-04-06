#ifndef BOSS_BARRIER_H_
#define BOSS_BARRIER_H_

#include "game_object.h"

namespace game {

	class BossBarrier : public GameObject {
		
		public:
			// Constructor
			BossBarrier(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);

			// Override collide function for custom behavior
			void Collide(GameObject* object) override;
	
	}; // class BossBarrier

} // namespace game

#endif // BOSS_BARRIER_H_