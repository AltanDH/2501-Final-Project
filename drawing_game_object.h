#ifndef DRAWING_GAME_OBJECT_H_
#define DRAWING_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class DrawingGameObject : public GameObject {

        public:
            DrawingGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Value until which we fill the bar
            float GetFillValue(void) const;
            void SetFillValue(float fill_val);

            // Color of the filling
            void SetFillColor(const glm::vec4& color);

            // Override to do nothing (UI shouldn't have collisions)
            void Collide(GameObject* other) override;

            // Render function for the text
            void Render(glm::mat4 view_matrix, glm::mat4 view_matrix_fixed, double current_time) override;


        private:
            float fill_value_;
            glm::vec4 fill_color_;

    }; // class DrawingGameObject

} // namespace game

#endif // DRAWING_GAME_OBJECT_H_
