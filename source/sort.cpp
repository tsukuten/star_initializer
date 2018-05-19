// パッケージ
#include "../Eigen/Dense"
#include "../Eigen/Geometry"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sys/stat.h>

// EigenさんはEigen::としないといけないのでnamespaceしときました
using namespace std;
using namespace Eigen;

#define real double

#include "../include/calc_option.hpp"
#include "../include/constant.hpp"
#include "../include/myvector.hpp"
#include "../include/plate_class.hpp"
#include "../include/star_class.hpp"
#include "../include/eigen_vector.hpp"
#include "../include/translation.hpp"
#include "../include/sort.hpp"


int compare_magnitude(const void * p1, const void * p2){
  //等級でソートするための関数
  star * pp = (star*)p1;
  star * qq = (star*)p2;
  //V等級かB等級か選ぶ
  double comp = ((real) pp->get_vtmag()) - ((real) qq->get_vtmag());
  // double comp = ((real) pp->get_btmag()) - ((real) qq->get_btmag());
  if(comp > 0L){
  	return 1;
  }else if (comp == 0L){
  	return 0;
  }else{
  	return -1;
  }
}
int compare_plate(const void * p1, const void * p2){
  //恒星データのもつナンバーでソートするための関数
  star * pp = (star*)p1;
  star * qq = (star*)p2;
  int comp = 0.0;
  comp = ((real) pp->get_plate_num()) - ((real) qq->get_plate_num());
  if(comp > 0L){
    return 1;
  }else if (comp == 0L){
    return 0;
  }else{
    return -1;
  }
}

// magは等級ソート用
int get_mesh_num(int nx, int ny, int m){
  int mesh = 0;
  if(ny%2 == 0){
    mesh = x_cell_length * ny + nx;
  }else{
    mesh = x_cell_length * ny + (x_cell_length - 1) - nx;
  }
  #ifdef SORT_BY_MESH_AND_MAG
  // 等級でのソートも含める場合
    #ifdef SORT_BY_MESH_AND_MAG_RETURN
      if(m%2 == 0){
        // 偶数等級のとき
        mesh -= x_cell_length * y_cell_length - 1;
        mesh = fabs(mesh);
      }
    #endif
    // 総セル数＊等級分だけ引くことで、大きい等級は大きく引かれ、
    // かならず1個小さい等級の前にソートされる
    mesh -= x_cell_length * y_cell_length * m;
  #endif
  return mesh;
}

int compare_stars_by_mesh(const void * p1, const void * p2){
  star * pp = (star*)p1;
  star * qq = (star*)p2;
  int comp = 0.0;
  int nx1 = (pp->get_x_plate()+rg)*x_cell_length/(2.0*rg);
  int ny1 = (pp->get_y_plate()+rg)*y_cell_length/(2.0*rg);
  int nx2 = (qq->get_x_plate()+rg)*x_cell_length/(2.0*rg);
  int ny2 = (qq->get_y_plate()+rg)*y_cell_length/(2.0*rg);
  int mesh1, mesh2;
  double mag1 = pp->get_vismag();
  double mag2 = qq->get_vismag();
  int m_1 = pp->get_layer_index();
  int m_2 = qq->get_layer_index();
  mesh1 = get_mesh_num(nx1, ny1, m_1);
  mesh2 = get_mesh_num(nx2, ny2, m_2);
  // cout << "mesh1: " << mesh1 << " mesh2: " << mesh2 << endl;
  comp = mesh1 - mesh2;
  if(comp > 0L){
    return 1;
  }else if (comp == 0L){
    return 0;
  }else{
    return -1;
  }
}

void sort_by_platenum(star * star_p, int nstar){
  star * stp = star_p;
  //ナンバー順にstarリストをソートする
  qsort(stp, nstar, sizeof(star), compare_plate);
  return;
}

void sort_in_plate(star * star_first, int nstar){
  star * stp = star_first;
  //mesh順にstarリストをソートする
  qsort(stp, nstar, sizeof(star), compare_stars_by_mesh);
  return;
}
