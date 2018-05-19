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

string judge_plate(int index, int i){
  string plate_name_temp;
  switch(i){
    case 0: //A原板
      if(index == 0) plate_name_temp = "B";
      else if(index == 1) plate_name_temp = "C";
      else if(index == 2) plate_name_temp = "D";
      else if(index == 3) plate_name_temp = "E";
      else if(index == 4) plate_name_temp = "F";
      break;
    case 1: //B原板
      if(index == 0) plate_name_temp = "G";
      else if(index == 1) plate_name_temp = "C";
      else if(index == 2) plate_name_temp = "A";
      else if(index == 3) plate_name_temp = "F";
      else if(index == 4) plate_name_temp = "K";
      break;
    case 2: //C原板
      if(index == 0) plate_name_temp = "H";
      else if(index == 1) plate_name_temp = "D";
      else if(index == 2) plate_name_temp = "A";
      else if(index == 3) plate_name_temp = "B";
      else if(index == 4) plate_name_temp = "G";
      break;
    case 3: //D原板
      if(index == 0) plate_name_temp = "I";
      else if(index == 1) plate_name_temp = "E";
      else if(index == 2) plate_name_temp = "A";
      else if(index == 3) plate_name_temp = "C";
      else if(index == 4) plate_name_temp = "H";
      break;
    case 4: //E原板
      if(index == 0) plate_name_temp = "J";
      else if(index == 1) plate_name_temp = "F";
      else if(index == 2) plate_name_temp = "A";
      else if(index == 3) plate_name_temp = "D";
      else if(index == 4) plate_name_temp = "I";
      break;
    case 5: //F原板
      if(index == 0) plate_name_temp = "K";
      else if(index == 1) plate_name_temp = "B";
      else if(index == 2) plate_name_temp = "A";
      else if(index == 3) plate_name_temp = "E";
      else if(index == 4) plate_name_temp = "J";
      break;
    case 6: //G原板
      if(index == 0) plate_name_temp = "H";
      else if(index == 1) plate_name_temp = "C";
      else if(index == 2) plate_name_temp = "B";
      else if(index == 3) plate_name_temp = "K";
      else if(index == 4) plate_name_temp = "L";
      break;
    case 7: //H原板
      if(index == 0) plate_name_temp = "I";
      else if(index == 1) plate_name_temp = "D";
      else if(index == 2) plate_name_temp = "C";
      else if(index == 3) plate_name_temp = "G";
      else if(index == 4) plate_name_temp = "L";
      break;
    case 8: //I原板
      if(index == 0) plate_name_temp = "J";
      else if(index == 1) plate_name_temp = "E";
      else if(index == 2) plate_name_temp = "D";
      else if(index == 3) plate_name_temp = "H";
      else if(index == 4) plate_name_temp = "L";
      break;
    case 9: //J原板
      if(index == 0) plate_name_temp = "K";
      else if(index == 1) plate_name_temp = "F";
      else if(index == 2) plate_name_temp = "E";
      else if(index == 3) plate_name_temp = "I";
      else if(index == 4) plate_name_temp = "L";
      break;
    case 10: //K原板
      if(index == 0) plate_name_temp = "G";
      else if(index == 1) plate_name_temp = "B";
      else if(index == 2) plate_name_temp = "F";
      else if(index == 3) plate_name_temp = "J";
      else if(index == 4) plate_name_temp = "L";
      break;
    case 11: //L原板
      if(index == 0) plate_name_temp = "G";
      else if(index == 1) plate_name_temp = "K";
      else if(index == 2) plate_name_temp = "J";
      else if(index == 3) plate_name_temp = "I";
      else if(index == 4) plate_name_temp = "H";
      break;
    default:
      cerr << "Now loading No is over 12" << endl;
  }
  return plate_name_temp;
}
void set_star_penta(int nstar, int i, star * star_p){
  //引数は全粒子数,原板ナンバー,回転後の星のポインタ
  star * star_temp = star_p;
  int penta_id;
  #ifdef OPENMP
    #pragma omp parallel for private(penta_id)
  #endif
  for(int j=0; j<nstar; j++){
    #ifdef MARGIN
      // 天球を1/5角度(72度)ずつ分割 マージン有ver.
      // もっときれいにしたいぞ
      double ra_temp = 0.0;
      int flag = 0, flag_rand = 0;
      float flag_rand_f = 0.0;
      ra_temp = (star_temp+j)->get_ra_rot();
      srand(123L);
      if((ra_temp >= 360.0-margin_angle) || (ra_temp <= penta_angle+margin_angle)){
        penta_id = 0;
        flag = 1;
      }else if((ra_temp >= penta_angle-margin_angle) && (ra_temp <= 2.0*penta_angle+margin_angle)){
        if(flag){
          // flagがたってるときはランダムで原板切り替えを行う
          flag_rand_f = (drand48()+0.5);
          flag_rand = (int)flag_rand_f;
          if(flag_rand){continue;}
        }
        penta_id = 1;
        flag = 1;
      }else if((2.0*ra_temp >= penta_angle-margin_angle) && (ra_temp <= 3.0*penta_angle+margin_angle)){
        if(flag){
          // flagがたってるときはランダムで原板切り替えを行う
          flag_rand_f = (drand48()+0.5);
          flag_rand = (int)flag_rand_f;
          if(flag_rand){continue;}
        }
        penta_id = 2;
        flag = 1;
      }else if((3.0*ra_temp >= penta_angle-margin_angle) && (ra_temp <= 4.0*penta_angle+margin_angle)){
        if(flag){
          // flagがたってるときはランダムで原板切り替えを行う
          flag_rand_f = (drand48()+0.5);
          flag_rand = (int)flag_rand_f;
          if(flag_rand){continue;}
        }
        penta_id = 3;
        flag = 1;
      }else if((4.0*ra_temp >= penta_angle-margin_angle) || (ra_temp <= margin_angle)){
        if(flag){
          // flagがたってるときはランダムで原板切り替えを行う
          flag_rand_f = (drand48()+0.5);
          flag_rand = (int)flag_rand_f;
          if(flag_rand){continue;}
        }
        penta_id = 4;
      }
    #else
      //天球を1/5角度(72度)ずつ分割 マージン無ver.
      penta_id = (int)((star_temp+j)->get_ra_rot()/penta_angle) % 5;
    #endif
    //原板のどこにいるかの判定
    (star_temp+j)->set_plate_name(judge_plate(penta_id,i));
  }
  return;
}
void rotationZY(int nstar, star * star_p, double angleZ, double angleY, int plate){
  star * star_temp = star_p;
  //回転前のベクトルと回転後のベクトル
  VectorXd point_in_3d(3), point_out_3d(3);
  vector pos_temp = 0.0;
  //反時計回り回転行列Z,Y
  Matrix3d AxisAngleZ, AxisAngleZ_add;
  Matrix3d AxisAngleY;
  //各軸の設計
  Vector3d axisY;
  Vector3d axisZ;
  axisY << 0,1,0;
  axisZ << 0,0,1;

  //axisに対する反時計回りの回転行列を作成
  AxisAngleZ = AngleAxisd(angleZ, axisZ);
  AxisAngleY = AngleAxisd(angleY, axisY);
  AxisAngleZ_add = AngleAxisd(-half_penta_angle*RADIAN,axisZ);

  // オフセット中心の回転行列
  Matrix3d AxisAngleY_offset, AxisAngleZ_offset;
  #ifdef OFFSET
    // オリオン座を中心にしたときの原板作成のための回転行列
    AxisAngleZ_offset = AngleAxisd(-offset_angle_z*RADIAN, axisZ);
    AxisAngleY_offset = AngleAxisd(-(90.0-offset_angle_y)*RADIAN, axisY);
  #else
    AxisAngleZ_offset = AngleAxisd(0.0, axisZ);
    AxisAngleY_offset = AngleAxisd(0.0, axisY);
  #endif

  #ifdef OPENMP
    #pragma omp parallel for private(pos_temp, point_in_3d, point_out_3d)
  #endif
  for(int i=0; i<nstar; i++){
    //point_in_3dに確保しておく
    pos_temp = (star_temp+i)->get_pos();
    //直行座標に変換
    pos_temp = trans_xyz(pos_temp);
    (star_temp+i)->set_posxyz(pos_temp);
    //Vector3f型に変換
    point_in_3d = trans_vec_to_eigen(pos_temp);
    //回転行列をかけて回転 オフセット回転させない場合は後ろ2行列は単位行列になる
    point_out_3d = AxisAngleY*AxisAngleZ*AxisAngleY_offset*AxisAngleZ_offset*point_in_3d;
    if(plate>5){
      // 後半の原板は半角だけz周りに回転させる必要がある
      point_out_3d = AxisAngleZ_add*point_out_3d;
    }
    pos_temp = trans_eigen_to_vector(point_out_3d);
    (star_temp+i)->set_posxyz(pos_temp);
    //(r,θ,φ)に変換してから挿入か(x,y,z)のまま挿入か
    pos_temp = trans_rtp(pos_temp);
    (star_temp+i)->set_posrot(pos_temp);
  }
}
void set_star_to_plate(int nstar, star * star_p){
  //天球を回転させてpenta_numの探査を行う
  double rot_angle_y, rot_angle_z;
  // 恒星データの回転
  for(int i=0; i<PLATE_NUM; i++){ //原板のforループ
    cout << "Now loading No." << i << " plate based." << endl;
    // ラジアンで書くように!!
    if(i==0){
      rot_angle_y = 0.0;
      rot_angle_z = 0.0;
    }else if(i<6){
      rot_angle_y = (M_PI - alpha);
      rot_angle_z = (0.5+(i-1)) * penta_angle * RADIAN;
    }else if((i>=6)&&(i!=11)){
      rot_angle_y = alpha;
      rot_angle_z = (i-5) * penta_angle * RADIAN;
    }else{
      rot_angle_y = 180.0 * RADIAN;
      rot_angle_z = 180.0 * RADIAN;
    }
    // //z軸とy軸の周りを回転させて天球1/5カット
    rotationZY(nstar, star_p, -rot_angle_z, -rot_angle_y, i);
    set_star_penta(nstar, i, star_p);
  }
  return;
}
void calc_plate_num(int nstar, star * star_p, plate * plate_p){
  //原板番号の算出
  int i = 0, j = 0;
  int max, max_id, index;
  int counter[PLATE_NUM];
  star  * star_temp = star_p;
  plate * plate_temp = plate_p;
  string str_temp;

  #ifdef OPENMP
    // #pragma omp parallel for private(max, max_id, index, counter, j)
  #endif
  for(i=0; i<nstar; i++){
    max = 0;
    max_id = 0;
    for(j=0; j<PLATE_NUM; j++){
      counter[j] = 0;
    }

    // 所属原板数カウント
    str_temp = (star_temp+i)->get_plate_name();
    for(j=0; j<str_temp.length(); j++){
      index = (char)str_temp[j]-'A';
      if(index < 0 || index >= PLATE_NUM){
        // error出力
        // cerr << "warning(valc_plate_num): id=" << i << " plate_name(" << str_temp << ") has " << (char)str_temp[j] << endl;
      } else {
        counter[index]++;
      }
    }

    // 一番多い原板を探すだけならmax探査でOK
    for(j=0; j<PLATE_NUM; j++){
      // cout << (char)(j+'A') << " " << counter[j] << " " << max << endl;
      if(counter[j]>max){
        // 先にmaxになったものが入る　同数でも同様
        max = counter[j];
        max_id = j;
      }
      // cout << "plate_" << (char) ('A' + j) << ":" << counter[j] << ", ";
    }

    string id_temp = "";
    int n_overlap = 0;
    for(j=0; j<PLATE_NUM; j++){
      if((counter[j] == max)){
        id_temp += (j+'A');
        n_overlap++;
      }
    }
    // cout << "This star overlapping in " << id_temp << endl;
    float flag_f;
    int flag;
    srand(123L);
    if(n_overlap == 2){
      flag_f = (drand48()+0.5);
      flag = (int)flag_f;
      if(flag){
        // 1/2の確率で後の原板に割り振る
        cout << "Change plate num " << id_temp[0] << " to ";
        max_id = id_temp[1] - 'A';
        cout << id_temp[1] << endl;
      }
    }else if(n_overlap == 3){
      // 3枚の原板でオーバーラップ
      flag_f = rand();
      flag = (int)flag_f%n_overlap;
      switch(flag){
        case 0:
          break;
        case 1:
          cout << "Change plate num " << id_temp[0] << " to ";
          max_id = id_temp[1] - 'A';
          cout << id_temp[1] << endl;
          break;
        case 2:
          cout << "Change plate num " << id_temp[0] << " to ";
          max_id = id_temp[2] - 'A';
          cout << id_temp[2] << endl;
          break;
        default:
          cout << "max_id is not 0~2" << endl;
          break;
      }
    }

    // cout << (star_temp+i)->get_mynum() << " " << (star_temp+i)->get_plate_name() << "  max:" << (char) ('A' + max_id) << "\n" << endl;
    (star_temp+i)->set_plate_num(max_id);
    (plate_temp+max_id)->add_nstar(1);
  }
}

// z軸に対するオフセットがある場合の処理
real calc_new_coordinate_dec(star * star_p, int NS){
  real dec_old = star_p->get_dec();
  real numerator, denominator, star_angle;
  // 分母
  denominator = R_SCALE * cos(dec_old*RADIAN);
  numerator = R_SCALE * sin(dec_old*RADIAN);
  if(NS == 0){
    // 北極側のシフト演算
    numerator -= H_SCALE;
  }else{
    // 南極側のシフト演算
    numerator += H_SCALE;
  }
  star_angle = atan2(numerator, denominator);
  return star_angle;
}
void set_new_coordinate_dec(plate * plate_p){
  for(int I=0; I<PLATE_NUM; I++){
    // i 原版の中で一番最初の star ポインタ
    star * star_p = (plate_p + I)->get_starf();
    int nstar_in_plate = (plate_p + I)->get_nstar();
    double new_dec = 0.0;
    if(I < PLATE_NUM/2){
      for(int i=0; i<nstar_in_plate; i++){
        new_dec = calc_new_coordinate_dec(star_p+i, 0) / RADIAN;
        (star_p+i)->set_dec(new_dec);
      }
    }else{
      for(int i=0; i<nstar_in_plate; i++){
        new_dec = calc_new_coordinate_dec(star_p+i, 1) / RADIAN;
        (star_p+i)->set_dec(new_dec);
      }
    }
  }
}
