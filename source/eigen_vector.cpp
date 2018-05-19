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
#include "../include/eigen_vector.hpp"

Vector3d trans_vec_to_eigen(vector vec){
  Vector3d eigen_temp;
  eigen_temp << vec[0], vec[1], vec[2];
  return eigen_temp;
}
vector trans_eigen_to_vector(Vector3d eigen){
  vector vec_temp;
  for(int i=0; i<3; i++) vec_temp[i] = eigen[i];
  return vec_temp;
}
vector trans_xyz(vector pos){
  // 引数の要素は(r,θ,φ)で返り値の要素は(x,y,z)
  vector pos_xyz = 0.0;
  // 半径情報は基本1にしておけばいい
  double rscale = 1.0;
  if(pos[0]!=1.0){cout << "===r scale is not 1. caution!===" << endl;}
  pos_xyz[0] = rscale*cos(pos[2]*RADIAN)*cos(pos[1]*RADIAN);
  pos_xyz[1] = rscale*cos(pos[2]*RADIAN)*sin(pos[1]*RADIAN);
  pos_xyz[2] = rscale*sin(pos[2]*RADIAN);
  return pos_xyz;
}
vector trans_rtp(vector pos){
  // 引数の要素は(x,y,z)で返り値の要素は(r,θ,φ)
  vector pos_rtp = 0.0;
  pos_rtp[0] = sqrt(pos*pos);
  pos_rtp[1]  = atan2(pos[1],pos[0])/RADIAN;
  pos_rtp[2]  = asin(pos[2])/RADIAN;
  if(pos_rtp[1]<0) pos_rtp[1] += 360.0;
  return pos_rtp;
}
