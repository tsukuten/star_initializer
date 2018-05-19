#pragma once
#include "../include/star_class.hpp"
#define plate_system plate
class plate_system{
  //原板情報の管理
private:
  unsigned int  star_num;   //原板内の星の数
  unsigned int  plate_num;  //原板のナンバー
  star  * star_first;       //原板に入っている星への最初のポインタ
  plate * next_plate;       //次の原板へのポインタ
public:
  plate_system(){
    star_num  = 0;
    plate_num = 0;
    star_first = NULL;
    next_plate = NULL;
  }

  void set_nstar(int new_num)      {star_num = new_num;}
  void add_nstar(int new_num)      {star_num += new_num;}
  void set_nplate(int new_num)     {plate_num = new_num;}
  void set_starf(star * new_star)  {star_first = new_star;}
  void set_next(plate * new_plate) {next_plate = new_plate;}

  int get_nstar() {return star_num;}
  star * get_starf() {return star_first;}
  void assign_star_plate(int nstar, star * star_first, plate * plane_first);
  void create_plate_list(int nstar, star * star_first, plate * plane_first);
  void calc_position_in_plate(plate * plate_p);
  // void set_star_to_plate();
  void sort_in_plate_by_mesh(star * star_p, plate * plate_p);
};
