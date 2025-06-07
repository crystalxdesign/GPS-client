echo(version=version());
$fn=50;
boxLength = 120;
boxWidth = 60;
wallheight = 30;

union(){
    Walls();
    Rim();
    Base();
}

module Rim(){
    translate([0,0,wallheight])
        linear_extrude(height = 3, scale = 1)
            difference(){
                BoxFootprint(boxLength-5,boxWidth-5);
                BoxFootprint(boxLength-10,boxWidth-10);
            }
}

module Walls(){
    linear_extrude(height = wallheight, scale = 1)
        difference()  {
            BoxFootprint(boxLength,boxWidth);
            BoxFootprint(boxLength-10,boxWidth-10);
        }
}

module Base(){
    translate([0,0,-2])
        linear_extrude(height = 3, scale = 1)
            BoxFootprint(boxLength,boxWidth);
}

module BoxFootprint(x, y){
    offset(r=10)
        square([x, y], center = true);
}
