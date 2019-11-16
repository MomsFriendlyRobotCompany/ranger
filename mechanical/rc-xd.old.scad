$fn=90;

m3 = 3.5;

module hex(side,t){
    s = side/2;
    h = sqrt(side*side-s*s);
    pts = [
        [2*s,0],
        [s,h],
        [-s,h],
        [-2*s,0],
        [-s,-h],
        [s,-h]
    ];
    linear_extrude(height=t){
        polygon(pts);
    }
}

module m3h(t){
    dia = 3.5;
    sdia = 8.5;  // counter sink dia
    cylinder(3*t, d=dia, center=true);  // M3
    translate([0,0,2]) cylinder(3*t, d=sdia, center=false);  // screw driver
}

/* translate([65+70,(-102)/2,80]) color("red") cube([23,77,7], center=true); */

/* cube([65,7,7]); */
/* translate([65,0,0]) rotate([0,-45,-45]) cube([65,7,7]); */
/* translate([0,-120-7,0]) {
    cube([65,7,7]);
    translate([35,0,0]) cube([30,7,40]);
} */

translate([0,-102/2,0]) rotate([0,90,0]) {
    difference()
    {
        // half cylinder to frame
        /* length = 65; */
        length = 100;
        cylinder(d=102+14, h=length);
        translate([0,0,-1]) cylinder(d=102, h=length+5);
        translate([0,-100,-10]) cube([100,200,200]);

        // tapper
        rotate([90,0,0]) {
            l = 400;
            translate([0,length,-l/4]) cylinder(d=102, h=l/2);
        }
        // center
        rotate([0,-90,0]) scale([1.5,1,1]) cylinder(d=85, h=100);
/*
        // suspension screw holes
        translate([0,0,5]) rotate([-90,90,0]){
            translate([-100,80,-50]) cylinder(d=3,h=100);
            translate([-15/2-100,80,-50]) cylinder(d=3,h=100);
            translate([-15-100,80,-50]) cylinder(d=3,h=100);
        } */
        // frame screws
        translate([-7/2,-150/2,40]) rotate([-90,90,0]){
            translate([0,0,128]) m3h(5);
            translate([30,0,128]) m3h(5);
            cylinder(d=m3,h=150);
            m3h(6);
            /* translate([-15/2,0,0]) cylinder(d=3,h=150); */
            translate([30,0,0]) cylinder(d=m3,h=150);
            translate([30,0,0]) m3h(6);
        }
    }
    /* rotate([0,-90,0]) cylinder(d=115, h=100); */
}

translate([103,-102/2,80/2+11]) cylinder(d=77,h=7);

translate([0,-102/2,80/2+1]) {
    w = 30;
    /* translate([0,-w/2,0]) cube([110,w,20]); // center bar */

    /* cylinder(d=50,h=10); */

    difference()
    {
        translate([110-10,-77/2,10]) cube([25,77,35]);
        translate([110-15,-57/2,17]) cube([50,57,40]);

        // suspension screw holes
        translate([120,0,40]) rotate([90,0,0]){
            /* translate([0,0,-50]) m3h(4); */
            translate([0,0,-50]) cylinder(d=m3,h=100);
            translate([-15/2-0,0,-50]) cylinder(d=m3,h=100);
            translate([-15-0,0,-50]) cylinder(d=m3,h=100);
        }
    }
}

/* // suspension screw holes
translate([120,-102,80]) rotate([-90,0,0]){
    cylinder(d=3,h=100);
    translate([-15/2,0,0]) cylinder(d=3,h=100);
    translate([-15,0,0]) cylinder(d=3,h=100);
} */

// frame screws
/* translate([10,-130,7/2]) rotate([-90,0,0]){
    cylinder(d=3,h=150);
    translate([30,0,0]) cylinder(d=3,h=150);
} */

/* translate([65,-120-7/2,0]) rotate([0,-45,45]) cube([65,7,7]); */

/* translate([65+70,(-120)/2,80]) cube([23,77,7], center=true); */
/* translate([65+25,(-120+25-7/2),45]){
    cube([56,77,7], center=false);
    translate([33,0,0]) cube([23,10,38]);
    translate([33,67,0]) cube([23,10,38]);
} */
