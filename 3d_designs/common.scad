module screw_hole(radius, h)
    linear_extrude(height=h)
    {
        circle(r=radius, $fn = 20);
    }
