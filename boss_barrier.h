#ifndef BOSS_BARRIER_H_
#define BOSS_BARRIER_H_

#include "game_object.h"

// Forward declare Mothership class (needed because of circular includes problems if not done)
namespace game {
	class Mothership;
}

namespace game {

	class BossBarrier : public GameObject {
		
		public:
			// Constructor
			BossBarrier(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Mothership* mothership);

			// Override collide function for custom behavior
			void Collide(GameObject* object) override;

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Tracker for walls to move alongside the mothership
			Mothership* mothership_;
	
	}; // class BossBarrier

} // namespace game

#endif // BOSS_BARRIER_H_