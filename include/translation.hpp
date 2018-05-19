#pragma once
string judge_plate(int index, int i);
void set_star_penta(int nstar, int i, star * star_p);
void rotationZY(int nstar, star * star_p, double angleZ, double angleY, int plate);
void set_star_to_plate(int nstar, star * star_p);
void calc_plate_num(int nstar, star * star_p, plate * plate_p);

real calc_new_coordinate_dec(star * star_p, int NS);
void set_new_coordinate_dec(plate * plate_p);
