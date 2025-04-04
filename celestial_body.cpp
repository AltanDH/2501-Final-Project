#include "celestial_body.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 

namespace game {
	
	// Constructor
	CelestialBody::CelestialBody(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float radius)
		: GameObject(position, geom, shader, texture) {

		// Initialize member
		gravitational_constant_ = 6.67 * 0.0009;

		// Radius is an attribute of game object
		radius_ = radius;

		type_ = "CelestialBody";
	}

	// We're not checking if it reached the angle and is still rotating
	// And it's also over shooting
	// (Notice that counter clockwise works fine)
	// Calculate acceleration given parameters
	void CelestialBody::GravitationalAcceleration(PlayerGameObject* player, float delta_time) {
		
		glm::vec3 planet_to_player = player->GetPosition() - position_;

		if (abs(glm::length(planet_to_player)) < radius_ * 3) {
			glm::vec3 left_side = glm::vec3(planet_to_player.y, -planet_to_player.x, 0.0f);

			float speed = delta_time * 1000.0;
			//float angle_increment = (glm::pi<float>() / 1800.0f) * speed;
			// make angle increment bigger the faster the player is (stronger effect when fast, then clamp aka limit max speed we use)
			float angle_increment = (glm::pi<float>() / 1800.0f) * speed;

			// Check if player going counter-clocwise (to left side of planet)
			if (glm::dot(glm::normalize(player->GetVelocity()), left_side) > 0) {
				//player->SetRotation(player->GetRotation() - angle_increment);
				glm::vec3 vel = player->GetVelocity();
				glm::vec3 dir = glm::normalize(player->GetVelocity());
				//float angle_diff = atan2(dir.y, dir.x) - angle_increment;
				float angle_diff = atan2(planet_to_player.y, planet_to_player.x) - angle_increment;
				glm::vec3 player_pos = player->GetPosition();
				//glm::mat4 player_translation = glm::translate(glm::mat4(1.0f), -player_pos);
				//glm::vec4 rotated_velocity = player_translation * glm::rotate(glm::mat4(1.0f), angle_diff, glm::vec3(0.0f, 0.0f, 1.0f)) * -player_translation * glm::vec4(vel.x, vel.y, vel.z, 0.0f);
				glm::vec4 rotated_velocity = glm::rotate(glm::mat4(1.0f), angle_increment, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(player->GetVelocity(), 0.0f);
				player->SetVelocity(glm::vec3(rotated_velocity.x, rotated_velocity.y, rotated_velocity.z));
			}
			else {
				//player->SetRotation(player->GetRotation() + angle_increment);
				glm::vec3 vel = player->GetVelocity();
				glm::vec3 dir = glm::normalize(player->GetVelocity());
				//float angle_diff = atan2(dir.y, dir.x) + angle_increment;
				float angle_diff = atan2(planet_to_player.y, planet_to_player.x) + angle_increment;
				glm::vec3 player_pos = player->GetPosition();
				//glm::mat4 player_translation = glm::translate(glm::mat4(1.0f), -player_pos);
				//glm::vec4 rotated_velocity = player_translation * glm::rotate(glm::mat4(1.0f), angle_diff, glm::vec3(0.0f, 0.0f, 1.0f)) * -player_translation * glm::vec4(vel.x, vel.y, vel.z, 0.0f);
				glm::vec4 rotated_velocity = glm::rotate(glm::mat4(1.0f), angle_increment, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(player->GetVelocity(), 0.0f);
				player->SetVelocity(glm::vec3(rotated_velocity.x, rotated_velocity.y, rotated_velocity.z));
			}
		}
		else {
			float distance = glm::length(player->GetPosition() - position_);
			glm::vec3 dir = glm::normalize(position_ - player->GetPosition());
			//player->SetAcceleration(player->GetAcceleration() + (gravitational_constant_ * radius_ / distance));
			player->SetVelocity(player->GetVelocity() + (dir * delta_time * (gravitational_constant_ * radius_ / (distance * distance))));
		}
	}

	// Overrite for custom collision behavior
	void CelestialBody::Collide(GameObject* object) {
		// Check if a player or enemy collided into celestial body
		if ((object->GetType() == "Enemy" || object->GetType() == "Player") && !(object->isDestroyed())) {
			// Set timer for duration until object deletion
			object->GetTimer().Start(5.0f);

			// Instantly destroy them
			object->SetHitpoints(0);
			object->SetDestroyed(true);
		}
	}
}