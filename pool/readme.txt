The little game of pool

Your job is to aim (using '+') and hit the four black (*) balls
 with the white (o) ball, with an appropriate force, in order
 to push them into one of the 6 available pockets...

A ball hit with a force equal to N will travel, before stopping,
 a distance equal to 5 x N units of measure;
 (the pool table has 64 x 48 units of measure)
The balls will bounce back when hitting the rim,
 and will collide according to the laws of physics
Due to multiple collisions, several balls might move at the same time
Also, friction will stop eventually the balls movement...

Please extend the size of the screen to at least 48 rows x 64 columns
Hit any key to start:


Fixed point (8.8) arithmetic is used to optimize execution time.
Also, some geometry formulas are used when managing the balls interaction (collision), to compute:
- the movement of the balls
- the distance from a point to a line ( https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line )
- the balls elastic collision parameters ( https://en.wikipedia.org/wiki/Elastic_collision )

If there are any extra arguments specified in the command line, the game starts in 'training' mode, 
so that you can practice hitting repeatedly the ball, from the same starting position, using different aiming points, to see the different resulting trajectories.

POOL30.HEX is a customized version, for those of you who have Geoff's VT100 Terminal Kit ( https://www.tindie.com/products/petrohi/geoffs-vt100-terminal-kit/ ).
