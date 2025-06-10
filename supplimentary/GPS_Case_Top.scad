echo(version=version());

isRed = false;
screenWidth = isRed ? 37 : 34;
screenLength = isRed ? 40 : 40;
screenHoleOffsetX = isRed ? 1.5 : -2.1;
screenHoleOffsetY = isRed ? 2.4 : 3.1 ;
screwHoleRadius = isRed ? 1.0 : 1.65;

basethickness = 5;
boxLength = 120;
boxWidth = 60;
wallthickness = 4;

main();

module main(){
    translate([0,0,basethickness])
        rotate([180,0,0])
            difference()  {
                Base();
                ScreenHole();
                ScrewHoles();
                AerialHole();
                Rim();
             }
}

module ScreenHole(){
    linear_extrude(height = basethickness+2)
        translate([0,0,-2])
            square([screenLength, screenWidth], center = true);
}

module AerialHole(){
    linear_extrude(height = basethickness+2)
        translate([0,-60,-2])
            offset(r=1.5)
                square([22.5, 22.5], center = true);
}

module ScrewHoles(){
    x = (screenLength/2)+screenHoleOffsetX;
    y = (screenWidth/2)+screenHoleOffsetY;
    ScrewHole(-x,-y);
    ScrewHole(x,-y);
    ScrewHole(-x,y);
    ScrewHole(x,y);
}
  
module ScrewHole(x,y){
     linear_extrude(height = basethickness+2)
        translate([x,y,-2])
            circle(screwHoleRadius,$fn=50);
}

module Base(){
    translate([0,-30,0])
        linear_extrude(height = basethickness, scale = 1)
            BoxFootprint(boxWidth,boxLength);
}

module BoxFootprint(x, y){
    offset(r=wallthickness)
        square([x, y], center = true);
}

module Rim(){
    linear_extrude(height = 3, scale = 1)
        translate([0,-30,2])
            BoxFootprint(boxWidth-5, boxLength-5);
}
