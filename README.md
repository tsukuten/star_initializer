Star initializer last updated in 2018.5.12
====

[Hipparcos catalog](https://heasarc.gsfc.nasa.gov/cgi-bin/W3Browse/w3query.pl?&tablehead=name%3Dheasarc%5Fhipparcos%26description%3DHipparcos+Main+Catalog%26url%3Dhttp%3A%2F%2Fheasarc%2Egsfc%2Enasa%2Egov%2FW3Browse%2Fstar%2Dcatalog%2Fhipparcos%2Ehtml%26archive%3D%26radius%3D1%26mission%3DSTAR%2BCATALOG%26priority%3D3&mission=STAR+CATALOG&Action=More+Options&Action=Parameter+Search&ConeAdd=1 "Hipparcos")
 と [tycho-2 catalog](http://cdsarc.u-strasbg.fr/viz-bin/Cat?I/259 "tycho-2") のデータを使って原板上でのプロット点データセットを作成する。

## Description
原板上でのプロット点データの作り方はいくつか変更することができる。


## Demo / How to use
実行ファイル（「star_list」）は make コマンドでカレントディレクトリ（「optics/」）下に生成される。
   * 第１引数でカットオフ等級（プロットする”最も暗い”等級）を指定可能。
   * 第２引数で読み込みファイル（元のカタログデータ）を指定可能。
```
$ make
$ ./star_list [Cutoff magnitude] [dastaset]
```
なおコンパイルに際して生成されるオブジェクトファイルは（「optics/object/」）下に生成される。
これらのオブジェクトファイルと実行ファイルは「make clean」で一括削除される。

## Requirement

## Usage
プログラムの計算内容を変更するには、「optics/include/calc_option.hpp」のマクロ定義を書き換える。

### Definition of macros
    * OPENMP: OpenMPによるスレッド並列化（default: 8threads）
    * OFFSET: 任意の緯度経度を天頂とする原板を生成する。
      - offset_angle_y offset_orion_y: 赤緯はy軸回転角に対応する。
      - offset_angle_z offset_orion_z: 赤経はｚ軸回転角に対応する。
    * MARGIN: レンズ同士の投影の重なりを補正した原板を生成する。
    * MARGIN_H: 北側と南側で投影機が分割していることによる投影面のズレを補正した原板を生成する。
    * GNUPLOT: gnuplotを使った原板の可視化を行う。
    * ANALITIC: 恒星のプロット径について、ステージの高さと照射時間について解析的な手法で得られた値を用いる。
    * CLASSIFY: 恒星のプロット順に関して、ステージの高さ方向の累積誤差を減らすために、等級ごとで分類された順番に従うようにする。
    * SORT_BY_MESH: 原板をメッシュに区切ってソートする。
    * SORT_BY_MESH_AND_MAG: メッシュでソートかつ等級でソートする。
    * SORT_BY_MESH_AND_MAG_RETURN: メッシュでソートかつ等級で折り返しソートする。
    * BASED_BY_PLOTTER: プロッター用のデータ出力を行う。
    * OUTPUT_CROSS: 四隅に目印として十字点を９つプロットするためのデータを出力しておく。

## Licence

## Author
Miyagawa Ginjiro(2013-2018 joined): ginmiya@ccs.tsukuba.ac.jp
