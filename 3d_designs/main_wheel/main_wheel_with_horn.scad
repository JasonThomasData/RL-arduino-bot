translate([-0.1,10.63,-0.67])
    rotate([90,0,0])
        import("../reused_parts/SG90-A5.stl");

import("../reused_parts/main_wheel.stl");

module wheelHornJoin()
    linear_extrude(height=4.66)
        difference()
        {
            circle(r=5, $fn=50);
            circle(r=3, $fn=20);
        }
 
translate([0,0,-3])
    wheelHornJoin();