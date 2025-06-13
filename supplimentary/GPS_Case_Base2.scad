echo(version=version());
$fn=50;
boxLength = 100;
boxWidth = 50;
wallheight = 30;
wallthickness = 4;
boxCornerCurveRadius = wallthickness;
corners = [[0,0,0],[boxWidth,0,0],[0,boxLength,0],[boxWidth,boxLength,0],
[0,0,wallheight],[boxWidth,0,wallheight],[0,boxLength,wallheight],[boxWidth,boxLength,wallheight]];

main();

module main(){
    difference()  {
        translate([-boxWidth,-boxLength/4,boxCornerCurveRadius])
            hull(){
                BoxCorners();
            };
        BoxVoid();
        UsbPortHole();
        SwitchHole();
    }
}

module BoxVoid(){
    translate([0,0,20+wallthickness])
        union(){
            cube([boxLength,boxWidth,40],center = true);
            translate([0,0,15]) 
                cube([boxLength+wallthickness+boxCornerCurveRadius,boxWidth+wallthickness+boxCornerCurveRadius,10],center = true);
            translate([0,0,12]) 
                cube([boxLength+wallthickness,boxWidth+wallthickness,10],center = true);
        }
}

module BoxCorners(){
    for (a = corners)
        translate([a[1],a[0],a[2]]) 
            sphere(boxCornerCurveRadius);
}

module UsbPortHole(){
    translate([-57,0,boxCornerCurveRadius+wallheight/2])
        rotate([0,90,0])
            rotate([0,0,90])
                linear_extrude(height = 10, scale = 1)
                    offset(r=2.65)
                        square([1.2, 9.3], center = true);
}


module SwitchHole(){
    translate([52,0,boxCornerCurveRadius+wallheight/2])
        rotate([0,90,0])
            linear_extrude(height = 10, center = true)
                circle(d=16);
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
