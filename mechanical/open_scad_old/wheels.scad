$fn=90;

module wheel(){
    rotate([90,0,0]) cylinder(h=2.2*25.4, d=5.5*25.4);
}

module rim(shift){
    s = 0.9;
    ss = 1;
    translate([0,shift,0])
    difference()
    {
        diao = 6.5*25.4;
        diai = diao-4;
        //scale([1, s, 1])
        {
            scale([ss, s, ss]) sphere(d=diao);
            scale([ss, s, ss]) sphere(d=diai);
            rotate([90,0,0]) cylinder(d=5.8*25.4,h=400, center=true);

        }
        translate([0, -shift, -200/2]) cube([400,400,200], center=true); // pos
    }
    translate([0,5,0]) rotate([0,60,0]) cylinder(d=5,h=78);
    translate([0,5,0]) rotate([0,-60,0]) cylinder(d=5,h=78);
}

rotate([0,30,0]) rim(-2.2*25.4/2);
wheel();
