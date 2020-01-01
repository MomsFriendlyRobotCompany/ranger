$fn=90;

m3 = 3.3;

// https://www.engineersedge.com/hardware/standard_metric_hex_nuts_13728.htm
// m2: d=4    D=4.62
// m3: d=5.5  D=6.35
module hex(D,t){
    x = D/2;
    y = sqrt(3)/2*x;
    pts = [
        [x/2,y],
        [x,0],
        [x/2,-y],
        [-x/2,-y],
        [-x,0],
        [-x/2,y]
    ];
    linear_extrude(height=t){
        polygon(pts);
    }
}

module M2(t){
    cylinder(t, d=2.2, center=true);
}

module M3(t){
    cylinder(h=t, d=3.3, center=true);
}


module M3NutVertical(t){
    hex(6.4, t);
    cylinder(h=2*t, d=3.3, center=true);
}

module M3Nut(t, flip=false){
    /* hex(6.4, t); */
    if (flip) {
        hex(6.4, t);
        translate([0,0,-t]) cylinder(t+1, d=3.3);
    }
    else {
        translate([0,0,-t]) hex(6.4, t);
        translate([0,0,-1]) cylinder(t+1, d=3.3);
    }
}

module M2Nut(t, flip=false){
    hex(4.65, t);
    if (flip) translate([0,0,-t]) cylinder(t+1, d=2.2);
    else translate([0,0,t]) cylinder(t, d=2.2);
}

module M2Hean(t){
    cylinder(t, d=2.2, center=true);
    translate([0,0,2]) cylinder(h=t, d=7); // head
}

module M3Head(t){
    translate([0,0,-t]) cylinder(h=t+1, d=3.3, center=false);
    cylinder(h=t, d=7); // head
}

//////////////////////////////////////////////////////////////////////////

// RPI deck
module turret(){
    color("blue") rotate([0,180,90]) import("lib/turret.stl");
}

///////////////////////////////////////////////////////////////////////////

/* module skidplatebolts(){
    // M3head(10);
    for (x = [0:30:90]) translate([x,0,0]) M3Head(10);
} */

module skidplate(){
    w = 7;
    h = 7;
    l = 100;

    difference()
    {
        cube([l, w, h]);
        translate([l/2 - 90/2, 2*w/3, h/2]) rotate([90,0,0])
            for (x = [0:30:90]) translate([x,0,0]) M3Head(10);
    }
    /* translate([0,102+w,0]) cube([110, w, h]);
    translate([l/2 - 90/2, 102+w+1*w/3, h/2]) rotate([-90,0,0]) skidplatebolts(); */
}

module bumper(th, sf=0.3){
    difference(){
        translate([0,109/2,0]) scale([sf,1,1]) cylinder(d=102+7, h=th);
        translate([0,109/2,-2]) scale([sf-.05,1,1]) cylinder(d=102+7-14, h=th+3);
        translate([0,-2, -2]) cube([20, 102+7+5, 20]);
    }
}

module frameside(){
    w = 7;
    h = 7;
    difference()
    {
        dia = 10;
        union(){
            cube([200, w, h]); // main bar
            translate([0,0,dia/2]) rotate([-90,0,0]) cylinder(d=10, h=102+7); // shocks
            bumper(dia);
        }
        // shock screws
        translate([0,-2,dia/2]) rotate([-90,0,0]) cylinder(d=2.5, h=130);
        translate([200,-2,dia/2]) rotate([-90,0,0]) cylinder(d=2.5, h=130);
        // top-down bolts
        for (x = [10,35,75,100,125,200-35, 190]) translate([x,7/2,2]) M3Nut(6, true); // mount pts on main bar
    }

    // shocks and bumper
    /* difference(){
        dia = 10;
        union(){
            translate([0,0,dia/2]) rotate([-90,0,0]) cylinder(d=10, h=102+7);
            bumper(dia);
        }
        translate([0,-2,dia/2]) rotate([-90,0,0]) cylinder(d=2.5, h=130);
    } */
    // front straight part of A frame to skidplate
    /* difference()
    {
        translate([51, 0,0]) cube([w, w, 65]);
        translate([51+7/2, 7/2, 65/2]) rotate([-90,0,0]) M3Nut(7, true);
    } */

    // back straight part of A frame to skidplate
    /* difference()
    {
    translate([200-51-7, 0,0]) cube([w, w, 65]);
    translate([200-51-7/2, 7/2, 65/2]) rotate([-90,0,0]) M3Nut(7, true);
    } */

    /* translate([0,50,0]) M3Nut(7); */

    // cross bar to other side, top down
    difference()
    {
        translate([52, 0, 0]) cube([7, 102+7, 7]);
        for (x = [1:3]) translate([52+7/2,x*109/2/2,2]) M3Nut(6, true);
    }
    /* translate([100-50, 0, 65]) skidplate(); */

    // angled part of A frame
    /* pa = 60; // plate bar anles
    translate([15+sqrt(49)*sin(pa),0,h-sqrt(49)*cos(pa)]) rotate([0,-pa,0]) cube([70, w, h]);
    translate([200-15,0,h]) rotate([0,-(90-pa)-90,0]) cube([70, w, h]); */

    // front curved bumper
    /* sf = 0.3;
    difference(){
        translate([0,109/2,0]) scale([sf,1,1]) cylinder(d=102+7, h=7);
        translate([0,109/2,-2]) scale([sf-.05,1,1]) cylinder(d=102+7-14, h=10);
        translate([0,-2, -2]) cube([20, 102+7+5, 20]);
    } */
}

module frame(){
    w = 7;
    h = 7;
    /* cube([200, w, h]); */
    frameside();
    translate([200,102+w,0]) rotate([0,0,180]) frameside();

}

frame();

//translate([100, 109/2, -5]) turret();
