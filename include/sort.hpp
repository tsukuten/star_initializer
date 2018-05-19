#pragma once
int compare_magnitude(const void * p1, const void * p2);
int compare_plate(const void * p1, const void * p2);

//magは等級ソート用
int get_mesh_num(int nx, int ny, double mag);

int compare_stars_by_mesh(const void * p1, const void * p2);

void sort_by_platenum(star * star_p, int nstar);
void sort_in_plate(star * star_first, int nstar);
