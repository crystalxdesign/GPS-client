echo(version=version());
$fn=50;
boxLength = 120;
boxWidth = 60;
wallheight = 30;
wallthickness = 4;

difference()  {
    union(){
        Walls();
        Rim();
        Base();
        Rim2WallBond();
    };
    UsbPortHole();
    SwitchHole();
}

module Rim2WallBond(){
        linear_extrude(height = wallheight, scale = 1)
            difference(){
                BoxFootprint(boxLength-3,boxWidth-3);
                BoxFootprint(boxLength-8,boxWidth-8);
            }
}

module Rim(){
        linear_extrude(height = wallheight+3, scale = 1)
            difference(){
                BoxFootprint(boxLength-5,boxWidth-5);
                BoxFootprint(boxLength-9,boxWidth-9);
            }
}

module UsbPortHole(){
    translate([-67,0,15])
        rotate([0,90,0])
            rotate([0,0,90])
                linear_extrude(height = 10, scale = 1)
                    offset(r=2.65)
                        square([0.2, 8.3], center = true);
}


module SwitchHole(){
    translate([62,0,15])
        rotate([0,90,0])
            linear_extrude(height = 10, center = true)
                circle(d=15);
}

module Walls(){
    linear_extrude(height = wallheight, scale = 1)
        difference()  {
            BoxFootprint(boxLength,boxWidth);
            BoxFootprint(boxLength-wallthickness, boxWidth-wallthickness);
        }
}

module Base(){
    translate([0,0,-2])
        linear_extrude(height = 3, scale = 1)
            BoxFootprint(boxLength,boxWidth);
}

module BoxFootprint(x, y){
    offset(r=wallthickness)
        square([x, y], center = true);
}
