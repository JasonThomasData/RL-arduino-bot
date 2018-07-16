use <../common.scad>;

module servo_holder_block()
    linear_extrude(height=5)
    {
        square([12,5]);
    }

module servo_holder_with_hole()
    rotate([0,-90,0])
        difference()
        {
            servo_holder_block();
            translate([6,2.5,0])
                screw_hole(1,5);
         }

module servo_holder_pair()
    rotate([0,0,-90])
    {
        translate([0,0,3])
            servo_holder_with_hole();
        translate([0,28,3])
            servo_holder_with_hole();
    }

module chassis_base_holes()
{
    //screw holes
    translate([6, 3, 0])
        screw_hole(1.6,3);
    translate([6, 9, 0])
        screw_hole(1.6,3);
    translate([6, 55, 0])
        screw_hole(1.6,3);
    translate([6, 61, 0])
        screw_hole(1.6,3);
    translate([87,32,0])
            screw_hole(1.6,3);

    //wire holes
    translate([2, 18, 0])
        screw_hole(4,3);
    translate([2, 46, 0])
        screw_hole(4,3);
}

module chassis_base()
    difference()
    {
        linear_extrude(height = 3)
            square([93,64]);
        chassis_base_holes();
    }

translate([0,0,0])
    chassis_base();
translate([15,0,0])
    servo_holder_pair();
translate([15,59,0])
    servo_holder_pair();
