//原板の枚数
#define PLATE_NUM 12

// 恒星データ数の定義
#define N_tycho_all 2539913
#define N_tycho_hip 119850
#define N_tycho_rem 2420063
#define N_hip_rem   117955

// 使う等級のカットオフ条件
// #define cutoff_mag 7.5

// ファイル内のNaNは0.0で書き換え
#define VT_MAG_NaN  100.0
#define BT_MAG_NaN  100.0
#define MAG_NaN     100.0
#define MAG_INIT   -100.0
#define HIP_NUM_NaN 0

// 5角形に関する角度情報
#define RADIAN (M_PI/180.0)      // 度 to radian変換用
#define penta_angle (360.0/5.0)  // 内角
#define alpha (acos(-1/sqrt(5))) // 2面角(radian)
#define half_penta_angle 36.0    // 内角の半角

// offset変換用のデフォルト値　ベテルギウスに関する角度 赤経赤緯はZY回転角に対応
#define offset_orion_y (-8.20163919)
#define offset_orion_z (78.63446353)

// マージン角の設定(度)　ここを実験して吟味する必要がある
#define margin_angle 1.0

// z軸に対するオフセット
#define H 30.0  // cm単位
#define R_SCALE 1.0
#define H_SCALE (H / R)
#define SMALL_R (R_SCALE - H_SCALE)

// 原板上の座標計算に使う定数群
#define R 500.0         // 投影距離[cm]
#define Focal_length 2.40
#define Magnification ((R - H) / Focal_length)
#define penta_side (((4.0*R) / (sqrt(15.0)+sqrt(3.0))) / Magnification)
#define rg (penta_side/(2.0*sin(half_penta_angle*RADIAN)))
// #define rg 2.50         // 恒星原版の半径[cm] 自分で設定する場合
#define proj_unit_r 3.0    // 投影ユニットの半径[cm]
// #define proj_angle 21.0 // 投影角度(半分で21°projection angle)
#define proj_angle 42.0 // 投影角（投影角は84°でその半分の42°で定義するべきだったのでは？）
#define mag0 2.0        // プロッター基準等級　2.0等級
#define r_spot 70.0     // スポット径２等級のときは0.07mm=70μmを基準にする
#define r_min 0.68      // 最小スポット径0.68μm
#define lambda (532.0/pow(10.0,3.0)) // レーザーの波長
#define loge (log10(exp(1.0)))       // 自然対数底に変換
#define z0 (M_PI*pow(r_min,2.0)/lambda)*sqrt(pow(70.0/0.68,2.0)-1.0) // 基準面の高さ
#define bang asin(2/((sqrt(15)+sqrt(3))*sin(36*RADIAN)))             // 外接球の中心角
#define F sin((penta_angle/2)*RADIAN)*tan(proj_angle*RADIAN)*cos(bang)*(sqrt(15)+sqrt(3))/2 // 圧縮の補正係数

// 経験則から求められた原版の穴の大きさと照射時間の関係
#define z_gradient (0.327)

// メッシュソートの刻み値
#define x_cell_length 1024
#define y_cell_length 16
