#include "hierarchical_transformation.h"
#include "game_object.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include "mothership_boss.h"
#include <iostream>

namespace game {


	HierarchicalTransformation::HierarchicalTransformation(GameObject* obj1, GameObject* obj2, GameObject* obj3, GameObject* obj4, Mothership* boss, const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: GameObject(position, geom, shader, texture) {
		this->obj1 = obj1;
		this->obj2 = obj2;
		this->obj3 = obj3;
		this->obj4 = obj4;
		this->boss = boss;

		offset = 1.5f;
		angular_speed_ = glm::radians(50.0f); // Degrees
		obj1->SetPosition(position);

		this->SetType("Hierarchy");
		obj1->SetType("Hierarchy");
		obj2->SetType("Hierarchy");
		obj3->SetType("Hierarchy");
		obj4->SetType("Hierarchy");

		obj1->SetRadius(1.0f);
		obj2->SetRadius(1.0f);
		obj3->SetRadius(1.0f);
		obj4->SetRadius(1.0f);


		this->obj1->SetInvincible(true);
		this->obj2->SetInvincible(true);
		this->obj3->SetInvincible(true);
		this->obj4->SetInvincible(true);
	}

	HierarchicalTransformation::~HierarchicalTransformation() { // Since the other objects are not added to game_objetcs_ array in game.cpp, delete these manually
		delete obj1;
		delete obj2;
		delete obj3;
		delete obj4;
	}

	// setters for the positions and rotations 
	void HierarchicalTransformation::SetTransform1(const glm::vec3& pos, float rotRadians) {
		obj1->SetPosition(pos);
		obj1->SetRotation(rotRadians);
	}


	// Update all rotations
	void HierarchicalTransformation::Update(double delta_time) {


		// obj1
		glm::vec3 angle = obj1->GetBearing();
		obj1->SetRotation(atan2(angle.y, angle.x) + angular_speed_ * delta_time);
		obj1->SetPosition(boss->GetPosition() - glm::vec3(0.0f, 2.0f, 0.0f)); // Set the position of this relative to the boss

		// obj2
		angle = obj1->GetBearing();
		obj2->SetRotation(2 * atan2(angle.y, angle.x) + angular_speed_ * delta_time);
		obj2->SetPosition(obj1->GetPosition() + obj1->GetBearing() * offset);

		// obj3
		angle = obj2->GetBearing();
		obj3->SetRotation(3 * atan2(angle.y, angle.x) + angular_speed_ * delta_time);
		obj3->SetPosition(obj2->GetPosition() + obj2->GetBearing() * offset);

		// obj4
		angle = obj3->GetBearing();
		obj4->SetRotation(4 * atan2(angle.y, angle.x) + angular_speed_ * delta_time);
		obj4->SetPosition(obj3->GetPosition() + obj3->GetBearing() * offset);




	}

	void HierarchicalTransformation::Collide(GameObject* other) {}
}

