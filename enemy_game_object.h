#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include <string>

namespace game {

	// Inherits from GameObject
	class EnemyGameObject : public GameObject {

		public:
			// Constructor
			EnemyGameObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* target);

			// Update function for moving the enemy object around
			void Update(double delta_time) override;

		protected:
			// Enemy state (can be either "patrolling" or "intercepting")
			std::string state_;

			// Variables for enemy object elliptical trajectory
			// Values representing ellipse
			float width_;
			float height_;
			glm::vec3 center_;
			// Tracker for progress in angular progression around ellipse
			float angular_movement_;

			// Target for enemy objects to pursue
			GameObject* target_;

			// Desired direction vector for pursuit
			glm::vec3 direction_;

			// Timer for trajectory correction/target recalculation during pursuit/"intercepting"
			Timer recalibration_interval_;

		}; // class EnemyGameObject

} // namespace game

#endif // ENEMY_GAME_OBJECT_H_