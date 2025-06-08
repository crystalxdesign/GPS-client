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
    };
    UsbPortHole();
    SwitchHole();
}

module Rim(){
    translate([0,0,wallheight])
        linear_extrude(height = 3, scale = 1)
            difference(){
                BoxFootprint(boxLength-5,boxWidth-5);
                BoxFootprint(boxLength-10,boxWidth-10);
            }
}

module UsbPortHole(){
    translate([-67,0,15])
        rotate([0,90,0])
            rotate([0,0,90])
                linear_extrude(height = 10, scale = 1)
                    offset(r=2.6)
                        square([0.2, 7.0], center = true);
}


module SwitchHole(){
    translate([62,0,15])
        rotate([0,90,0])                        linear_extrude(height = 10, center = true)
                circle(d=14.5);
}

module Walls(){
    linear_extrude(height = wallheight, scale = 1)
        difference()  {
            BoxFootprint(boxLength,boxWidth);
            BoxFootprint(boxLength-wallthickness,boxWidth-wallthickness);
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
