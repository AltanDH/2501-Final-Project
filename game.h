#ifndef GAME_H_
#define GAME_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shader.h"
#include "game_object.h"
#include "celestial_body.h"

namespace game {

    // A class for holding the main game objects
    class Game {

        public:
            // Constructor and destructor
            Game(void);
            ~Game();

            // Call Init() before calling any other method
            // Initialize graphics libraries and main window
            void Init(void); 

            // Set up the game world (scene, game objects, etc.)
            void SetupGameWorld(void);

            // Destroy the game world
            void DestroyGameWorld(void);

            // Spawn a Collectible
            void SpawnCollectible(void);

            // Perform Ray-Circle Collision check
            bool RayCircleCollision(const glm::vec3& proj_pos, const glm::vec3& proj_dir, const glm::vec3& other_pos, float other_rad, float proj_speed, double delta_time);

            // Run the game (keep the game active)
            void MainLoop(void); 

        private:
            // Main window: pointer to the GLFW window structure
            GLFWwindow *window_;

            // Sprite geometry
            Geometry *sprite_;

            // Particle geometry
            Geometry *particles_;

            // Pulse Particle geometry
            Geometry* explosion_particles_;

            // Shader for rendering sprites in the scene
            Shader sprite_shader_;

            // Shader for rendering particles
            Shader particle_shader_;

            // Shader for rendering text
            Shader text_shader_;

            // Shader for drawing UI elements
            Shader drawing_shader_;

            // References to textures
            // This needs to be a pointer
            GLuint *tex_;

            // List of game objects
            std::vector<GameObject*> game_objects_;

            // List of celestial bodies (planets)
            std::vector<CelestialBody*> celestial_objects_;

            // Keep track of time
            double current_time_;

            // Timer to spawn collectibles regularly
            Timer collectible_spawn_timer_;

            // Handle user input
            void HandleControls(double delta_time);

            // Update all the game objects
            void Update(double delta_time);
 
            // Render the game world
            void Render(void);

            // Callback for when the window is resized
            static void ResizeCallback(GLFWwindow* window, int width, int height);

            // Set a specific texture
            void SetTexture(GLuint w, const char *fname);

            // Load all textures
            void LoadTextures(std::vector<std::string> &textures);
    
    }; // class Game

} // namespace game

#endif // GAME_H_
