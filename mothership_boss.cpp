#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "mothership_boss.h"

namespace game {

	// Constructor
	Mothership::Mothership(const glm::vec3& position, Geometry* geom, Shader* shader, GLuint* textures, GameObject* player)
		: EnemyGameObject(position, geom, shader, textures[2]) {

		// Mothership specifics
		type_ = "Mothership";
		hitpoints_ = 30;

		// Variables to define boss zone
		width_ = 30;
		height_ = 30;
		player_ = player;

		position_.x = player_->GetPosition().x;
		position_.y = player_->GetPosition().y + height_/2;

		// Trackers for mothership movement
		direction_ = glm::vec3(0.0f, 0.8f, 0.0f);
		speed_ = 0.1f;

		// References to textures
		tex_ = textures;

		// Timer for shifting directions
		shift_cooldown_ = Timer();
		shift_cooldown_.Start(10.0f);

		// Initialize timer to spawn enemies
		enemy_spawn_timer_ = Timer();
		enemy_spawn_timer_.Start(10.0f);
		total_enemy_count_ = 0;

		// Increase scale for grandeur
		GameObject::SetScale(3.0f);
	}

	// Loads in the barriers for the boss area
	void Mothership::LoadBarriers(void) {
		int min_x = position_.x - width_/2;
		int min_y = position_.y;
		float pi_over_two = glm::pi<float>() / 2.0f;

		// Setup barriers on Top Layer (left side of Boss)
		for (int i = position_.x - width_ / 2 - 1; i < position_.x - 1; i++) {
			BossBarrier* barrier = new BossBarrier(glm::vec3(i, min_y, 0.0f), geometry_, shader_, tex_[3], this);
			game_objects_ref_->push_back(barrier);
		}
		// Setup barriers on Top Layer (right side of Boss)
		for (int i = position_.x + 2; i < position_.x + 2 + width_/2; i++) {
			BossBarrier* barrier = new BossBarrier(glm::vec3(i, min_y, 0.0f), geometry_, shader_, tex_[3], this);
			game_objects_ref_->push_back(barrier);
		}

		// Setup barriers on Left Side (Top to Bottom)
		for (int i = min_y - 1; i > position_.y - height_; i--) {
			BossBarrier* barrier = new BossBarrier(glm::vec3(min_x - 1, i, 0.0f), geometry_, shader_, tex_[3], this);
			// Set barrier rotation
			barrier->SetRotation(pi_over_two);
			game_objects_ref_->push_back(barrier);
		}

		// Setup barriers on Right Side (Top to Bottom)
		for (int i = min_y - 1; i > position_.y - height_; i--) {
			BossBarrier* barrier = new BossBarrier(glm::vec3(min_x + 1 + width_, i, 0.0f), geometry_, shader_, tex_[3], this);
			// Set barrier rotation
			barrier->SetRotation(-pi_over_two);
			game_objects_ref_->push_back(barrier);
		}

		// Setup barriers on Bottom Layer
		for (int i = min_x - 1.5; i < min_x + 2 + width_; i++) {
			BossBarrier* barrier = new BossBarrier(glm::vec3(i, min_y - height_, 0.0f), geometry_, shader_, tex_[3], this);
			game_objects_ref_->push_back(barrier);
		}
	}

	// Spawns an Enemy
	void Mothership::SpawnEnemy(void) {

		// Create random spawn position within boss area
		glm::vec3 spawn_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		spawn_pos.x = position_.x + 3 - width_/2 + rand() % (width_ - 3);
		spawn_pos.y = position_.y - 3 - rand() % (height_ + 3);

		// Create a new enemy chosen at random
		int rand_choice = rand() % 3;
		EnemyGameObject* new_enemy;

		if (rand_choice == 0) {
			new_enemy = new BoomerEnemy(spawn_pos, geometry_, shader_, tex_[4], player_, this);
		}
		else if (rand_choice == 1) {
			new_enemy = new DreadnoughtEnemy(spawn_pos, geometry_, shader_, tex_[5], tex_[10], this);
		}
		else {
			new_enemy = new FighterEnemy(spawn_pos, geometry_, shader_, tex_[6], tex_[9], (PlayerGameObject*)player_, this);
		}

		// Set enemy rotation
		float pi_over_two = glm::pi<float>() / 2.0f;
		new_enemy->SetRotation(pi_over_two);
		// Provide the game objects list pointer (just in case the chosen enemy will need it)
		new_enemy->SetGameObjectsRef(game_objects_ref_);

		// Add the new enemy to the game objects list (we insert it such that it gets added right before the background)
		game_objects_ref_->insert(game_objects_ref_->end() - 1, new_enemy);
		// Increase total enemy count within boss area
		total_enemy_count_++;
	}

	// Override collide method for custom behavior
	void Mothership::Collide(GameObject* other) {
		// Trigger normal collision if Player rams into Boss
		if (other->GetType() == "Player" && !other->isDestroyed()) {
			hitpoints_--;

			if (hitpoints_ <= 0 && !is_destroyed_) {
				is_destroyed_ = true;
				timer_.Start(2.0f);
			}
		}

		// All other collisions triggered from Projectiles and Pulses are handled on their side
	}

	// Override update method for custom behavior
	void Mothership::Update(double delta_time) {

		// Check if enemy needs to be spawned
		if (enemy_spawn_timer_.Finished() && total_enemy_count_ <= 5) {
			// Spawn a new enemy
			SpawnEnemy();

			// Restart the timer to spawn the next enemy after a randomly generated value between [2, 8] seconds
			enemy_spawn_timer_.Start(6.0f * ((float)rand()) / ((float)RAND_MAX) + 2.0f);
		}

		// Keep mothership in constant movement until destroyed
		if (!is_destroyed_) {

			// Euler integration
			// Check if its time to recalculate desired/target direction
			if (shift_cooldown_.Finished()) {
				// Generates a random value for direction.x between [-1, 1]
				direction_.x = 2.0f * ((float)rand()) / ((float)RAND_MAX) - 1.0f;
				// Generates a random value for the next timer between [0, 8]
				shift_cooldown_.Start(8.0f * ((float)rand()) / ((float)RAND_MAX));
			}

			// Calculate resulting velocity
			glm::vec3 velocity = glm::normalize(direction_ * speed_);
			// Apply it to the current position
			position_ += velocity * (float)delta_time;
		}
	}

} // namespace game