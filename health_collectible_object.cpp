
#include "health_collectible_object.h"

namespace game {

	/*
		HealthCollectibleObject inherits from CollectibleGameObject
		It overrides CollectibleGameObject's constructor to set a custom sub_type
	*/
	HealthCollectibleObject::HealthCollectibleObject(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint texture)
		: CollectibleGameObject(position, geom, shader, texture) {

		// Overrite sub_type_
		sub_type_ = "Health";
	}

} // namespace game