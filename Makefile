# コンパイラの種類を指定　基本は g++ で大丈夫
COMPILE = g++

# OpenMP を使うためのコンパイルオプション　ソースコードで OpenMP を無効にすれば必要ない
OPENMP = -fopenmp

# かなり厳しいエラー報告をしてくるコンパイルオプション　デバッグ作業時には役立つ　普段は無効でOK
# WARNING = -wall

# 実行ファイルがどの関数にどれくらい時間がかかっていて、
# どの関数がどれくらい呼び出されているのか等を計測してくれるコンパイルオプション
# -pg つけてコンパイルして実行ファイル生成後　./実行ファイル名　でプログラムを実行
# その後実行ファイルと同じディレクトリには gmon.out ファイルが生成されるので、
# gprof 実行ファイル名　で上記の情報が得られる
# DEBUG = -pg

# その他のコンパイルオプション
# それぞれ　「コンパイラのversion」「最高位の最適化オプション」「for文などに関して無駄なループを取り払う最適化オプション」
# 「数学関数の実行を早くする最適化オプション」「マシンのCPUに合わせて最適化するオプション」
CFLAGS = -std=gnu++11 -O3 -funroll-loops -ffast-math -march=native $(OPENMP) $(WARNING) $(DEBUG)

# リンクオプションの設定　基本いじらなくてOK
ifeq "$(shell getconf LONG_BIT)" "64"
	LDFLAGS =
else
	LDFLAGS =
endif

# ほかに使いたいライブラリがあるときはここに追加すればいい
LIBS =

# ファイル置き場のディレクトリ指定　
# それぞれ「ヘッダーファイル.hpp 置き場」「実行ファイル　置き場」「ソースファイル.cpp 置き場」
INCLUDE = -I./include
TARGET = ./star_list
SRCDIR = ./source

# ソースコード置き場を指定しない場合はカレントディレクトリに指定
ifeq "$(strip $(SRCDIR))" ""
	SRCDIR = .
endif

# ここに作ったソースファイルを shell コマンドで追加していけばいい
SOURCES = $(shell ls $(SRCDIR)/eigen_vector.cpp)\
$(shell ls $(SRCDIR)/translation.cpp)\
$(shell ls $(SRCDIR)/sort.cpp)\
$(shell ls $(SRCDIR)/plate.cpp)\
$(shell ls $(SRCDIR)/star.cpp)\
$(shell ls $(SRCDIR)/output.cpp)\
$(shell ls $(SRCDIR)/main.cpp)\

# オブジェクトファイル置き場がないときはカレントディレクトリに指定
OBJDIR = ./object
ifeq "$(strip $(OBJDIR))" ""
	OBJDIR = .
endif

# ターゲットである実行ファイルが依存するオブジェクトファイルを指定
# SOURCES で指定したソースファイルから生成されたオブジェクトをすべて使用することを想定している
# subset を使用して、SRC_DIR から OBJ_DIR に変換し、拡張子も .cpp から .o に変換する
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

# 依存関係を示す中間ファイルの指定
DEPENDS = $(OBJECTS:.o=.d)

# 実行ファイルコンパイル
$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILE) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# オブジェクトファイル生成
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILE) $(CFLAGS) $(INCLUDE) -o $@ -c $<

# make all で実行ファイルだけを削除できる
all: clean $(TARGET)

# make clean でオブジェクトファイル、中間ファイル、実行ファイルを削除できる　
clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)

# SOURCES 内のソースファイルの依存関係が明記された .d ファイルを読み込む
# これによって header のみが更新された場合でも make が実行できる
-include $(DEPENDS)

# ファイルを生成しないターゲットを明記
.PHONY: all clean
