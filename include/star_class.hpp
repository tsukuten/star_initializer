#pragma once
#include "../include/myvector.hpp"

class star{
  private:
    unsigned int hip_num;     //ヒッパルコスナンバー
    unsigned int my_num;      //天文研ナンバー
    unsigned int plate_num;   //原板ナンバー
    string plate_name;        //原板探索の結果
    vector position;          //3次元ベクトル(r,θ,φ)
    vector position_xyz;      //直交座標系での位置(x,y,z)
    vector position_rot;      //回転後のベクトル(x,y,z)でも(r,θ,φ)でもよいが
    vector position_plate;    //原板上の位置
    double spot;             // スポット径
    double vtmag;            //V等級
    double btmag;            //B等級
    double vismag;           //視等級
    double vismag_classified;  // 視等級を段階的に分類したもの
    int    layer_index;  //　視等級を段階的に分類したもののインデックス
    double ray_time;     //照射時間
    star * pre_star;    //前の星へのポインタ
    star * next_star;         //次の星へのポインタ
  public:
    star(){
      hip_num = 0;
      my_num = 0;
      plate_num = 0;
      position = 0.0;
      position_xyz = 0.0;
      position_rot = 0.0;
      position_plate = 0.0;
      vtmag = 0.0;
      btmag = 0.0;
      vismag = MAG_INIT;
      vismag_classified = 0.0;
      layer_index = 0;
      next_star = NULL;
      pre_star = NULL;
      plate_name = "";
    }
    unsigned int  get_hipnum(){return hip_num;}
    unsigned int  get_mynum() {return my_num;}
    unsigned long get_plate_num() {return plate_num;}
    vector  get_pos()   {return position;}
    vector* get_pos_p() {return &position;}
    vector  get_posxyz()   {return position_xyz;}
    vector* get_posxyz_p() {return &position_xyz;}
    vector  get_posrot()   {return position_rot;}
    vector* get_posrot_p() {return &position_rot;}
    vector  get_posplate()   {return position_plate;}
    vector  get_posplate_nm() {
      vector offset_plotter;
      offset_plotter[0] = proj_unit_r;
      offset_plotter[1] = proj_unit_r;
      offset_plotter[2] = 0.0;
      return 10000000.0*(position_plate + offset_plotter);
    }
    long get_posplate_nm_x(){
      return 10000000.0*(position_plate[0] + proj_unit_r);
    }
    long get_posplate_nm_y(){
      return 10000000.0*(position_plate[1] + proj_unit_r);
    }
    long get_posplate_nm_z(){
      return 10000000.0*(position_plate[2]);
    }

    vector* get_posplate_p() {return &position_plate;}
    double  get_x_plate() {return position_plate[0];}
    double  get_y_plate() {return position_plate[1];}
    double  get_r()   {return position[0];}
    double  get_ra()  {return position[1];}
    double  get_dec() {return position[2];}
    double  get_xyradii() {return sqrt(position_xyz[0]*position_xyz[0]+position_xyz[1]*position_xyz[1]);}
    double  get_x() {return position_xyz[0];}
    double  get_y() {return position_xyz[1];}
    double  get_z() {return position_xyz[2];}
    double  get_r_rot()   {return position_rot[0];}
    double  get_ra_rot()  {return position_rot[1];}
    double  get_dec_rot() {return position_rot[2];}
    double get_vtmag() {return vtmag;}
    double get_btmag() {return btmag;}
    double get_vismag() {return vismag;}
    double get_spot() {return spot;}
    double get_ray_time() {return ray_time;}
    string get_plate_name() {return plate_name;}
    star* get_next() {return next_star;}
    star* get_pre() {return pre_star;}

    void set_hipnum(unsigned int N)     {hip_num = N;}
    void set_mynum(unsigned int N)      {my_num = N;}
    void set_plate_num(unsigned int N)  {plate_num = N;}
    void set_plate_name(string N)       {plate_name += N;}
    void set_pos(vector new_vec)        {position = new_vec;}
    void set_posxyz(vector new_vec)     {position_xyz = new_vec;}
    void set_posrot(vector new_vec)     {position_rot = new_vec;}
    void set_r(double new_r)            {position[0] = new_r;}
    void set_ra(double new_ra)          {position[1] = new_ra;}
    void set_dec(double new_dec)        {position[2] = new_dec;}
    void set_vtmag(real new_vtmag)      {vtmag = new_vtmag;}
    void set_btmag(real new_btmag)      {btmag = new_btmag;}
    void set_vismag(){
      if(vtmag == VT_MAG_NaN || btmag == BT_MAG_NaN)
        vismag = MAG_NaN;
      else
        vismag = vtmag + 0.008-0.0988*(btmag-vtmag);
    }
    void set_vismag_2(){
      if(vtmag == VT_MAG_NaN || btmag == BT_MAG_NaN){
        vismag = MAG_NaN;
      } else {
        vismag = vtmag - 0.09*(btmag-vtmag);
        set_classified_vismag(vismag);
      }
    }
    void set_vismag_direct(double new_vismag) {vismag = new_vismag;}
    void set_classified_vismag(double new_vismag) {vismag_classified = (new_vismag);}
    double  get_classified_vismag() {return vismag_classified;}
    void set_layer_index(int new_index) {layer_index = new_index;}
    int  get_layer_index() {return layer_index;}
    void set_next(star * new_star)      {next_star = new_star;}
    void set_pre(star * new_star)      {pre_star = new_star;}
    void set_posplate();
    void set_spot(double new_spot){
      spot = new_spot;
    };
    void set_z_t_relation(){
      // ここではcm単位
      position_plate[2] = spot/(z_gradient*10000.0);
      // 時間はマイクロ秒単位
      ray_time = 10000.0*position_plate[2]*2.0;
    }
    int include_star_data(int nstar, star * star_first, int data_num, double cut_mag);
    // void assign_star(int nstar, star * star_first);
};
