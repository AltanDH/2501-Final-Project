#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, float scaleX, float scaleY) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = glm::vec2(scaleX, scaleY);
    angle_ = 0.0;
    radius_ = 0.4f;
    hitpoints_ = 1;
    is_destroyed_ = false;
    ghost_ = false;
    geometry_ = geom;
    shader_ = shader;
    texture_ = texture;
    timer_ = Timer();
    type_ = "Default";
}


glm::vec3 GameObject::GetBearing(void) const {

    glm::vec3 dir(cos(angle_), sin(angle_), 0.0);
    return dir;
}


glm::vec3 GameObject::GetRight(void) const {

    float pi_over_two = glm::pi<float>() / 2.0f;
    glm::vec3 dir(cos(angle_ - pi_over_two), sin(angle_ - pi_over_two), 0.0);
    return dir;
}


void GameObject::SetRotation(float angle){ 

    // Set rotation angle of the game object
    // Make sure angle is in the range [0, 2*pi]
    float two_pi = 2.0f*glm::pi<float>();
    angle = fmod(angle, two_pi);
    if (angle < 0.0){
        angle += two_pi;
    }
    angle_ = angle;
}

void GameObject::Collide(GameObject* object) {
    // Don't allow collisions with collectibles by default
    if (object->GetType() != "Collectible") {
        // lower hitpoints
        hitpoints_--;
        // change state to 'destroyed' if necessary
        if (hitpoints_ <= 0 && !is_destroyed_) {
            is_destroyed_ = true;
            // set timer for duration until object deletion
            timer_.Start(5.0f);
        }
    }
}

void GameObject::Update(double delta_time) {

}


void GameObject::Render(glm::mat4 view_matrix, double current_time){

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Set the ghost property in the fragment shader
    shader_->SetUniform1i("ghost", ghost_);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
