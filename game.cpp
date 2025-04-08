#include <stdexcept>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <SOIL/SOIL.h>
#include <iostream>

#include <path_config.h>

#include "sprite.h"
#include "shader.h"
#include "player_game_object.h"
#include "mothership_boss.h"
#include "collectible_game_object.h"
#include "projectile.h"
//#include "hierarchical_transformation.h"
#include "game.h"


namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Final Project";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


void Game::SetupGameWorld(void)
{

    // Setup the game world

    // **** Load all the textures that we will need
    //
    // Declare all the textures here
    std::vector<std::string> textures;
    enum { tex_player = 0,
           tex_invincible = 1,
           tex_mothership = 2,
           tex_barrier = 3,
           tex_boomer = 4,
           tex_dreadnought = 5,
           tex_fighter = 6,
           tex_explosion = 7,
           tex_projectile_player = 8,
           tex_projectile_enemy = 9,
           tex_pulse = 10,
           tex_collectible = 11,
           tex_stars = 12,
           tex_orb = 13,
           tex_empty = 14};
    textures.push_back("/textures/player.png");
    textures.push_back("/textures/invincible.png");
    textures.push_back("/textures/mothership.png");
    textures.push_back("/textures/barrier.png");
    textures.push_back("/textures/boomer.png");
    textures.push_back("/textures/dreadnought.png");
    textures.push_back("/textures/fighter.png");
    textures.push_back("/textures/explosion.png");
    textures.push_back("/textures/projectile_player.png");
    textures.push_back("/textures/projectile_enemy.png");
    textures.push_back("/textures/pulse.png");
    textures.push_back("/textures/Powerups/collectible.png");
    textures.push_back("/textures/stars.png");
    textures.push_back("/textures/orb.png");
    textures.push_back("/textures/empty.png");
    // Load textures
    LoadTextures(textures);


    // **** Setup all the game objects in the world

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_player]));
    float pi_over_two = glm::pi<float>() / 2.0f;
    game_objects_[0]->SetRotation(pi_over_two);

    // Spawn Mothership (boss)
    // Note that, in this specific implementation, the boss object will always be the second object in the game object vector
    Mothership* mothership = new Mothership(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_, game_objects_[0]);
    mothership->SetGameObjectsRef(&game_objects_);
    mothership->SetRotation(pi_over_two);
    game_objects_.push_back(mothership);

    // Load boss area (place barries to encase player)
    mothership->LoadBarriers();

    /* HIERARCHICAL TRANSFORMATION
    GameObject* obj1 = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_boomer]);
    GameObject* obj2 = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_player]);
    GameObject* obj3 = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_player]);
    GameObject* obj4 = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_fighter]);
    HierarchicalTransformation* hier = new HierarchicalTransformation(obj1, obj2, obj3, obj4, glm::vec3(-30.0f, -30.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_empty]);
    game_objects_.push_back(hier);
    game_objects_.push_back(obj1);
    game_objects_.push_back(obj2);
    game_objects_.push_back(obj3);
    game_objects_.push_back(obj4);*/

    /* Spawn a Planet
    // Note the planet radius to scale ratio --> radius = 0.4 + (0.4 * scale)
    CelestialBody* planet = new CelestialBody(glm::vec3(-7.0f, -7.0f, 0.0f), sprite_, &sprite_shader_, tex_[tex_orb], 2.0f);
    planet->SetScale(4.0f);
    celestial_objects_.push_back(planet);*/
    
    // Create a scale for the background texture coordinates enhancing
    float texture_coord_scale = 100.0f;
    // Make a new sprite allowing for the background tiling effect
    Geometry* bckgrnd_sprite = new Sprite(texture_coord_scale);
    bckgrnd_sprite->CreateGeometry();
    
    // Setup background
    GameObject *background = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), bckgrnd_sprite, &sprite_shader_, tex_[tex_stars]);
    // Resize background to suit the new scale
    background->SetScale(12.0 * texture_coord_scale);

    // In this specific implementation, the background will always be the last object
    game_objects_.push_back(background);
}


void Game::DestroyGameWorld(void)
{
    // Free memory for all objects
    // Only need to delete objects that are not automatically freed
    for (int i = 0; i < game_objects_.size(); i++){
        delete game_objects_[i];
    }

    for (int i = 0; i < celestial_objects_.size(); i++) {
        delete celestial_objects_[i];
    }
}

void Game::HandleControls(double delta_time)
{
    // Get player game object and typecast it to be able to use relevant methods
    PlayerGameObject *player = (PlayerGameObject*)game_objects_[0];
    // Get current position and angle
    glm::vec3 curpos = player->GetPosition();
    float angle = player->GetRotation();
    // Compute current bearing direction
    glm::vec3 dir = player->GetBearing();
    // Adjust motion increment and angle increment 
    // if translation or rotation is too slow
    float speed = delta_time*1000.0;
    float motion_increment = 0.0015*speed;
    float angle_increment = (glm::pi<float>() / 1800.0f)*speed*2;

    // Get current velocity and acceleration
    glm::vec3 velocity = player->GetVelocity();
    float acceleration = player->GetAcceleration()*1.6;

    // Check for player input and make changes accordingly
    // Move player in desired direction
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->SetVelocity(velocity + acceleration * dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetVelocity(velocity - acceleration * dir);
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        player->SetVelocity(velocity - acceleration * player->GetRight());
    }
    if (glfwGetKey(window_, GLFW_KEY_E) == GLFW_PRESS) {
        player->SetVelocity(velocity + acceleration * player->GetRight());
    }

    // Rotate player as requested
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetRotation(angle - angle_increment);
        //player->SetVelocity(velocity * (dir + angle_increment));
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetRotation(angle + angle_increment);
        //player->SetVelocity(velocity * (dir - angle_increment));
    }

    // Fire projectile if requested
    if (glfwGetKey(window_, GLFW_KEY_F) == GLFW_PRESS) {
        // Call the Fire function from player which will return the created projectile, or nullptr if not fired
        // Cooldown & Too many bullets fired logic handled within Fire function
        GameObject* projectile = player->FireProjectile(tex_[8]);

        // Make sure we received a valid projectile (meaning one was fired)
        if (projectile != nullptr) {
            // Add it to game objects (we insert it right before the background to avoid collision detection issues)
            game_objects_.insert(game_objects_.end() - 1, projectile);
        }
    }

    // Fire pulse if requested
    if (glfwGetKey(window_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        // Call the Fire function from player which will return the created pulse, or nullptr if not fired
        // Cooldown logic handled within Fire function
        GameObject* pulse = player->FirePulse(tex_[10]);

        // Make sure we received a valid pulse (meaning one was fired)
        if (pulse != nullptr) {
            // Add it to game objects (we insert it right before the background to avoid collision detection issues)
            game_objects_.insert(game_objects_.end() - 1, pulse);
        }
    }

    // Close Window if requested by player
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
}


void Game::Update(double delta_time)
{
    // Spawns Collectibles when necessary (with internal checks)
    SpawnCollectible();

    // Check player invincibility
    // note that as long as player alive, game_object_[0] is always the player
    if (game_objects_[0]->GetType() == "Player") {
        // Check if player is invincible (thus requires changing texture)
        if (((PlayerGameObject*)game_objects_[0])->isInvincible()) {
            // note tex_[3] is already defined to be the invincible player texture
            game_objects_[0]->SetTexture(tex_[1]);
        }
        // and vice-versa if not invincible (also make sure player not destroyed to avoid resetting texture)
        else if (!game_objects_[0]->isDestroyed()) {
            // note tex_[2] is already defined to be the normal player texture
            game_objects_[0]->SetTexture(tex_[0]);
        }
    }

    // Update all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);

        // Check for collision with other game objects
        // Note the loop bounds: we avoid testing the last object since
        // it's the background covering the whole game world
        for (int j = i + 1; j < (game_objects_.size() - 1); j++) {
            GameObject* other_game_object = game_objects_[j];

            // Don't waste time on already destroyed objects
            if (!current_game_object->isDestroyed() && !other_game_object->isDestroyed()) {
                // Compute distance between both objects (to be used for circle-circle collision)
                float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());

                // Check if Projectiles are involved (then Ray-Circle method necessary)
                if (current_game_object->GetType() == "Projectile") {
                    if (RayCircleCollision(current_game_object->GetPosition(), current_game_object->GetBearing(), other_game_object->GetPosition(), other_game_object->GetRadius(), ((Projectile*)current_game_object)->GetSpeed(), delta_time)) {
                        current_game_object->Collide(other_game_object);
                        other_game_object->Collide(current_game_object);
                    }
                }
                if (other_game_object->GetType() == "Projectile") {
                    if (RayCircleCollision(other_game_object->GetPosition(), other_game_object->GetBearing(), current_game_object->GetPosition(), current_game_object->GetRadius(), ((Projectile*)other_game_object)->GetSpeed(), delta_time)) {
                        current_game_object->Collide(other_game_object);
                        other_game_object->Collide(current_game_object);
                    }
                }
                // Otherwise, if distance is below a threshold, we have a collision (circle-circle method)
                else if (distance < (current_game_object->GetRadius() + other_game_object->GetRadius())) {
                    // Trigger collision functions
                    current_game_object->Collide(other_game_object);
                    other_game_object->Collide(current_game_object);
                }

                // Change object texture to explosions if destroyed (except for collectibles)
                if (current_game_object->isDestroyed() && current_game_object->GetType() != "Collectible") {
                    current_game_object->SetTexture(tex_[7]);

                    // Update total enemy count (all enemies are spawned by mothership for the purposes of this project)
                    if (current_game_object->GetType() == "Enemy") {
                        // For the purposes of this project, the Mothership is always maintained in the second position of the array
                        ((Mothership*)game_objects_[1])->EnemyDied();
                    }
                }
                if (other_game_object->isDestroyed() && other_game_object->GetType() != "Collectible") {
                    other_game_object->SetTexture(tex_[7]);

                    // Update total enemy count (all enemies are spawned by mothership for the purposes of this project)
                    if (other_game_object->GetType() == "Enemy") {
                        // For the purposes of this project, the Mothership is always maintained in the second position of the array
                        ((Mothership*)game_objects_[1])->EnemyDied();
                    }
                }
            }
        }
    }

    // Update all celestial objects
    for (int i = 0; i < celestial_objects_.size(); i++) {
        // Get the current celestial object
        CelestialBody* celestial_object = celestial_objects_[i];

        // Update the current celestial object
        celestial_object->Update(delta_time);

        // Check for collision with other game objects
        for (int j = 0; j < (game_objects_.size() - 1); j++) {
            GameObject* game_object = game_objects_[j];

            // Don't waste time on already destroyed objects
            if (!game_object->isDestroyed()) {
                // Compute distance between both objects (to be used for circle-circle collision)
                float distance = glm::distance(celestial_object->GetPosition(), game_object->GetPosition());
                
                // If distance is below a threshold, we have a collision (circle-circle method)
                if (distance < (celestial_object->GetRadius() + game_object->GetRadius())) {
                    // Trigger collision functions
                    celestial_object->Collide(game_object);
                    game_object->Collide(celestial_object);
                }

                // Change object texture to explosions if destroyed (except for collectibles)
                if (game_object->isDestroyed() && game_object->GetType() != "Collectible") {
                    game_object->SetTexture(tex_[7]);
                }
            }

            // Check if the object is a boomer and apply physics
            BoomerEnemy* boomer = dynamic_cast<BoomerEnemy*>(game_objects_[j]);
            if (boomer != nullptr) {
                celestial_object->BoomerChasePlayer(boomer, boomer->GetTarget(), delta_time);
            }
        }

        // Apply Celestial object gravity to player
        celestial_object->GravitationalAcceleration((PlayerGameObject*)game_objects_[0], delta_time);
    }
    
    // Clean up destroyed objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Make sure correct conditions have been met
        if (game_objects_[i]->isDestroyed() && game_objects_[i]->GetTimer().Finished()) {
            // Check if we're deleting the player or mothership (win condition met)
            if (i == 0 || i == 1) {

                // Player is always the first item in game_objects_ array so we'd know they lost if they're getting deleted
                if (i == 0) {
                    // Provide feedback to user
                    std::cout << "Game Over!" << std::endl;
                }

                // Mothership (Boss) is always the second item in game_objects_ array so we'd know player won if it's getting deleted
                else if (i == 1) {
                    // Feedback to user
                    std::cout << "Victory!! Well done." << std::endl;
                }

                // Free memory used by game world
                DestroyGameWorld();

                // Free rendering resources
                delete sprite_;

                // Close window
                glfwDestroyWindow(window_);
                glfwTerminate();

                // Terminate program
                exit(0);
            }
            
            // Otherwise we're deleting a normal game object
            delete game_objects_[i];
            // Make sure to remove the object pointer/entry from our list
            game_objects_.erase(game_objects_.begin() + i);
            
            // Re-adjust the index since we just deleted an object
            i--;
        }
    }
}


void Game::Render(void){

    // Clear background
    glClearColor(viewport_background_color_g.r,
                 viewport_background_color_g.g,
                 viewport_background_color_g.b, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use aspect ratio to properly scale the window
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glm::mat4 window_scale_matrix;
    if (width > height){
        float aspect_ratio = ((float) width)/((float) height);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/aspect_ratio, 1.0f, 1.0f));
    } else {
        float aspect_ratio = ((float) height)/((float) width);
        window_scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f/aspect_ratio, 1.0f));
    }

    // Set view to zoom out, centered by default at 0,0
    float camera_zoom;
    // Make camera zoom out procedurally during game beginning
    if (glfwGetTime() < 4) {
        camera_zoom = 0.6f - (glfwGetTime() / 8.0f);
    }
    // Then stabilize it
    else {
        camera_zoom = 0.6f - (4.0f / 8.0f);  // 0.1
    }
    glm::mat4 camera_zoom_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(camera_zoom, camera_zoom, camera_zoom));

    // Create the translation that allows the camera to follow the player object
    glm::mat4 player_translation = glm::translate(glm::mat4(1.0f), -game_objects_[0]->GetPosition());
    // Form the view_matrix that will follow the player
    glm::mat4 view_matrix = window_scale_matrix * camera_zoom_matrix * player_translation;

    // Render all celestial objects
    for (int i = 0; i < celestial_objects_.size(); i++) {
        celestial_objects_[i]->Render(view_matrix, current_time_);
    }
    
    // Render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        game_objects_[i]->Render(view_matrix, current_time_);
    }
}


void Game::SpawnCollectible(void) {
    // Check if it's time to spawn a collectible
    if (collectible_spawn_timer_.Finished()) {
        // Generate random spawn position
        glm::vec3 spawn_position = glm::vec3(0.0f, 0.0f, 0.0f);
        spawn_position.x = game_objects_[1]->GetPosition().x + rand() % 20 - 4;
        spawn_position.y = game_objects_[1]->GetPosition().y - rand() % 20 - 4;

        // Create a new collectible game object at the spawn position
        // note that tex[11] already defined as corresponding collectible texture
        CollectibleGameObject* new_collectible = new CollectibleGameObject(spawn_position, sprite_, &sprite_shader_, tex_[11]);

        // Add the new collectible to game objects list (note we insert it such that it gets added right before the background to avoid collision detection mismatch)
        game_objects_.insert(game_objects_.end() - 1, new_collectible);

        collectible_spawn_timer_.Start(rand() % 8 + 7);
    }
}

// Perform Ray-Circle Collision check
bool Game::RayCircleCollision(const glm::vec3& proj_pos, const glm::vec3& proj_dir, const glm::vec3& other_pos, float other_rad, float proj_speed, double delta_time) {
    // Vector from Circle's center to Ray's origin
    glm::vec3 circ_to_ray = proj_pos - other_pos;

    // Compute quadratic equation coefficients to find intersections
    float a = glm::dot(proj_dir, proj_dir);
    float b = 2.0f * glm::dot(circ_to_ray, proj_dir);
    float c = glm::dot(circ_to_ray, circ_to_ray) - other_rad * other_rad;

    // Find discriminant
    float discriminant = b * b - 4 * a * c;

    // Don't need to keep going if solutions/intersections don't exist
    if (discriminant < 0) {
        return false;
    }

    // Compute intersection points along the ray
    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    // Calculate how far the projectile can travel within delta_time
    float max_distance = proj_speed * delta_time;

    // Check if either intersection occurs in front of projectile and within its movement range (to avoid premature collisions)
    return (t1 > 0 && t1 <= max_distance) || (t2 > 0 && t2 <= max_distance);
}


void Game::MainLoop(void)
{
    // Loop while the user did not close the window
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window_)){

        // Calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;

        // Update window events like input handling
        glfwPollEvents();

        // Handle user input
        HandleControls(delta_time);

        // Update all the game objects
        Update(delta_time);

        // Render all the game objects
        Render();

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);
    }
}


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set whether window can be resized
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Initialize sprite geometry
    sprite_ = new Sprite();
    sprite_->CreateGeometry();

    // Initialize sprite shader
    sprite_shader_.Init((resources_directory_g+std::string("/sprite_vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/sprite_fragment_shader.glsl")).c_str());

    // Initialize time
    current_time_ = 0.0;

    // Initialize timer to spawn collectibles
    collectible_spawn_timer_ = Timer();
    collectible_spawn_timer_.Start(6.0f);
}


Game::~Game()
{
    // Free memory used by game world
    DestroyGameWorld();

    // Free rendering resources
    delete sprite_;

    // Close window
    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        std::cout << "Cannot load texture " << fname << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping using GL_REPEAT for background tiling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::LoadTextures(std::vector<std::string> &textures)
{
    // Allocate a buffer for all texture references
    int num_textures = textures.size();
    tex_ = new GLuint[num_textures];
    glGenTextures(num_textures, tex_);
    // Load each texture
    for (int i = 0; i < num_textures; i++){
        SetTexture(tex_[i], (resources_directory_g+textures[i]).c_str());
    }
    // Set first texture in the array as default
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}

} // namespace game
