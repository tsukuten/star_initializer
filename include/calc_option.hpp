// OpenMP 並列化して計算
#define OPENMP
  #ifdef OPENMP
    #include <omp.h>
  #endif

// offset角を中心にしたときの原板作成 MARGIN_H とは共用不可
#define OFFSET
  #ifdef OFFSET
    // 例えばオリオンの場合はoffset_orion_*を代入すればよい
    #define offset_angle_y offset_orion_y
    #define offset_angle_z offset_orion_z
  #endif

// Marginをとって原板探査
// #define MARGIN

// z軸方向にHだけシフトさせた場合の原版作成　ここが無効だと、同一中心光源の原版が出来上がる
// #define MARGIN_H

// gnuplotによる画像出力を行う
#define GNUPLOT

// 解析的に求めたステージの高さを利用する場合はここを有効にする
// #define ANALITIC

// 段階的に分類された等級を用いてzとtを決定する
#define CLASSIFY

// メッシュでソートする
#define SORT_BY_MESH

// メッシュでソートかつ等級でソートする
#define SORT_BY_MESH_AND_MAG

// メッシュでソートかつ等級で折り返しソートする
#define SORT_BY_MESH_AND_MAG_RETURN

// プロッター用のデータ出力を行う
#define BASED_BY_PLOTTER

// 四隅に目印として十字点を９つプロットするためのデータを出力しておく
#define OUTPUT_CROSS
