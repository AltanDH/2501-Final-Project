#ifndef BOSS_BARRIER_H_
#define BOSS_BARRIER_H_

#include "game_object.h"
#include "timer.h"

// Forward declare Mothership class (needed because of circular includes problems if not done)
namespace game {
	class Mothership;
}

namespace game {

	class BossBarrier : public GameObject {
		
		public:
			// Constructor
			BossBarrier(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, Mothership* mothership = nullptr);

			// Override collide function for custom behavior
			void Collide(GameObject* other) override;

			// Override update method for custom behavior
			void Update(double delta_time) override;

		protected:
			// Tracker for walls to move alongside the mothership
			Mothership* mothership_;

			// Timer to give the wall a cooldown when inflicting damage to entities that encounter it
			Timer dmg_cooldown_;
	
	}; // class BossBarrier

} // namespace game

#endif // BOSS_BARRIER_H_