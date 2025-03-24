#ifndef CELESTIAL_BODY_H_
#define CELESTIAL_BODY_H_

#include "game_object.h"
#include "player_game_object.h"

namespace game {
	
	// Any space object satellite (that is not an enemy or player)
	class CelestialBody : public GameObject {
		public:
			// Constructor
			CelestialBody(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float radius = 0.4f);

			// Calculates acceleration based on CelestialBody
			void GravitationalAcceleration(PlayerGameObject* player, float delta_time);

			// Overwrite for custom collision
			void Collide(GameObject* object) override;

		private:
			// We will use this to model both gravity in a simple way
			// The game does not use mass in the formula gravitational_acceleration = (gravitational_constant x mass)/distance
			// So will instead use (gravitational_constant * radius)/distance.
			float gravitational_constant_;
		};
}

#endif