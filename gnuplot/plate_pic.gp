reset
set size square
set grid
#set border lw 3 lc rgb "white"
#set grid lc rgb "white"
#set xtics textcolor rgb "white"
#set ytics textcolor rgb "white"

#set xr[-2.5:2.5]
#set yr[-2.5:2.5]
# set xr[0:5]
# set yr[0:5]
set xr [0:x_max]
set yr [0:y_max]
set term png
dirname = "./plate_data"
# dirname = "./plate_data_margin"
# PNG形式の画像ファイルの大きさの設定
set term png size 5000,5000
# set label cb font 'Times,30'
#set object rectangle from screen 0,0 to screen 1,1 behind fc rgb "black"

# for文回すための変数
n0=0
nm=12
dn=1

# 5角形プロットするための定数
# npenta = 5
# set param
# set tr[0:2*pi]
# s1(x)=x-floor(x)
# a(npenta)=2e0*pi/real(npenta)
# cnp(npenta,t)=cos(a(npenta)*s1(t/a(npenta))-0.5e0*a(npenta))
# pc(npenta,t)=(2/0.81)*cos(t)*cos(0.5e0*a(npenta))/cnp(npenta,t)
# ps(npenta,t)=(2/0.81)*sin(t)*cos(0.5e0*a(npenta))/cnp(npenta,t)
# set samples npenta*100+1

load './gnuplot/plate_pic.plt'

np=0
