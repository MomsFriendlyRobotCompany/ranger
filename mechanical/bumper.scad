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


module joint(a, d){
    rotate([-a,0,0]){
        intersection()
        {
            rotate([180+a,0,0]) cylinder(d=d, h=d,center=true);
            cylinder(d=d, h=d, center=true);
        }
        lead = d;
        cylinder(d=d, h=lead,center=false);
        rotate([180+a,0,0]) cylinder(d=d, h=lead, center=false);
    }
}

module wishbone(){
    {
        difference()
        {
            scale([1,1,.5]) rotate([0,-90,0]) cylinder(d=120, h=20);
            scale([1,1,.45]) rotate([0,-90,0]) translate([0,0,-10]) cylinder(d=100, h=40);
            translate([-50/2, -200/2, 0]) cube([50,200,50]);
        }
    }
}

module bumper(){
    iw = 69;
    il = 20;
    difference(){
        union(){
            translate([-il/2,-iw/2,0]) cube([il,iw,30]); // mounting holes
            translate([10,0,52]) wishbone();
        }

        translate([-3,20,10]) cylinder(d=10,h=40);
        translate([-3,-20,10]) cylinder(d=10,h=40);
        translate([-3,20,5]) M3Head(42);
        translate([-3,-20,5]) M3Head(42);
    }

    /* translate([-3,20,-1]) color("red") cylinder(d=8,h=40);
    translate([-3,-20,-1]) color("red") cylinder(d=8,h=40); */

    // whishbone
    /* translate([10,0,52]) wishbone(); */

    // sides from whishbone to handle
    bw = 10;
    bh = 20;
    translate([-bh/2,120/2-bw,52]) cube([bh,bw,38]);
    translate([-bh/2,-120/2,52]) cube([bh,bw,38]);

    // handle bar
    translate([10,0,50+20])
    difference()
    {
        scale([.25,1,1]) cylinder(d=120, h=20);
        translate([0,0,-1]) scale([.17,1,1]) cylinder(d=100, h=24);
        translate([-20,-100,-100]) cube([20,200,200]);
    }
}

//translate([-10/2,-55/2,52]) color("blue") cube([10,55,45]); // servo

bumper();
