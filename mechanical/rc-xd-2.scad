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
    M3(t);
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
            translate([0,0,-1]) cylinder(d=102, h=length+5);
            translate([0,-100,-10]) cube([100,200,200]);

            // tapper in front
            rotate([90,0,0]) {
                l = 400;
                translate([0-4,100-2,-l/4]) cylinder(d=102, h=l/2);
            }

            // center cut out from motor
            rotate([0,-90,0]) scale([1.5,1.2,1]) cylinder(d=85, h=100);
        }
    }
}

module suspension2(thick=10){

    dd = 30;
    sus = 77;

    difference()
    {
        union(){
            translate([dd/2,sus/2,thick/2]) rotate([90,0,0])
            cylinder(d=thick, h=sus);

            // half circle support for suspension
            difference()
            {
                translate([dd/2,0,0]) scale([1,2.2,1]) cylinder(d=dd,h=thick);
                translate([dd/2,-100/2,-1]) cube([50,100,20]);
            }
        }
        // for a M3 screw, could use 2.5mm or 2.75mm ... 2.5 is a little tight, but works
        translate([dd/2,sus/2+1,thick/2]) rotate([90,0,0]) cylinder(d=2.75, h=80);
    }
}


difference()
{
    union(){
        frame2();
        translate([65,-102/2,58]) suspension2();
    }

    translate([71-15/2+30/2-5,-102/2-20,62-2]) rotate([0,180,0]) M3NutVertical(10); // front right
    translate([71-15/2+30/2-5,-102/2,62-2]) rotate([0,180,0]) M3NutVertical(10); // front center
    translate([71-15/2+30/2-5,-102/2+20,62-2]) rotate([0,180,0]) M3NutVertical(10); // front left
}
