#define PR(x)  cerr << #x << " = " << x << " "
#define PRC(x) cerr << #x << " = " << x << ",  "
#define PRL(x) cerr << #x << " = " << x << "\n"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>

//読み込みファイル内の列情報
//#NAME #RA #DEC #VTmag. #BTmag. #hip.number
#define real double
// #define real float
#define hip_column 5
using namespace std;

int main(void){
  string filename, outfilename, hipoutfilename;
  filename = "./dataset/tycho2_all.csv";
  string changename = "_rejected.csv";
  string hipchangename = "_hip.csv";

  outfilename = filename+changename;
  hipoutfilename = filename+hipchangename;

  //入出力ファイル先
  ifstream ifs (filename);
  ofstream ofs (outfilename);
  ofstream hip_ofs (hipoutfilename);

  //読み込んだ行数のカウント
  int count = 0, n_hip = 0, n_remain = 0;
  int check;
  string str;
  while(getline(ifs,str)){
    /* ファイルから得られる順序及び型は
     * [0:ra:double, 1:dec:double, 2:vt_mag:double, 3:bt_mag:double, 4:hip_num:unsigned int]
     */
     count++;
     string token;
     istringstream stream(str);
     //1行のうち、文字列とコンマを分割する
     int j = 0;
     check = 1;
     while(getline(stream,token,',')){
      switch(j){
        case 0:
          break;
        case 1:
          break;
        case 2: //vt_mag:double
          if(token.size() == 0) check = 0;
          break;
        case 3: //bt_mag:double
          if(token.size() == 0) check = 0;
          break;
        case 4: //hip_num:unsigned int
          if(token.size() != 0) check = 0;
          break;
        default: // 予定よりcolumnが多かったごときで処理を終わらせては(exit(0)しては)いけない
          cerr << "over columns :(" << j << ")" << endl;
          break;
      }
      j++;
    }
    if(check){
      n_remain++;
      // Hippalcos numがないとき
      // 且つ穴抜けがないとき
      ofs << str << endl;
    }else{
      n_hip++;
      //Hippalcos numが入っているとき
      // または穴抜けがあるとき
      hip_ofs << str << endl;
    }
  }
  //何行読みこんだかを出力しておく
  PRL(count); PRL(n_remain); PRL(n_hip);
  ofstream outputfile("./dataset/star_data.dat");
  outputfile << count << "\t" << n_hip << "\t" << n_remain << endl;
  outputfile .close();

  return 0;
}
