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

module M2Head(t){
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

module arch(){
    color("blue") import("stl/base-v2.stl");
}

module other(){
    color("blue") import("stl/rc-xd-4-v1.stl");
}

///////////////////////////////////////////////////////////////////////////

/* module skidplatebolts(){
    // M3head(10);
    for (x = [0:30:90]) translate([x,0,0]) M3Head(10);
} */

/* module skidplate(){
    w = 7;
    h = 7;
    l = 100;

    difference()
    {
        cube([l, w, h]);
        translate([l/2 - 90/2, 2*w/3, h/2]) rotate([90,0,0])
            for (x = [0:30:90]) translate([x,0,0]) M3Head(10);
    }
} */
// save
/* module bumper(th, sf=0.3){
    w = 102+7;
    difference(){
        union(){
            translate([0,w/2+7/2,0]) scale([sf,1,1]) cylinder(d=w, h=th);
            //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) screwhole(20);
        }
        //translate([0,w/2,-2]) scale([sf-.05,1,1]) cylinder(d=w-14, h=th+3);
        translate([0,-2, -2]) cube([20, w+5, 20]);

        //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) screwhole(20);
        //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) cylinder(d=2.65, h=30);
    }
} */

module bumper(th, sf=0.3){
    w = 102+7+7;
    difference(){
        union(){
            translate([0,w/2,0]) scale([sf,1,1]) cylinder(d=w, h=th);
            //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) screwhole(20);
        }
        translate([0,w/2,-2]) scale([sf-.05,1,1]) cylinder(d=w-14-6, h=th+3);
        translate([0,-2, -2]) cube([90, w+5, 20]);

        //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) screwhole(20);
        //translate([-17,(102+14)/2-20,5]) rotate([0,90,0]) cylinder(d=2.65, h=30);
    }
}

module screwhole(l=20){
    dia = 8;
    difference(){
        cylinder(d=dia, h=l);
        translate([0,0, -2]) cylinder(d=2.65, l+4);  // screw hole: 2.5 - 2.75
    }
    translate([0,0,l]) sphere(d=dia);
}

module bolthole(l=10, d=15){
    dia = d;
    dia2 = 2*d;
    difference()
    {
        union()
        {
            cylinder(d=dia, h=l);
            /* translate([0,dia2/3,0]) cylinder(d=dia2, h=l); */
        }
        /* translate([0,0, -2]) cylinder(d=2.65, l+4);  // screw hole: 2.5 - 2.75 */
        translate([0,0, l/2]) M3Nut(l, true);
        /* translate([-dia2/2, dia/3,-2]) cube([dia2,dia2,l+4]); */
    }
    /* translate([0,0,l]) sphere(d=dia); */
}

module boltholebrace(l=10, d=15){
    dia = d;
    dia2 = 2*d;
    difference()
    {
        union()
        {
            cylinder(d=dia, h=l);
            translate([0,dia2/3,0]) cylinder(d=dia2, h=l);
        }
        /* translate([0,0, -2]) cylinder(d=2.65, l+4);  // screw hole: 2.5 - 2.75 */
        translate([0,0, l/2]) M3Nut(l, true);
        translate([-dia2/2, dia/3,-2]) cube([dia2,dia2,l+4]);
    }
    /* translate([0,0,l]) sphere(d=dia); */
}

module frameside(){
    w = 10;
    h = 10;
    difference()
    {
        dia = 10;
        sl = 78;
        union(){
            translate([30,0,0]) cube([200-60, w, h]); // main bar
            /* translate([0,(102+14)/2-sl/2-8,dia/2]) rotate([-90,0,0]) color("red") cylinder(d=10,h=4); // shock joint
            translate([0,(102+14)/2-sl/2-4,dia/2]) rotate([-90,0,0]) color("red") cylinder(d=5,h=4); // shock washer */
            translate([0,(102+14)/2-sl/2,dia/2]) rotate([-90,0,0]) cylinder(d=10, h=sl); // shocks
            translate([30,0,0]) bumper(dia,.60);  // .63

            // side mount holes
            translate([42+5,0,h+3]) rotate([-90,0,0]) boltholebrace(10);
            translate([42+90+26-5,0,h+3]) rotate([-90,0,0]) boltholebrace(10);

            // front mount holes
            ww = 70;
            xx = 20;
            translate([-5,(102+7+7)/2-ww/2,0]) cube([10,ww,10]);
            translate([-5,(102+7+7)/2+xx,h+3]) rotate([-90,0,-90]) boltholebrace(10);
            translate([-5,(102+7+7)/2-xx,h+3]) rotate([-90,0,-90]) boltholebrace(10);
        }
        // shock screws
        translate([0,-2,dia/2]) rotate([-90,0,0]) cylinder(d=2.5, h=130);
        translate([200,-2,dia/2]) rotate([-90,0,0]) cylinder(d=2.5, h=130);
    }

    // cross bar to other side, top down
    difference()
    {
        h = 10;
        w = 10;
        cen = 200/2;
        dist = cen - (30+15+13);
        translate([dist, 0, 0]) cube([w, 102+h, h]);

        // top to base mounting holes
        translate([dist+w/2,116/2+15,4]) rotate([180,0,0]) M3Head(h);
        translate([dist+w/2,116/2-15,4]) rotate([180,0,0]) M3Head(h);
    }


    translate([100-35-9, 13, 0]) bolthole(10);
    translate([100+35+9, 13, 0]) bolthole(10);
    /* translate([100, 10/2, 0])screwhole(20); */
    // front and back screw holes
    /* translate([-17,(102+14)/2-20,10+1.5]) rotate([0,90,0]) screwhole(20);
    translate([-17,(102+14)/2+20,10+1.5]) rotate([0,90,0]) screwhole(20); */

}

module frame(){
    w = 7;
    h = 7;
    /* cube([200, w, h]); */
    frameside();
    translate([200,102+w+w,0]) rotate([0,0,180]) frameside();

    difference()
    {
        translate([51,5,0]) cube([98,102+7,2]);
        translate([100,(102)/2+7, -1]) cylinder(d=102,h=4);

        // holes to mount turrent ... right side
        translate([100-35-9,13,-1]) M3(35);
        translate([100+35+9,13,-1]) M3(35);
        // turret left side
        translate([100-35-9,102+14-13,-1]) M3(35);
        translate([100+35+9,102+14-13,-1]) M3(35);

    }

}

/* bolthole(10); */

frame();

/* translate([0,0,-30]) other(); */
/* {
    pos = 200/2 - (30+15+13);
    translate([pos, 0, -65-7]) color("purple") cube([90+26,7,7]);
    translate([pos, 0, -65]) arch();
    translate([pos+90+13+13, 116, -65]) rotate([0,0,180]) arch();
} */
//translate([100, 109/2, -5]) turret();
