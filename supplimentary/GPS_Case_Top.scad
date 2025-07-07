echo(version=version());

$fn=50;
isRed = true;
useScrewHoles = false;
screenWidth = isRed ? 37 : 34;
screenLength = isRed ? 40 : 40;
screenHoleOffsetX = isRed ? 0.5 : -2.8;
screenHoleOffsetY = isRed ? 2.0 : 3.5 ;
screwHoleRadius = isRed ? 1.0 : 1.65;

basethickness = 4;
boxLength = 100;
boxWidth = 50;
wallthickness = 4;

corners = [[boxWidth/2,boxLength/2,0],[-boxWidth/2,-boxLength/2,0],[boxWidth/2,-boxLength/2,0],[-boxWidth/2,boxLength/2,0]];

main();

module main(){
    translate([0,0,basethickness])
        rotate([180,0,0])
            union(){
                difference()  {
                    Base();
                    ScreenHole();
                    if (useScrewHoles) { ScrewHoles();}
                    AerialHole();
                    Hollow();
                 };
                 #Rim();
             }
}

module ScreenHole(){
    linear_extrude(height = basethickness+2)
        translate([0,0,-2])
            square([screenLength, screenWidth], center = true);
}

module AerialHole(){
    linear_extrude(height = basethickness+2)
        translate([0,-50,-2])
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
    translate([0,-25,0])
        difference()  {
            hull(){
                for (a = corners)
                    translate([a[0],a[1],a[2]]) 
                        sphere(wallthickness);
            };
       translate([0,0,-wallthickness])
            cube([boxWidth+10, boxLength+10, 10], center = true);
    }
}

module BoxFootprint(x, y){
    offset(r=wallthickness)
        square([x, y], center = true);
}

module Rim(){
    translate([0,-25,-2])
        linear_extrude(height = 5, scale = 1)
            difference()  {
                BoxFootprint(boxWidth-4, boxLength-4);
                BoxFootprint(boxWidth-6, boxLength-6);
            }
}

module Hollow(){
    linear_extrude(height = 2, scale = 1)
        translate([0,-25,2])
            BoxFootprint(boxWidth-8, boxLength-8);
}