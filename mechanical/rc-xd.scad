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


module M3NutVertical(t){
    hex(6.4, t);
    cylinder(h=2*t, d=3.3, center=true);
}

module M3Nut(t){
    hex(6.4, t);
}

module M2Nut(t){
    hex(4.65, t);
}

module M2(t){
    cylinder(3*t, d=2.2, center=true);
    translate([0,0,-1]) M2Nut(3); // nut
}

module M3(t){
    cylinder(h=3*t, d=3.3, center=true);
    translate([0,0,2]) cylinder(h=3*t, d=7, center=false); // head
}

module m3h(t){
    /* dia = m3;
    sdia = 7;  // counter sink dia
    cylinder(3*t, d=dia, center=true);  // M3
    translate([0,0,2]) cylinder(3*t, d=sdia, center=false);  // screw driver */
    M3(t);
}

/* translate([65+70,(-102)/2,80]) color("red") cube([23,77,7], center=true); */

/* cube([65,7,7]); */
/* translate([65,0,0]) rotate([0,-45,-45]) cube([65,7,7]); */
/* translate([0,-120-7,0]) {
    cube([65,7,7]);
    translate([35,0,0]) cube([30,7,40]);
} */

module frame(){
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
}

module rail(){
    length = 47;
    inset = 2;
    difference()
    {
        cube([length,7,7]);
        rotate([-90,0,0]){
            translate([length-30-3,-7/2,inset]) m3h(7);
            translate([length-3,-7/2,inset]) m3h(7);
        }
    }
}

module frame2(){
    // rails and screw holes for base plate
    rail();
    translate([0,-102,7]) rotate([180,0,0]) rail();

    // curve frame
    translate([0,-102/2,7]) rotate([0,90,0]) {
        difference()
        {
            // half cylinder to frame
            length = 78;
            cylinder(d=102+14, h=length);
            /* translate([0,0,-1]) cylinder(d=102+6, h=length+5); */
            translate([0,0,-1]) scale([1,1.05,1]) cylinder(d=102, h=length+5);
            translate([0,-100,-10]) cube([100,200,200]);

            // tapper in front
            rotate([90,0,0]) {
                l = 400;
                /* translate([0,length-2,-l/4]) cylinder(d=102, h=l/2); */
                translate([0-4,100-2,-l/4]) cylinder(d=102, h=l/2);
            }

            // center cut out from motor
            rotate([0,-90,0]) scale([1.5,1.25,1]) cylinder(d=85, h=100);
            ll = 150;
            /* rotate([90,0,0]) translate([-60,0,-ll/2]) scale([.8,1.5,1]) cylinder(d=85, h=ll); */
        }
    }
}

/* translate([103,-102/2,80/2+11]) cylinder(d=77,h=7); */

module suspension(){
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
}

/* module sus(t){
    length = 30;
    h = 10;
    w = 8;
    inset = w-2;
    difference()
    {
        cube([length,10,h]);
        rotate([-90,0,0]){
            translate([length-4,-h/2,inset]) {
                M3Nut(t);
                translate([0,0,-inset-1]) cylinder(d=m3, h=30);
                translate([-10,0,-inset-1]) cylinder(d=2.5, h=30);
                translate([-20,0,-inset-1]) cylinder(d=2.75, h=30);
            }
        }
    }

}*/



module suspension2(){

    dd = 30;
    thick = 7;

    /* translate([0,-102/2,80]) sus(10); */
    /* translate([102,-102/2+77/2,62]) rotate([90,0,0]) { */
    translate([71-15/2+dd/2,-102/2+77/2,61.5]) rotate([90,0,0]) {
        difference(){
                cylinder(d=thick, h=77);
            translate([0,0,-1]) cylinder(d=2.5, h=80);
        }
    }

    // half circle support for suspension
    difference()
    {
        translate([71-15/2+dd/2,-102/2,62-4]) scale([1,1.9,1]) cylinder(d=dd,h=thick);
        translate([78,-102/2-100/2,50]) cube([50,100,50]);
        translate([71-15/2+dd/2,-102/2+80/2,62-4+7/2]) rotate([90,0,0]) cylinder(d=2.5, h=80);
    }
    /* translate([102,-102/2,62-5]) cylinder(d=40,h=10); */
    /* translate([0,-102/2,80/2+1]) { */
        /* translate([0,0,10]) difference()
        {
                cylinder(d=77, h=6);
                translate([0,0,-1]) cylinder(d=40, h=10); */
            /* translate([110-15,-57/2,17]) cube([50,57,40]); */

            // suspension screw holes
            /* translate([120,0,40]) rotate([90,0,0]){
                translate([0,0,-50]) cylinder(d=m3,h=100);
                translate([-15/2-0,0,-50]) cylinder(d=m3,h=100);
                translate([-15-0,0,-50]) cylinder(d=m3,h=100);
            } */
        /* } */
    /* } */
}

difference()
{
    union(){
        frame2();
        translate([2,0,0]) suspension2();
    }

    translate([71-15/2+30/2-5,-102/2-20,62-2]) rotate([0,180,0]) M3NutVertical(10); // front right
    translate([71-15/2+30/2-5,-102/2+20,62-2]) rotate([0,180,0]) M3NutVertical(10); // front left
}
/* translate([71, -102/2, 0]) cylinder(d=15, h=100); */

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
