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

// realを使ったところは double と float を使い分けたいであろう変数を定義する際に利用
#define real double

// ヘッダーファイル
#include "../include/calc_option.hpp"
#include "../include/constant.hpp"
#include "../include/plate_class.hpp"
#include "../include/star_class.hpp"
#include "../include/eigen_vector.hpp"
#include "../include/translation.hpp"
#include "../include/sort.hpp"
#include "../include/output.hpp"

// マクロ関数
#define print(var) cout << #var << ":\n" << var << endl << endl
#define print_name(var) cout << (#var)

int main(int argc, char * argv[]){
  double cut_mag; // カットオフ等級
  int dataset;    // 使うデータセットの指定　(tycho か Hippalcos かを指定する）

  if(argc == 3){
    char * ends;
    cut_mag = strtod(argv[1], & ends); // カットオフ等級
    dataset = atoi(argv[2]);          // 使うデータセットの指定　(tycho か Hippalcos かを指定する）
  } else {
    cout << "Argument 1: Input cutoff magnitude: ";
    cin >> cut_mag;
    cout << "\n\n";
    cout << "\t1.Using ALL of stars of tycho:" << N_tycho_all << "stars" << endl;
    cout << "\t2.Using Hippalcos overlapped stars of ALL tycho:" << N_tycho_hip << "stars" << endl;
    cout << "\t3.Using of useful Rejected stars of ALL tycho:" << N_tycho_rem << "stars" << endl;
    cout << "\t4.Using first 100 stars of ALL tycho" << endl;
    cout << "\t5.Using TEST dataset" << endl;
    cout << "\t6.Using Hippalcos Bright stars:" << N_hip_rem << "stars" << endl;
    cout << "Argument 2: Input dataset number 0~6: ";
    cin >> dataset;
    if(dataset > 6){
      cout << "Error!! No." << dataset << " is not available." << endl;
      return 0;
    }
  }

  unsigned int n;
  n  = output_file_condition(dataset);  // データセットの設定と恒星数の取得

  output_constant();                    // 定数とかの確認用出力

  // 恒星データ配列の確保
  star * stp = NULL;
  stp = new star[n+1];
  // 原板情報の確保 12枚分
  plate * plp = NULL;
  plp = new plate[PLATE_NUM];

  // 恒星データの読み込みと視等級計算
  n = stp->include_star_data(n, stp, dataset, cut_mag);

  // 所属原板ナンバー計算
  set_star_to_plate(n, stp);
  calc_plate_num(n, stp, plp);

  // 原板リストの作成
  plp->create_plate_list(n, stp, plp);
  plp->calc_position_in_plate(plp);
  #ifdef SORT_BY_MESH
    plp->sort_in_plate_by_mesh(stp, plp);
  #endif
  cout << "END CALCULATION..." << endl;

  // 恒星データの出力
  output_data(plp, dataset);
  cout << "END OUTPUT FILE..." << endl;

  // 恒星データのgnuplotファイル作成
  #ifdef GNUPLOT
    output_gnuplot();
  #endif

  return 0;
}
