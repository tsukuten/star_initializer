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
#include "../include/plate_class.hpp"
#include "../include/star_class.hpp"
#include "../include/eigen_vector.hpp"
#include "../include/translation.hpp"
#include "../include/sort.hpp"
#include "../include/output.hpp"

void output_data(plate * plate_p, int data_index){
  // ファイル名，ディレクトリ名
  char filename[50];
  char dirname[50], _dirname[50];
  //ディレクトリ作成
  #ifdef MARGIN
    sprintf(dirname,"plate_data_margin");
  #else
    sprintf(dirname,"plate_data");
  #endif
  mkdir(dirname,0777);
  chmod(dirname,0777);
  star * star_output;
  plate * plate_output;

  for(int i=0; i<PLATE_NUM; i++){
    //ファイル出力
    strcpy(_dirname,dirname);
    switch(data_index){
      case 1:
        sprintf(filename,"/tycho2_all_%d.dat", i);
        break;
      case 2:
        sprintf(filename,"/tycho2_all_hipp_%d.dat", i);
        break;
      case 3:
        sprintf(filename,"/tycho2_all_rej_%d.dat", i);
        break;
      case 4:
        sprintf(filename,"/tycho2_all_100_%d.dat", i);
        break;
      case 5:
        sprintf(filename,"/test_data_%d.dat", i);
        break;
      case 6:
        sprintf(filename,"/hipp_%d.dat", i);
        break;
      default:
        cout << "dataset index is not available" << endl;
        break;
    }
    // sprintf(filename,"/hipp_%c.dat", i+'A');
    strcat(_dirname,filename);
    ofstream outputfile(_dirname);
    plate_output = (plate_p+i);
    star_output = plate_output->get_starf();
    // sort_in_plate(plate_output->get_starf(), plate_output->get_nstar());
    for(int j=0; j<plate_output->get_nstar(); j++){
      outputfile
                 << setw(10) << right << (star_output+j)->get_mynum() << "|"
                // << (star_output+j)->get_hipnum() << "\t"
                // << (star_output+j)->get_plate_num() << "\t" << endl;
                //  << (star_output+j)->get_posxyz() << "\t"
                 // << (star_output+j)->get_posplate() << "\t"
                 << setw(10) << right << (star_output+j)->get_posplate_nm_x() << "|"
                 << setw(10) << right << (star_output+j)->get_posplate_nm_y() << "|"
                 << setw(10) << right << (star_output+j)->get_posplate_nm_z() << "|"
                 // << (star_output+j)->get_pos() << "|"
                //  << (star_output+j)->get_posrot() << "\t"
                //  << (star_output+j)->get_vtmag() << "\t"
                //  << (star_output+j)->get_btmag() << "\t"
                 // << (star_output+j)->get_vismag() << "\t"
                 // << (star_output+j)->get_spot() << "\t"
                 << setw(10) << right << (int)(star_output+j)->get_ray_time() << "|" << endl;
                //  << (star_output+j)->get_plate_name() << endl;
    }
    #ifdef OUTPUT_CROSS
      real Center[2];
      for(int k = 0; k < 4; k++){
        if(k % 3 == 0){
          // Center[0] = 0.0;
          Center[0] = 1.0 * 10000000.0;
        }else{
          // Center[0] = 2.0 * proj_unit_r * 10000000.0;
          Center[0] = (2.0 * proj_unit_r - 1.0) * 10000000.0;
        }
        if(k < 2){
          // Center[1] = 0.0;
          Center[1] = 1.0 * 10000000.0;
        }else{
          // Center[1] = 2.0 * proj_unit_r * 10000000.0;
          Center[1] = (2.0 * proj_unit_r - 1.0) * 10000000.0;
        }
        // 十字の中心
        for(int l = 0; l < 5; l++){
          outputfile  << setw(10) << right << -1 << "|"
                      << setw(10) << right << (long)(Center[0] + 250000.0 * (l - 2)) << "|"
                      << setw(10) << right << (long)(Center[1]) << "|"
                      << setw(10) << right << 505301 << "|"
                      << setw(10) << right << 1010 << "|" << endl;
        }
        for(int l = 0; l < 5; l++){
          if(l == 2) continue;
          outputfile << setw(10) << right << -1 << "|"
                     << setw(10) << right << (long)(Center[0]) << "|"
                     << setw(10) << right << (long)(Center[1] + 250000.0 * (l - 2)) << "|"
                     << setw(10) << right << 505301 << "|"
                     << setw(10) << right << 1010 << "|" << endl;
        }
      }
    #endif
    outputfile .close();
  }
}

void output_constant(){
  cout << "\nState of Calculation Options..." << endl;
  #ifdef OPENMP
    cout << "\t*OPENMP is effective\t\t:: Number of threads = " << omp_get_max_threads() << endl;
  #else
    cout << "\t*OPENMP is not effective"<< endl;
  #endif

  #ifdef GNUPLOT
    cout << "\t*GNUPLOT is effective\t\t:: Output PNG file to plate_data." << endl;
  #else
    cout << "\t*GNUPLOT is not effective" << endl;
  #endif

  #ifdef ANALITIC
    cout << "\t*ANALITIC is effective" << endl;
  #else
    cout << "\t*ANALITIC is not effective\t:: Using z_t_relation." << endl;
  #endif

  #ifdef CLASSIFY
    cout << "\t*CLASSIFY is effective\t\t:: Discrete magnitude and ray time, stage height." << endl;
  #else
    cout << "\t*CLASSIFY is not effective\t\t:: Using continuity magnitude and ray time, stage height." << endl;
  #endif

  #ifdef OFFSET
    cout << "\t*OFFSET is effective\t\t:: y:" << offset_angle_y << "  z:" << offset_angle_z << endl;
  #else
    cout << "\t*OFFSET is not effective\t:: The origin is Polaris" << endl;
  #endif

  #ifdef MARGIN_H
    #ifdef OFFSET
      cout << "\t*Error! MARGIN_H cannot be effective with OFFSET" << endl;
      cout << "\t You must 'undefine MARGIN_H' and 'make clean / re compile'" << endl;
    #else
      cout << "\t*MARGIN_H is effective\t\t:: h = " << H << " [cm]" << endl;
    #endif
  #endif

  #ifdef MARGIN
    cout << "\t*MARGIN is effective\t\t:: margin = " << margin_angle << "°" << endl;
  #else
    cout << "\t*MARGIN is not effective\t:: Make complete dodecahedron." << endl;
  #endif

  #ifdef SORT_BY_MESH
    cout << "\t*SORT_BY_MESH is effective\t:: The number of meshes:" << x_cell_length << "×" << y_cell_length << endl;
    #ifdef SORT_BY_MESH_AND_MAG
      cout << "\t   ↳ *SORT_BY_MESH_AND_MAG is effective" << endl;
      #ifdef SORT_BY_MESH_AND_MAG_RETURN
        cout << "\t    ↳ *SORT_BY_MESH_AND_MAG_RETURN is effective" << endl;
      #endif
    #endif
  #else
    cout << "\t*SORT_BY_MESH is not effective\t\t:: Only sort by plate number." << endl;
  #endif

  cout << "\t*The radius of dome　 \t\t:: " << R << "[cm]" << endl;
  cout << "\t*The radius of Plates \t\t:: " << rg << "[cm]" << endl;
}

int output_file_condition(int file_num){
  switch (file_num) {
    case 1:
      cout << "Using ALL of stars of tycho:" << N_tycho_all << "stars" << endl;
      return N_tycho_all;
    case 2:
      cout << "Using Hippalcos overlapped stars of ALL tycho:" << N_tycho_hip << "stars" << endl;
      return N_tycho_hip;
    case 3:
      cout << "Using of useful Rejected stars of ALL tycho:" << N_tycho_rem << "stars" << endl;
      return N_tycho_rem;
    case 4:
      cout << "Using first 100 stars of ALL tycho" << endl;
      return 100;
    case 5:
      int n;
      cout << "Using TEST dataset" << endl;
      cout << "Input the number of test stars:";
      cin >> n;
      return n;
    case 6:
      cout << "Using Hippalcos Bright stars:" << N_hip_rem << "stars" << endl;
      return N_hip_rem;
    default:
      cout << "No dataset... Error" << endl;
      exit(1);
  }
}

void output_gnuplot(){
  FILE * fpg;
  stringstream plot_range;
  plot_range << "x_max = y_max =" << 20000000.0*proj_unit_r << "\n";
  fpg = popen("gnuplot", "w");
  if(fpg == NULL){
    cout << "gnuplot is can not open!!" << endl;
  }
  // gnuplotフォルダ内にある画像作成ファイルの読み込み
  fputs(plot_range.str().c_str(), fpg);
  fputs("load './gnuplot/plate_pic.gp'\n", fpg);
  fputs("load './gnuplot/plot_trace.gp'\n", fpg);
  fflush(fpg);
  pclose(fpg);
}
