#include <glm/gtc/matrix_transform.hpp>

#include "drawing_game_object.h"

namespace game {

DrawingGameObject::DrawingGameObject(const glm::vec3 &position, Geometry *geom, Shader *shader, GLuint texture) 
    : GameObject(position, geom, shader, texture) {
       
    type_ = "UI";
    is_invincible_ = true;
}

float DrawingGameObject::GetFillValue(void) const {
    return fill_value_;
}

void DrawingGameObject::SetFillValue(float fill_val) {
    // Clamp the fill value between 0.0 and 1.0 (easier to deal with in fragment shader then)
    if (fill_val < 0.0f) {
        fill_val = 0.0f;
    }
    else if (fill_val > 1.0f) {
        fill_val = 1.0f;
    }
    fill_value_ = fill_val;
}

void DrawingGameObject::SetFillColor(const glm::vec4& color) {
    fill_color_ = color;
}

void DrawingGameObject::Render(glm::mat4 view_matrix, glm::mat4 view_matrix_fixed, double current_time) {

    // Set up the shader
    shader_->Enable();

    // Set up the view matrix
    shader_->SetUniformMat4("view_matrix", view_matrix_fixed);

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

    // Set the fill value in the shader
    shader_->SetUniform1f("fill_val", fill_value_);

    // Set the color value in the shader
    shader_->SetUniform4f("color", fill_color_);

    // Set up the geometry
    geometry_->SetGeometry(shader_->GetShaderProgram());

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, geometry_->GetSize(), GL_UNSIGNED_INT, 0);
}

void DrawingGameObject::Collide(GameObject* other) {
    // Do nothing
}

} // namespace game
