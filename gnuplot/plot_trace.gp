reset
set size square
set grid
#set border lw 3 lc rgb "white"
#set grid lc rgb "white"
#set xtics textcolor rgb "white"
#set ytics textcolor rgb "white"

# set xr[-2.5:2.5]
# set yr[-2.5:2.5]
set xr[0:x_max]
set yr[0:y_max]
set term png size 5000,5000
dirname = "./plate_data"
# dirname = "./plate_data_margin"
filename = "./plate_data/hipp_0.dat"
set output "./plate_data/plot_trace.png"
# PNG形式の画像ファイルの大きさの設定

pl filename u 2:3 w l notitle, filename u 2:3 w p pt 7 ps 0.5 notitle
