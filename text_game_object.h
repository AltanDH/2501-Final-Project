#ifndef TEXT_GAME_OBJECT_H_
#define TEXT_GAME_OBJECT_H_

#include <string>

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class TextGameObject : public GameObject {

        public:
            TextGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture);

            // Text to be displayed
            std::string GetText(void) const;
            void SetText(std::string text);

            // Override to do nothing (UI shouldn't have collisions)
            void Collide(GameObject* other) override;

            // Render function for the text
            void Render(glm::mat4 view_matrix, glm::mat4 view_matrix_fixed, double current_time) override;

        private:
            std::string text_;

    }; // class TextGameObject

} // namespace game

#endif // TEXT_GAME_OBJECT_H_
