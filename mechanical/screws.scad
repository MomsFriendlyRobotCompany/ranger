/* $fn=90; */

/* m3 = 3.3; */

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
