#ifndef HIERARCHICAL_TRANSFORMATION_H_
#define HIERARCHICAL_TRANSFORMATION_H_

#include "game_object.h"
#include "mothership_boss.h"

namespace game {
	class HierarchicalTransformation : public GameObject { // Takes 4 game objects and creates a heirarchical transformation of 3 links for position and rotation
		private:
			// Apply any position and rotation transformation to object 1 and it will apply them to objects 2,3,4.
			GameObject* obj1;
			GameObject* obj2;
			GameObject* obj3;
			GameObject* obj4;
			Mothership* boss;

			// Distance between each "node" connecting transformations
			float offset;

			// Speed of rotation
			float angular_speed_;

		public:
			HierarchicalTransformation(GameObject* obj1, GameObject* obj2, GameObject* obj3, GameObject* obj4, Mothership* boss, const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture);
			~HierarchicalTransformation();

			void Collide(GameObject* other) override;

			// setters for the local offsets
			void SetTransform1(const glm::vec3& pos, float rotRadians); 

			// Update all rotations
			void Update(double delta_time) override;


	};
}


#endif