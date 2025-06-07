echo(version=version());

screenWidth = 37;
screenLength = 40;
basethickness = 4;
boxLength = 120;
boxWidth = 60;

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
                square([22, 22], center = true);
}

module ScrewHoles(){
    x = (screenLength/2)+1.5;
    y = (screenWidth/2)+2.4;
    ScrewHole(-x,-y);
    ScrewHole(x,-y);
    ScrewHole(-x,y);
    ScrewHole(x,y);
}
  
module ScrewHole(x,y){
     linear_extrude(height = basethickness+2)
        translate([x,y,-2])
            circle(1,$fn=50);
}

module Base(){
    translate([0,-30,0])
        linear_extrude(height = basethickness, scale = 1)
            BoxFootprint(boxWidth,boxLength);
}

module BoxFootprint(x, y){
    offset(r=10)
        square([x, y], center = true);
}

module Rim(){

    linear_extrude(height = 2, scale = 1)
        translate([0,-30,2])
            BoxFootprint(boxWidth-5, boxLength-5);
}