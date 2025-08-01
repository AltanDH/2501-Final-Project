#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.h"

namespace game {

ParticleSystem::ParticleSystem(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture, GameObject *parent)
	: GameObject(position, geom, shader, texture){

    parent_ = parent;

    // Values to avoid having collisions trigger on the particle system
    type_ = "Particle System";
    is_invincible_ = true;
}

void ParticleSystem::Collide(GameObject* other) {
    // Does nothing (no collisions should happen here)
}

void ParticleSystem::Update(double delta_time) {

    // Call the parent's update method to move the object in standard way, if desired
    GameObject::Update(delta_time);
}


void ParticleSystem::Render(glm::mat4 view_matrix, glm::mat4 view_matrix_fixed, double current_time){

    // If a player is loading the particle system (for a boost effect)
    if (parent_->GetType() == "Player" && type_ == "Booster") {
        // Check if the conditions are met (if player not boosting or doesn't have enough fuel, cancel rendering the particle system)
        if (!(((PlayerGameObject*)parent_)->isBoosting()) || ((PlayerGameObject*)parent_)->GetFuel() <= 0) {
            return;
        }
    }

    // If a player is loading the particle system (for a pulse effect)
    if (parent_->GetType() == "Mothership" && type_ == "Explosion") {
        // Check if the conditions are met (if player triggered firing condition, cancel rendering the particle system if not)
        if (!parent_->isDestroyed()) {
            return;
        }
    }

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_.x, scale_.y, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angle_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // Set up the parent transformation matrix
    glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
    glm::mat4 parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix * scaling_matrix;

    // Set the transformation matrix in the shader
    shader_->SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set the time in the shader
    shader_->SetUniform1f("time", current_time);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the particle texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
