#ifndef PULSE_H_
#define PULSE_H_

#include "game_object.h"

namespace game {

    // Pulse that radiates outwards, expanding.
    class Pulse : public GameObject {

    public:
        // Constructor
        Pulse(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture, GameObject* owner);

        // Update function for moving the expanding pulse
        void Update(double delta_time) override;

        // Overrided collision for custom behavior
        void Collide(GameObject* other) override;

    protected:
        // Value used to control pulse expansion
        float max_scale_;

        // Timer for pulse lifespan
        Timer lifespan_;

        // Value to track entity that fired pulse (to avoid colliding with itself)
        GameObject* owner_;

    }; // class Pulse

} // namespace game

#endif // PULSE_H_
