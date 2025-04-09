#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "collectible_game_object.h"
#include "particle_system.h"
#include "projectile.h"
#include "pulse.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Getters
            inline glm::vec3 GetVelocity(void) const { return velocity_; }
            inline float GetMaxVelocity(void) const { return max_velocity_; }
            inline float GetAcceleration(void) const { return acceleration_; }
            inline float GetFuel(void) const { return fuel_; }
            inline bool isBoosting(void) const { return use_boost_; }

            // Setters
            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
            inline void SetMaxVelocity(float max_velocity) { max_velocity_ = max_velocity; }
            inline void SetAcceleration(float acceleration) { acceleration_ = acceleration; }
            inline void AllowBoost(bool use_boost) { use_boost_ = use_boost; }
            inline void ProjectileDestroyed(void) { projectile_count_--; }

            // Function that fires projectile and returns its pointer, or nullptr if none were fired
            Projectile* FireProjectile(GLuint texture);
            // Function that fires pulse and returns its pointer, or nullptr if none were fired
            Pulse* FirePulse(GLuint texture);

            // Overrided function inherited from GameObject for custom reaction to collisions
            void Collide(GameObject* other) override;

            // Update function for moving the player object around
            void Update(double delta_time) override;

        protected:
            // Values for implementing physics-based movement
            glm::vec3 velocity_;
            float acceleration_;
            float max_velocity_;

            // Trackers for collectibles gathered by player
            int shield_collectible_count_;
            float fuel_;

            // Tracker for player boost/nitro usage
            bool use_boost_;
            Timer fuel_consumption_;

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
