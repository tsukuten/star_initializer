if(exist ("np") == 0 || np<0) np = n0

filename = dirname.sprintf("/plate_pos_%d.png",np)
set output filename
#loadname = dirname.sprintf("/tycho2_all_%d.dat",np)
#loadname = dirname.sprintf("/tycho2_all_hipp_%d.dat",np)
#loadname = dirname.sprintf("/tycho2_all_rej_%d.dat",np)
#loadname = dirname.sprintf("/tycho2_all_100_%d.dat",np)
#loadname = dirname.sprintf("/test_data_%d.dat",np)
loadname = dirname.sprintf("/hipp_%d.dat",np)

#単純なdot出力
# pl loadname u 1:2 w dot notitle,\
# pc(npenta,t),ps(npenta,t) lw 3 notitle

#tychopalcosの出力
#pl loadname u 1:2 w dot notitle, loadname2 u 1:2 w dot notitle

#等級をカラーバーで出力
#pl loadname2 u 1:2:7 w p pt 7 ps 1 palette notitle

#等級ごとに大きさを変えてプロット + 5角形のラインをプロット
pl loadname u 2:3:($4+10):4 w circle fs solid notitle palette,\
loadname u 2:3 w dot lc rgb "white" notitle
# pl loadname2 u 2:3 w p pt 7 ps 1 notitle
# pc(npenta,t),ps(npenta,t) lw 3 notitle
#pl loadname2 u 1:2:(1/($7+30)) w circle fc rgb "white" fs solid notitle


np = np+dn
if(np<nm)reread
