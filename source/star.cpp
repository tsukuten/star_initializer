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
#include <cstdlib> // std::atoi, std::strtol
#include <cstdio>  // std::sscanf

// EigenさんはEigen::としないといけないのでnamespaceしときました
using namespace std;
using namespace Eigen;

#define real double

#include "../include/calc_option.hpp"
#include "../include/constant.hpp"
#include "../include/myvector.hpp"
#include "../include/star_class.hpp"
#include "../include/eigen_vector.hpp"

// ステージの高さを5等から約20μm刻みに分割した際の等級（-2.0~5等まで）
const double mag_layer_table[] = {5.0, 4.32, 3.79, 3.37, 3.02, 2.72, 2.45, 2.21, 2, 1.81, 1.63,
                                  1.46, 1.31, 1.17, 1.03, 0.91, 0.79, 0.67, 0.57, 0.46, 0.36,
                                  0.27, 0.18, 0.09, 0.01, -0.07, -0.15, -0.22, -0.29, -0.36,
                                  -0.43, -0.49, -0.56, -0.62, -0.68, -0.74, -0.79, -0.85, -0.9,
                                  -0.96, -1.01, -1.06, -1.11, -1.16, -1.2, -1.25, -1.29, -1.34,
                                  -1.38, -1.42, -1.47, -1.51, -1.55, -1.59, -1.62, -1.66, -1.7,
                                  -1.74, -1.77, -1.81, -1.84, -1.88, -1.91, -1.94, -1.98};

double calc_mag_layer(double mag, star * star_p){
  int size = sizeof(mag_layer_table)/sizeof(mag_layer_table[0]);
  if(mag >= 5.0){
    // 5等より暗い
    star_p->set_layer_index((int) round(mag) + size);
    return (int) round(mag);
  }else{
    for(int i=0; i<size; i++){
      if(mag > mag_layer_table[i]){
        star_p->set_layer_index(size - i - 1);
        return (mag_layer_table[i] + mag_layer_table[i-1]) * 0.5;
      }
    }
  }
}

int star::include_star_data(int nstar, star * star_p, int data_num, double cut_mag){
  star * star_temp = star_p;
  //csvファイルを1行ずつ読み込む
  char filename[50];
  if ((data_num == 1)||(data_num == 4)) sprintf(filename,"./dataset/tycho2_all.csv");
  else if ((data_num == 2)) sprintf(filename,"./dataset/tycho2_all.csv_hip.csv");
  else if ((data_num == 3)) sprintf(filename,"./dataset/tycho2_all.csv_rejected.csv");
  else if ((data_num == 5)) sprintf(filename,"./dataset/test_data.csv");
  else if ((data_num == 6)) sprintf(filename,"./dataset/hipp_all_rejected.csv");
  // else if ((data_num == 4)) sprintf(filename,"tycho2_all.csv");
  cout << "\nInclude from " << filename << endl;
  ifstream ifs (filename);
  string str;
  int i=0, j=0;
  // 本来やりたいデータから呼び出し
  int check = 0;
  while((i<nstar)&&(getline(ifs,str))){
    j = 0;
    string token;
    istringstream stream(str);
    (star_temp+i)->set_r(1.0);
    //1行のうち、文字列とコンマを分割する
    while(getline(stream,token,',')){
      /* ファイルから得られる順序及び型は
       * [0:ra:double, 1:dec:double, 2:vt_mag:double, 3:bt_mag:double, 4:hip_num:unsigned int]
       */
      switch(j){
        case 0: //ra:double
          (star_temp+i)->set_ra(stod(token));
          break;
        case 1: //dec:double
          (star_temp+i)->set_dec(stod(token));
          break;
        case 2: //vt_mag:double
          if(token.size() == 0)
            (star_temp+i)->set_vtmag(VT_MAG_NaN);
          else
            (star_temp+i)->set_vtmag(stod(token));
          break;
        case 3: //bt_mag:double
          if(token.size() == 0)
            (star_temp+i)->set_btmag(BT_MAG_NaN);
          else{
            (star_temp+i)->set_btmag(stod(token));
            (star_temp+i)->set_vismag_2();
          }
          break;
        case 4: //hip_num:unsigned int
          if(token.size() == 0)
            (star_temp+i)->set_hipnum(HIP_NUM_NaN);
          else
            (star_temp+i)->set_hipnum((unsigned int)stoi(token));
          break;
        case 5: //visual magniturde
          if(token.size() == 0){
            // // star_temp->set_vismag();
            (star_temp+i)->set_vismag_2();
          } else {
            (star_temp+i)->set_vismag_direct(stod(token));
            #ifdef CLASSIFY
              double classified_mag = calc_mag_layer(stod(token), (star_temp+i));
              (star_temp+i)->set_classified_vismag(classified_mag);
            #endif
          }
          break;
        default: // 予定よりcolumnが多かったごときで処理を終わらせては(exit(0)しては)いけない
          cerr << "over columns :(" << i << "," << j << ")" << endl;
          break;
      }
      j++;
    }
    // case 5 入らない案件
    // 原因不明：実視等級がやたら低い
    // 11等級までで50万個しか入っていない
    // 解決必須
    // if(check < 10){
    //   cout << (star_temp+i)->get_vismag() << endl;
    //   check++;
    // }
    if((star_temp+i)->get_vismag()<=cut_mag){
      // cout << (star_temp+i)->get_classified_vismag() << endl;
      i++;
      (star_temp+i)->set_mynum(i+1);
    }
  }
  cout << "\nThe number of loaded stars:" << i << endl;

  char flag;
  cout << "Are you sure you want to proceed with the calculation?....... [Y / N]";
  cin >> flag;
  if((flag == 'Y') || (flag == 'y')){
    return i;
  }else{
    cout << "Cancel calculation." << endl;
    exit(1);
  }
}

void star::set_posplate(){
  // 天球面上の恒星を平面に射影する関数
  // xy座標の計算
  double cosa = cos(proj_angle*RADIAN);
  position_plate[0]=(F)*(rg/(sin(proj_angle*RADIAN)))*cosa*(get_xyradii()/get_z())*(get_x()/get_xyradii());
  position_plate[1]=(F)*(rg/(sin(proj_angle*RADIAN)))*cosa*(get_xyradii()/get_z())*(get_y()/get_xyradii());
  //平方根の中身がマイナスのときはレーザーの最小スポット径より小さく，表現できないので最小スポット径とする
  //x[2]つまりz軸の係数の平方根をとるのは、相対的に穴の大きさは変わらないから　本来は平方根をとらない
  #ifdef CLASSIFY
    spot = sqrt(exp((mag0-get_classified_vismag())/(2.5*loge)))*r_spot;
  #else
    spot = sqrt(exp((mag0-get_vismag())/(2.5*loge)))*r_spot;
  #endif
  #ifdef ANALITIC
  // 解析解を使ったステージの高さ
     double check;
     check=pow((spot/r_min),2.0)-1.0;
     if(check > 0.0){
         position_plate[2]=(M_PI*pow(r_min,2.0)/lambda)*sqrt((pow((spot/r_min),2.0)-1.0));
     }else{
         position_plate[2]=-z0;
     }
  #else
    // 経験則を使ったステージの高さと照射時間の設定
    set_z_t_relation();
  #endif
}
