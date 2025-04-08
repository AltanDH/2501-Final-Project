#include "celestial_body.h"
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 

namespace game {

    // Constructor
    CelestialBody::CelestialBody(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, float radius)
        : GameObject(position, geom, shader, texture) {

        // Initialize member
        gravitational_constant_ = 6.67;

        // Radius is an attribute of game object
        radius_ = radius;

        type_ = "CelestialBody";
    }

    // Calculate acceleration given parameters
    void CelestialBody::GravitationalAcceleration(PlayerGameObject* player, double delta_time) {
        glm::vec3 planet_to_player = player->GetPosition() - position_;
        glm::vec3 vel = player->GetVelocity();
        float speed = glm::length(vel);
        float distance_to_player = glm::length(planet_to_player);
        //std::cout << speed << std::endl;

        if (distance_to_player < 0.001f) { return; } // Avoid division by zero
        if (glm::length(vel) < 0.1f) { goto physics_based; } // If we are too slow, rotating velocity will feel weird, so use physics based

        if (distance_to_player < radius_ * 3) {
            // Calculate both possible tangents
            glm::vec3 left_tangent = glm::vec3(-planet_to_player.y, planet_to_player.x, 0.0f); // Counter clockwise movement tangent
            glm::vec3 right_tangent = glm::vec3(planet_to_player.y, -planet_to_player.x, 0.0f); // Clockwise movement tangent

            // Determine which tangent to target based on current direction
            glm::vec3 vel_dir = glm::normalize(vel);
            bool is_counter_clockwise = glm::dot(vel_dir, glm::normalize(left_tangent)) > 0;

            // Set the tangent we want from the orbit based on if we are going counter clockwise or not
            glm::vec3 target_tangent = is_counter_clockwise ? left_tangent : right_tangent;
            glm::vec3 tangent_dir = glm::normalize(target_tangent);

            // Calculate angle between tangent and velocity.
            // |a x b| = |a| * |b| * sin(theta), where theta is the angle between vectors.
            // |a| * |b| = 1 since they are normalized, therefore |a x b| = sin(theta).
            // Project velocity direction onto the orbital tangent direction.
            float cos_theta = glm::dot(vel_dir, tangent_dir);
            // Get sin of the angle of the vectors by getting 2D cross product
            float sin_theta = vel_dir.x * tangent_dir.y - vel_dir.y * tangent_dir.x;
            float target_angle = atan2(sin_theta, cos_theta);

            // Calculate angle increment with delta_time and clamp
            float rotation_speed = 0.5; // Adjust this value to control rotation speed
            float angle_increment = target_angle * rotation_speed * delta_time;

            // Clamp to prevent overshooting the angle we wantw
            if (angle_increment > 0) { angle_increment = glm::min(angle_increment, target_angle); }
            else { angle_increment = glm::max(angle_increment, target_angle); }

            // Apply rotation to velocity
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle_increment, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec4 new_vel = rotation * glm::vec4(vel, 0.0f);
            glm::vec3 final_velocity = glm::vec3(new_vel.x, new_vel.y, 0.0f);
            player->SetVelocity(final_velocity);
        }

    physics_based:
        // Calculate distance to the planet
        float distance = glm::length(player->GetPosition() - position_);
        // Get direction vector from player to the planet
        glm::vec3 dir = glm::normalize(position_ - player->GetPosition());
        // Calculate its effect on velocity
        player->SetVelocity(player->GetVelocity() + (dir * (float)delta_time * (gravitational_constant_ * radius_ / (distance))));

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

    // Call this in enemy update function
    void CelestialBody::BoomerChasePlayer(BoomerEnemy* boomer, PlayerGameObject* player, double delta_time) {
        // Get the distance of the enemy to the planet, check if we are in rotation orbit radius
        glm::vec3 planet_to_boomer = boomer->GetPosition() - this->GetPosition();
        float distance_to_boomer = glm::length(planet_to_boomer);

        // If the enemy is within the range of rotating t
        if (distance_to_boomer < radius_ * 3 && distance_to_boomer > 0.001) {
            glm::vec3 vel = boomer->GetVelocity();
            boomer->SetInOrbit(true);

            // Calculate both possible tangents
            glm::vec3 left_tangent = glm::vec3(-planet_to_boomer.y, planet_to_boomer.x, 0.0f); // Counter clockwise movement tangent
            glm::vec3 right_tangent = glm::vec3(planet_to_boomer.y, -planet_to_boomer.x, 0.0f); // Clockwise movement tangent

            // Determine which tangent to target based on current direction
            glm::vec3 vel_dir = glm::normalize(vel);
            bool is_counter_clockwise = glm::dot(vel_dir, glm::normalize(left_tangent)) > 0;

            // Set the tangent we want from the orbit based on if we are going counter clockwise or not
            glm::vec3 target_tangent = is_counter_clockwise ? left_tangent : right_tangent;
            glm::vec3 tangent_dir = glm::normalize(target_tangent);

            // Calculate angle between tangent and velocity.
            // |a x b| = |a| * |b| * sin(theta), where theta is the angle between vectors.
            // |a| * |b| = 1 since they are normalized, therefore |a x b| = sin(theta).
            // Project velocity direction onto the orbital tangent direction.
            float cos_theta = glm::dot(vel_dir, tangent_dir);
            // Get sin of the angle of the vectors by getting 2D cross product
            float sin_theta = vel_dir.x * tangent_dir.y - vel_dir.y * tangent_dir.x;
            float target_angle = atan2(sin_theta, cos_theta);

            // Calculate angle increment with delta_time and clamp
            float rotation_speed = 1; // Adjust this value to control rotation speed
            float angle_increment = target_angle * rotation_speed * delta_time;

            // Clamp to prevent overshooting the angle we wantw
            if (angle_increment > 0) { angle_increment = glm::min(angle_increment, target_angle); }
            else { angle_increment = glm::max(angle_increment, target_angle); }

            // Apply rotation to velocity
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle_increment, glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec4 new_vel = rotation * glm::vec4(vel, 0.0f);
            glm::vec3 final_velocity = glm::vec3(new_vel.x, new_vel.y, 0.0f);
            std::cout << "VELOCITY: " << glm::length(final_velocity) << std::endl;
            boomer->SetVelocity(final_velocity);
            boomer->SetPosition(boomer->GetPosition() + boomer->GetVelocity() * (float)delta_time);

            // Increment the boomer height to the same height of the player from the planet to chase while in orbit
            glm::vec3 planet_to_player = player->GetPosition() - this->GetPosition();
            float distance_to_player = glm::length(planet_to_player);

            // Check if boomer height is too high or too low, increment the enemy height
            if (distance_to_boomer > distance_to_player) {
                boomer->SetPosition((boomer->GetPosition() - (glm::normalize(planet_to_boomer) * 0.0001f)));
            }
            else if (distance_to_boomer < distance_to_player) {
                boomer->SetPosition((boomer->GetPosition() + (glm::normalize(planet_to_boomer) * 0.0001f)));
            }

        }
        else {
            // Make the boomer feel regular physics oherwise
            boomer->SetInOrbit(false);
            // Calculate distance to the planet
            float distance = glm::length(boomer->GetPosition() - position_);
            // Get direction vector from player to the planet
            glm::vec3 dir = glm::normalize(position_ - boomer->GetPosition());
            // Calculate its effect on velocity
            boomer->SetVelocity(boomer->GetVelocity() + (dir * (float)delta_time * (gravitational_constant_ * radius_ / (distance))));
        }

    }

}
