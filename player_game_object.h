#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#include "projectile.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            // Overrided function inherited from GameObject for custom reaction to collisions
            void Collide(GameObject* object) override;

            // Function that fires projectile and returns its pointer, or nullptr if none were fired
            Projectile* Fire(GLuint texture);

            // Getter for invincibility state
            inline bool isInvincible(void) const { return is_invincible_; }
            inline void SetInvincible(bool b) { is_invincible_ = b; }

            // Getters
            inline glm::vec3 GetVelocity(void) const { return velocity_; }
            inline float GetAcceleration(void) const { return acceleration_; }
            inline Timer GetTimer(void) const { return invincibility_duration_; }

            // Setters
            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
            inline void SetAcceleration(float acceleration) { acceleration_ = acceleration; }

        protected:
            // Values for implementing physics-based movement
            glm::vec3 velocity_;
            float acceleration_;
            float max_velocity_;

            // Tracker for collectibles gathered by player
            int collectible_count_;

            // Boolean to track invincibility state
            bool is_invincible_;

            // Timer to track invinvibility duration
            Timer invincibility_duration_;

            // Timer to track projectile firing cooldown
            Timer shooting_cooldown_;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
