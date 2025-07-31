Note that this project contains code from the COMP2501 demos: 

Copyright (c) 2020-2025 Oliver van Kaick <Oliver.vanKaick@carleton.ca>, David Mould <mould@scs.carleton.ca>
under the MIT License. 
Refer to the "licenses.txt" file for more information.

We developed the final project on Windows.

Group members:
	- Teddy Al Houwayek
	- Atan Dogan Hoca
	
---------------------
----- Controls ------
---------------------
W - Forward Movement		|        A - Rotate to the left
S - Backward Movement       |        D - Rotate to the right
Q - Move to the left        |        E - Move to the right

F - Fire projectiles        |      Left Shift - Fire shockwave/pulse

Space Bar - Boost (Hold down)

How to play:
Your objective is to defeat the boss that has surrounded you as you get invaded by waves of enemies. There will be collectibles that appear and help you survive.
Good Luck!


--------------------
-- Game Mechanics --
--------------------
- The player must fight enemies while attempting to defeat a boss.
- The boss spawns the 3 types of enemies which attempt to defeat the player.
- The player wins once the boss dies.


-------------
-- Enemies --
-------------
Boomers:
- They try to chase the player following the "Chase" mechanic and collide with them dealing 1 damage and sacrificing themselves (a form of melee weapon).

Dreadnoughts: 
- They move in parametric motion following an elliptic trajectory and release a shockwave/pulse regularly which pushes away any player and enemy entity,
as well as deals 1 damage.

Fighters: 
- They have a state machine that allows for "Wandering", "In pursuit", and "Chase". When the player is too far, they wander around the map randomly picking points to go to.
- Once the player gets close enough they switch to being "in pursuit" at which point they start firing projectiles at the player's expected/predicted future position.
They also attempt to move closer to the player as they shoot.
- Finally, if even closer, they will enter "Chase" mode during which they speed up to ram into the player whilst also firing projectiles. They will target the player directly
at that point without predicting where they'll go.
- Each projectile deals 1 damage, and colliding with the player blowing themselves up also deals 1 damage.

All 3 enemies have different textures and so do their weapons.


-------------
-- Weapons --
-------------
- Weapons have different sprites.
- The player can fire a projectile that uses ray circle collision to ensure contact with enemies. It will deal 1 damage.
- The player also has a shockwave/pulse attack which functions identically to the dreadnought. Circle-to-Circle collision dealing 1 damage to each enemy touched.


------------------
-- Collectibles --
------------------
- One collectible replenishes hp by 2 points. Players have a maximum of 14 hp.
- Another replenishes fuel by 25 points. Fuel is used to trigger the booster, capacity is 100 fuel.
- The third, when you collect 5 of them, will allow you to automatically enter an invincible state for 10 seconds during which nothing can harm you.


----------------------------------
-- Movement and Transformations --
----------------------------------
- All Movement is handled/applied in Render functions using transformations.
- The player, fighter, and boomer have physics based movement.
- The dreadnought has parametric based movement.


-------------------------
-- Collision detection --
-------------------------
- There exists Collision Detection with circle-to-circle implementation, and ray-circle implementation.
- There is various collision detection behaviour between collectibles, players, enemies, walls, and planets.


----------------
-- Game World --
----------------
- The screen is continuously scrolling upward.
- The game's background uses texture tiling with a range of 100x100 without stretching.


----------------------
-- Particle Systems --
----------------------
- Particle Systems are used to create a booster effect when the player holds down spacebar (boost button). 
- Particle Systems are used to create an explosion animation when the boss is killed.


---------------------------------------------------------------------
-- Hierarchical Transformation of a Chain of at Least Three Linkes --
---------------------------------------------------------------------
- Created heirarchical transformation header and cpp which takes 4 game objects to create a heirarchical transformation object of 3 links.
- It just spins each sub-object in a circle to act as a spinning object.
- We were going to use it as an obstacle which knocks players and enemies around but we never got around to it.


--------
-- UI --
--------
- We have health bars for the boss and player.
- We also have a bar for the player's fuel tank. 
- And a timer for how long the game has lasted.
- All UI elements have fixed positions on screen (will remain in place as the player and camera move to remain consistent).


---------------------
-- Advanced Method --
---------------------
- We implemented advanced steering behaviors. One of the enemy types (Fighter) has a state machine that replicates wandering, pursuit, and chasing appropriately.


-----------
-- Bonus --
-----------
- Special collision resolution: When colliding with walls, all entities receive knockback and end up bouncing off of them.
This also includes the player being forced to bounce off enemy shockwave attacks.

- Player and certain enemies get acted on by gravity using formula: (GravityConstant*Radius/DistanceToPlanet)*delta_time.
When orbiting a planet close by, the game also uses rotation based movement where player/boomer velocity vectors are rotated for smoother movement (can be found in celestial body).
To do this we take the vector from the planet to the player/boomer, get the left or right side vector depending on if we are rotating counter-clockwise or clockwise around the planet, 
then nudge the player/boomer velocity vector towards the left or right side vector (which is a tangent to the circle at that point).
This allows the player/boomer to rotate their velocity vector towards the tangent of the circle at player/boomer position allowing for rotation based movement.

- Boomers can chase players when orbiting a planet by adjusting their height to the planet to match the player's height while using the rotation based movement explained above.
Otherwise, if boomer's are not close to a planet, they will simply set the player as a target and move towards it.
The player and enemies can bounce of walls of the game.


--------------
--- Assets ---
--------------


All files in "/Powerups/*" folder
Source: https://gisha.itch.io/2d-space-game-pack
License: Creative Commons CC0: "No right reserved". Attribution is not required, but highly appreciated.
Creator: gishadev

"stars.png"
Source:  https://opengameart.org/content/stars-parallax-backgrounds
License: This work is released to the Public Domain / CC0 license.
Creator: Bonsaiheldin

"explosion.png"
Source:  https://opengameart.org/content/pixel-explosion-12-frames
License: CC BY 3.0 license.
Creator: JROB774

"projectile_player.png"
Source: https://gisha.itch.io/2d-space-game-pack
License: Creative Commons CC0: "No right reserved". Attribution is not required, but highly appreciated.
Creator: gishadev

"projectile_enemy.png"
Source: https://gisha.itch.io/2d-space-game-pack
License: Creative Commons CC0: "No right reserved". Attribution is not required, but highly appreciated.
Creator: gishadev

"pulse.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki

"barrier.png"
Source:  https://opengameart.org/content/laser-gate
License: This work is released to the Public Domain / CC0 license.
Creator: Fleurman

"mothership.png"
Source:  https://guardian5.itch.io/spaceship-asset
License: Author specified "This assets is for personal and commercial use. The asset can be edited. This asset cannot be resold or distributed."
Creator: Guardian

"player.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki

"invincible.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki (note that we slightly modified the color scheme)

"dreadnought.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki

"boomer.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki

"fighter.png"
Source:  https://zintoki.itch.io/space-breaker
License: CC0 1.0 license.
Creator: zintoki

The other textures "bar.png", "empty.png", "orb.png" and "font.png" were created by us using Paint3D or taken from previous demos.
