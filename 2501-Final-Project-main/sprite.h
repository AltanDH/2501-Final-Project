#ifndef SPRITE_H_
#define SPRITE_H_

#include "geometry.h"

namespace game {

    // A sprite (i.e., a square composed of two triangles)
    class Sprite : public Geometry {

        public:
            // Constructor and destructor
            Sprite(void);
            Sprite(float scale);

            // Create the geometry (called once)
            void CreateGeometry(void);

            // Use the geometry
            void SetGeometry(GLuint shader_program);

        private:
            // Scale for texture coordinates used in Sprite Geometry creation
            float texture_scale_;

    }; // class Sprite
} // namespace game

#endif // SPRITE_H_
