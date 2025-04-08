#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "projectile.h"
#include "pulse.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            // Overrided function inherited from GameObject for custom reaction to collisions
            void Collide(GameObject* other) override;

            // Function that fires projectile and returns its pointer, or nullptr if none were fired
            Projectile* FireProjectile(GLuint texture);
            // Function that fires pulse and returns its pointer, or nullptr if none were fired
            Pulse* FirePulse(GLuint texture);

            // Getters
            inline glm::vec3 GetVelocity(void) const { return velocity_; }
            inline float GetAcceleration(void) const { return acceleration_; }

            // Setters
            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
            inline void SetAcceleration(float acceleration) { acceleration_ = acceleration; }
            inline void ProjectileDestroyed(void) { projectile_count_--; }

        protected:
            // Values for implementing physics-based movement
            glm::vec3 velocity_;
            float acceleration_;
            float max_velocity_;

            // Tracker for collectibles gathered by player
            int collectible_count_;

            // Timer to track invinvibility duration
            Timer invincibility_duration_;

            // Timer to add projectile firing cooldown to avoid multiple shots being fired at one click
            Timer projectile_cooldown_;
            // Timer to add pulse firing cooldown (only 1 pulse can be shot at a time)
            Timer pulse_cooldown_;
            // Tracker for total projectiles fired
            int projectile_count_;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
