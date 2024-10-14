the little game of pool
your job is to aim (using the crosshair '+') and hit the * ball
 with the o ball, with an appropriate force, in order
 to push it into one of the 6 available pockets...
the balls will bounce back when hitting the rim,
 and will collide according to the laws of physics
also, friction will slow their movement speed

Fixed point (8.8) arithmetic is used to optimize execution time.
Also, some geometry formulas are used when managing the balls interaction (collision), to compute:
- the movement of the balls
- the distance from a point to a line ( https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line )
- the balls elastic collision parameters ( https://en.wikipedia.org/wiki/Elastic_collision )
