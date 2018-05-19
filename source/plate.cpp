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
#include "../include/eigen_vector.hpp"
#include "../include/translation.hpp"
#include "../include/sort.hpp"

void plate::create_plate_list(int nstar, star * star_p, plate * plate_p){
  star  * star_temp = star_p;
  plate * plate_temp = plate_p;
  int nstar_temp = 0;
  //原版ごとに恒星データを並べ替える
  sort_by_platenum(star_temp, nstar);
  plate_temp->set_starf(star_temp);
  for(int i=1; i<PLATE_NUM; i++){
    nstar_temp += (plate_temp+i-1)->get_nstar();
    (plate_temp+i)->set_starf(star_temp+nstar_temp);
  }
  return;
}

void plate::sort_in_plate_by_mesh(star * star_p, plate * plate_p){
  star  * star_temp = star_p;
  plate * plate_temp = plate_p;
  int nstar_temp = 0;

  // 原版内で恒星データをプロット順に並べ替える
  for(int i=0; i<PLATE_NUM; i++){
    sort_in_plate((plate_temp+i)->get_starf(),(plate_temp+i)->get_nstar());
  }
  nstar_temp = 0;
  for(int i=1; i<PLATE_NUM; i++){
    nstar_temp += (plate_temp+i-1)->get_nstar();
    (plate_temp+i)->set_starf(star_temp+nstar_temp);
  }
}


real calc_rot_angle_y_MARGIN_H(int i){
  real rot_angle = 0.0;
  if(i==0){
    rot_angle = 0.0;
  }else if(i<6){
    #ifdef MARGIN_H
      rot_angle = atan2(R_SCALE*sin(M_PI-alpha)-H_SCALE, R_SCALE*cos(M_PI-alpha));
    #else
      rot_angle = M_PI - alpha;
    #endif
  }else if((i>=6)&&(i!=11)){
    #ifdef MARGIN_H
      rot_angle = atan2(R_SCALE*sin(alpha)-H_SCALE, R_SCALE*cos(alpha));
    #else
      rot_angle = alpha;
    #endif
  }else{
    rot_angle = 180.0 * RADIAN;
  }
  return rot_angle;
}
void plate::calc_position_in_plate(plate * plate_p){
  int n_temp = 0;
  star  * star_temp;
  plate * plate_temp = plate_p;
  double rot_angle_z, rot_angle_y;
  #ifdef MARGIN_H
    // 投影機のz軸offsetを考慮する場合は緯度の更新が必要
    set_new_coordinate_dec(plate_p);
  #endif
  for(int i=0; i<PLATE_NUM; i++){
    // 最後の原板は原板探査の自転で回転させてるので今回転する必要がない
    n_temp = (plate_temp+i)->get_nstar();
    star_temp = (plate_temp+i)->get_starf();
    // MARGIN_H の機能のせいでこんな関数を if 分の外に出す羽目になりました
    rot_angle_y = calc_rot_angle_y_MARGIN_H(i);
    if(i==0){
      rot_angle_z = 0.0;
    }else if(i<6){
      rot_angle_z = (0.5+(i-1)) * penta_angle * RADIAN;
    }else if((i>=6) && (i!=11)){
      rot_angle_z = (i-5) * penta_angle * RADIAN;
    }else{
      rot_angle_z = 180.0 * RADIAN;
    }
    // y,z軸まわりに所属原板が一番上にくるように回転させる
    rotationZY(n_temp, star_temp, -rot_angle_z, -rot_angle_y, i);
    for(int j=0; j<n_temp; j++){
      // 原板上でのxyz座標を計算してstarクラスのposition_xyzに格納
      (star_temp+j)->set_posplate();
    }
  }
}
