#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class Projectile : public GameObject {

    public:
        Projectile(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, const glm::vec3& direction, GameObject* owner);

        // Getters
        inline float GetSpeed(void) const { return speed_; }

        // Update function for moving the projectile around
        void Update(double delta_time) override;

        // Overrided function inherited from GameObject for custom reaction to collisions
        void Collide(GameObject* other) override;

    protected:
        // Values for implementing projectile movement
        glm::vec3 direction_;
        float speed_;

        // timer for projectile lifespan
        Timer lifespan_;

        // Value to track entity that fired pulse (to avoid colliding with itself)
        GameObject* owner_;

    }; // class Projectile

} // namespace game

#endif // PROJECTILE_H_
