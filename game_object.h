#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>

#include "shader.h"
#include "geometry.h"
#include "timer.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of one object in the game world
        The update and render methods are virtual, so you can inherit them from GameObject and override the update or render functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float scaleX = 1.0f, float scaleY = 1.0f);

            // Update the GameObject's state. Can be overriden in children
            virtual void Update(double delta_time);

            // Renders the GameObject 
            virtual void Render(glm::mat4 view_matrix, double current_time);

            // Getters
            inline glm::vec3 GetPosition(void) const { return position_; }
            inline glm::vec2 GetScale(void) const { return scale_; }
            inline float GetRotation(void) const { return angle_; }
            inline float GetRadius(void) const { return radius_; }
            inline int GetHitpoints(void) const { return hitpoints_; }

            // Getter to retrieve GameObject type (ex: "Player", "Collectible", "Enemy", etc..)
            inline const std::string& GetType(void) const { return type_; }
            
            // Getter to check if object has been destroyed
            inline bool isDestroyed(void) const { return is_destroyed_; }

            // Getter to be able to access object timer
            inline Timer& GetTimer(void) { return timer_; }

            // Get bearing direction (direction in which the game object
            // is facing)
            glm::vec3 GetBearing(void) const;

            // Get vector pointing to the right side of the game object
            glm::vec3 GetRight(void) const;

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { scale_ = glm::vec2(scale, scale); }
            inline void SetScale(glm::vec2 scale) { scale_ = scale; }
            void SetRotation(float angle);
            inline void SetTexture(GLuint texture) { texture_ = texture; }

            inline void SetInvincible(bool b) { invincible_ = b;  }
            inline bool GetInvincible(void) const { return invincible_; }

            void SetHitpoints(int health);
            inline void SetDestroyed(bool destroyed) { is_destroyed_ = destroyed; }

            // Placeholder function only meant for use in inheritance
            virtual void SetVelocity(const glm::vec3& velocity);

            // Function to simulate collision
            virtual void Collide(GameObject* object);


        protected:
            // Object's Transform Variables
            glm::vec3 position_;
            glm::vec2 scale_;
            float angle_;

            // Radius variable for circle-to-circle collision
            float radius_;

            // Variables to track object destruction
            int hitpoints_;
            bool is_destroyed_;

            // Property to trigger grayscale
            bool ghost_;

            // Geometry
            Geometry *geometry_;
 
            // Shader
            Shader *shader_;

            // Object's texture reference
            GLuint texture_;

            // Timer to use when object is destroyed
            Timer timer_;

            // String to identify the type of GameObject
            std::string type_;

            bool invincible_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
