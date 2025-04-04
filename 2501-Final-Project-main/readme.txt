We developed the final project on Windows.

Group members list ?
////// List all implemented aspects and where to find them //////

------------
-- PART 1 --
------------

Added 'velocity_', 'acceleration_' and 'max_velocity_' attributes to "player_game_object.h".
And initialized them in "player_game_object.cpp".

Modified the 'Update' function in "player_game_object.cpp" to properly modify player position using velocity.

Created 'GetVelocity', 'GetAcceleration' and 'SetVelocity' methods in "player_game_object.h".

Modified 'HandleControls' method in "game.cpp" to use our new methods for physics-based player movement.

------------------
-- PART 2 and 3 --
------------------

Added a 'bullet.png' image for projectiles into to the 'textures' folder of the project.
Source:
https://opengameart.org/content/bullet-symbol
License:
Public domain

Added "projectile.h" and "projectile.cpp" files, and modified "CMakeLists.txt" accordingly.
Implemented the projectile class appropriately as requested in the assignment specification.

Modified 'SetupGameWorld' method in "game.cpp" to add projectile texture.

Added a 'shooting_cooldown_' attribute to "player_game_object.h", and initialized it in "player_game_object.cpp".
Added a 'Fire' method in "player_game_object.h" to handle projectile creation/cooldown, and implemented it in "player_game_object.cpp".

Added 'include "projectile.h"' statement to "game.cpp".
Modified 'HandleControls' method in "game.cpp" so that player fires projectile when pressing the 'F' key (if the shooting cooldown of 1s is complete).

------------
-- PART 4 --
------------

Already implented from previous assignment.

------------
-- PART 5 --
------------

Added 'RayCircleCollision' method in "game.h" and implemented it in "game.cpp".

Modified 'Update' method of "game.cpp" to call for 'RayCircleCollision' when Projectiles are involved, and uses Circle-Circle detection otherwise.

Made a 'GetSpeed' method in "projectile.h".

------------
-- PART 6 --
------------

Modified the 'Render' method in "game.cpp" to make the camera move with the player object.

Modified the 'SetTexture' method of "game.cpp" to use GL_REPEAT for the texture wrapping.

Added a 'texture_scale_' attribute to "Sprite.h", and initialized it in "Sprite.cpp".
Made an alternative Constructor for "Sprite.h" and implemented it in "Sprite.cpp".

Modified the 'CreateGeometry' method of "Sprite.cpp" to make use of the 'texture_scale_' attribute.

Modified the 'SetupGameWorld' method of "game.cpp" to: 
	- Create an alternative Sprite for the background game object.
	- Change the scalar used for the background.

------------
-- PART 7 --
------------

Modified the game object constructor in "game_object.h" and "game_object.cpp" to take values for x and y scaling with default values given.

Modified the 'scale_' attribute in "game_object.h" to be a vector of 2 elements, and changed its initialization in "game_object.cpp".
Made the appriopriate changes for 'GetScale' and 'SetScale' methods in "game_object.h" accordingly.

Modified the 'SetupGameWorld' method of "game.cpp" to adjust the background 'SetScale' usage to our new one.

Modified the 'Render' method in "game_object.cpp" to send the corresponding scaling matrix in the vertex shader.

Modified the 'SpawnCollectible' method in "game.cpp" to make the collectibles twice as long vertically, but the same horizontally.

------------
-- PART 8 --
------------

Added 'ghost_' attribute to "game_object.h" and initialized it in "game_object.cpp".

Modified 'Collide' method in "collectible_game_object.cpp" to trigger ghost mode when object picked up.

Modified 'Update' method in "game.cpp" so that collectibles don't get deleted anymore (remain on screen).

Added a 'ghost' property to the fragment shader "sprite_fragment_shader.glsl".

Modified 'Render' method in "game_object.cpp" to pass 'ghost_' attribute into fragment shader as integer variable.

Modified 'main' function in fragment shader "sprite_fragment_shader.glsl" to color in grayscale when necessary.