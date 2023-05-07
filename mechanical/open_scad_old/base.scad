$fn=90;

include <screws.scad>

module top(){
    color("blue") rotate([0,180,90]) import("stl/rc-xd-4-v1.stl");
}
module arch(){
    color("blue") translate([60+70,109,0]) rotate([0,0,180]) import("stl/base-v1.stl");
}

//////////////////////////////////////////////////////////////////////////




module rail(offset, w, h, bar){
    /* offset = 7/2; */
    /* offset = 18; */
    difference() {
        cube([bar,w,h]);
        translate([offset, w-4,h/2]) rotate([90,0,0]) M3Head(7);
        translate([30+offset, w-4,h/2]) rotate([90,0,0]) M3Head(7);
    }
}

module bolthole(){
    dia = 7;
    difference(){
        cylinder(d=dia, h=10);
        translate([0,0,dia/2]) M3Nut(10);
    }
}

module frame(){
    w = 7;
    h = 7;
    offset = 13; // how far forward of front screw
    bar = offset + 30 + 15;
    rail(offset, w,h, bar);
    translate([0,102+7+7,7]) rotate([180,0,0]) rail(offset, w,h, bar);
    translate([0,102+7,0]) cube([5,5,5]);

    difference()
    {
        cen = (102+8)/2 + 3;
        union(){
            y = 40;
            x = 16;
            translate([0,cen-y/2,cen-6]) cube([x,y,10]); // mount
            translate([0,cen,7]) rotate([0,90,0]) cylinder(d=102+8, h=bar);

            // side mounting holes square risers
            cube([10,7,28]);
            translate([0,102+7,0]) cube([10,7,28]);

            // front mounting hole semi-circles
            color("blue") translate([5+5,7/2,28]) rotate([0,-90,0]) bolthole();
            color("blue") translate([5+5,102+7+7/2,28]) rotate([0,-90,0]) bolthole();
        }
        // x-axis cylinder, cut center out
        translate([-1,cen,7]) rotate([0,90,0]) cylinder(d=102, h=bar+4);

        // cube, remove bottom half
        translate([-1, -1, -60]) cube([bar+5, 120, 60+7]);

        // y-axis cylinder
        dia = 94;
        translate([dia/2+14,(102+14),dia/2+7+4+2]) rotate([90,0,0]) cylinder(d=dia, h=102+14);

        // topdeck mounting holes
        translate([5, cen+15, 102/2+4+4]) M3Nut(10);  // 7 max, come down 3mm
        translate([5, cen-15, 102/2+4+4]) M3Nut(10);  // 1 is min, go up

        // side mounting hole
        translate([5, 4, 21]) rotate([90,0,0]) M3Nut(10);
        translate([5, 102+14-4, 21]) rotate([-90,0,0]) M3Nut(10);

        // front mounting hole
        translate([5+5,7/2,28]) rotate([0,-90,0]) translate([0,0,10/2]) M3Nut(10);
        translate([5+5,102+7+7/2,28]) rotate([0,-90,0]) translate([0,0,10/2]) M3Nut(10);
    }
    /* color("blue") translate([5+5,7/2,28]) rotate([0,-90,0]) bolthole(); */

    /* translate([23, 7/2, 0]) cylinder(d=7, h=50); */

    /* color("blue") translate([0, (102+7)/2, 0]) cylinder(d=7, h=55); */
}

/* translate([-35,109,65]) rotate([0,0,90]) top(); */
/* arch(); */
frame();
