use <../common.scad>;

module sensor_holder_block()
    linear_extrude(height=2)
    {
        square([5,5]);
    }
             
module sensor_holder_corner_with_hole(x_offset, y_offset)
    difference()
    {
        sensor_holder_block();
        translate([x_offset,y_offset,0])
            screw_hole(1, 2);
     }

module one_sensor_holder_all_corners()
{
    sensor_holder_corner_with_hole(3, 3); 
    translate([0,21,0])
        sensor_holder_corner_with_hole(3, 2);
    translate([28.5,0,0])
        sensor_holder_corner_with_hole(2, 3);
    translate([28.5,21,0])
        sensor_holder_corner_with_hole(2, 2);
}

module all_sensor_holders()
{
    one_sensor_holder_all_corners();
    translate([0,26,0])
        one_sensor_holder_all_corners();
    translate([0,52,0])
        one_sensor_holder_all_corners();
}

module sensors_mount_outer_brace()
    linear_extrude(height=2)
    {
        square([3,84]);
        translate([36,0,0])
            square([3,84]);
        translate([3,0,0])
            square([33.5,3]);
        translate([3,81,0])
            square([33.5,3]);
    }

module sensors_mount()
{
    sensors_mount_outer_brace();
    translate([3,3,0])
        all_sensor_holders();
}

module sensor_wall(length)
    linear_extrude(height = 15)
    {
        square([length, 2]);
    }

module short_walls()
{
    translate([2,0,0])
        sensor_wall(35);
    translate([2,28,0])
        sensor_wall(35);
    translate([2,54,0])
        sensor_wall(35);
    translate([2,82,0])
        sensor_wall(35);
}

module short_wall_holes()
{
    rotate([-90,0,0])
        translate([6, -4, 0])
            screw_hole(1, 84);
    rotate([-90,0,0])
        translate([33.5, -4, 0])
            screw_hole(1, 84);
}

module long_walls()
{
      translate([2,0,0])
        rotate([0,0,90])
            sensor_wall(84);
    translate([39,0,0])
        rotate([0,0,90])
            sensor_wall(84);
}

module chassis_connector_block()
    linear_extrude(height = 2)
    {
        circle(r=3.5, $fn = 20);
        translate([-5,-3,0])
            square([4, 6]);
    }

module chassis_connector()
    difference()
    {
        chassis_connector_block();
        screw_hole(1.6,2);
    }

module chassis_connectors()
{
    translate([44,10,0])
        chassis_connector();
    translate([44,16,0])
        chassis_connector();
    translate([44,62,0])
        chassis_connector();
    translate([44,68,0])
        chassis_connector();
}

sensors_mount();
difference()
{
    short_walls();
    short_wall_holes();
}
long_walls();
translate([0,3,0])
    chassis_connectors();
