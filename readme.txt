We developed the final project on Windows.

Group members list ?
////// List all implemented aspects and where to find them //////

--------------------
-- Game Mechanics --
--------------------
The player must fight enemies while attempting to defeat a boss.
The boss spawns the 3 types of enemies which attempt to defeat the player.
The player wins once the boss dies.

-------------
-- Enemies --
-------------
Boomer's try to chase the player and collide with them.
Dreadnauts moves in parametric motion and releases a pulse around it which pushes any player and enemy entity.
Fighter's attempt to kill the player by shooting them.

-------------
-- Weapons --
-------------
Weapons have different sprites.
Can fire a projectile with ray circle collision.
Has a pulse which functions identically to the dreadnaut.

------------------
-- Collectibles --
------------------
One collectible replenishes hp by 2 points.
Another replenishes fuel by 25 points.
The third, when you collect 5 of them, you enter an invincible state for 10 seconds.

----------------------------------
-- Movement and Transformations --
----------------------------------
Movement is handled in render functions using transformations.
The player, fighter, and boomer have physics based movement.
The dreadnaught has parametric movement.


-------------------------
-- Collision detection --
-------------------------
Collision detection with circle-circle collision, ray-circle collision.
There is various collision detection behaviour between collectibles, players, enemies, walls, and planets.

----------------
-- Game World --
----------------
The screen is continuously scrolling upward.
The game is tiled 100x100.

----------------------
-- Particle Systems --
----------------------
Particles occur when the boss is killed, and the player presses the boost button (spacebar).

---------------------------------------------------------------------
-- Hierarchical Transformation of a Chain of at Least Three Linkes --
---------------------------------------------------------------------
Created heirarchical transformation header and cpp which takes 4 game objects to create a heirarchical transformation object of 3 links.
It just spins each sub-object in a circle to act as a spinning object which kills anything it touches.

--------
-- UI --
--------
We have health bars for the boss, player, and fuel, as well as a timer for how long the game has lasted.

---------------------
-- Advanced Method --
---------------------
The enemies have a wandering, pursuit, and chase.

-----------
-- Bonus --
-----------
Player and certain enemies get acted on by gravity using formula: (GravityConstant*Radius/DistanceToPlanet)*delta_time.
When orbiting a planet close by, the game also uses rotation based movement where player/boomer velocity vectors are rotated for smoother movement (can be found in celestial body).
To do this we take the vector from the planet to the player/boomer, get the left or right side vector depending on if we are rotating counter-clockwise or clockwise around the planet, 
then nudge the player/boomer velocity vector towards the left or right side vector (which is a tangent to the circle at that point).
This allows the player/boomer to rotate their velocity vector towards the tangent of the circle at player/boomer position allowing for rotation based movement.

Boomers can chase players when orbiting a planet by adjusting their height to the planet to match the player's height while using the rotation based movement explained above.
Otherwise, if boomer's are not close to a planet, they will simply set the player as a target and move towards it.
The player and enemies can bounce of walls of the game.

--------------
--- Assets ---
--------------


All files in "/Powerups/*" folder
Source: https://gisha.itch.io/2d-space-game-pack
License: Creative Commons CC0: "No right reserved". Attribution is not required, but highly appreciated.
Creator: gishadev

"beams.png"
Source:  https://opengameart.org/content/lasers-and-beams
License: This work is released to the Public Domain / CC0 license.
Creator: Rawdanitsu

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

"pulse2.png"
Source:  https://opengameart.org/content/lasers-and-beams
License: This work is released to the Public Domain / CC0 license.
Creator: Rawdanitsu

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