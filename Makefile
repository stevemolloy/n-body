CC = clang
CFLAGS = -O3 -Wall -Wpedantic -Wextra -std=c18 -ggdb
CINCLUDES = -I./thirdparty/raylib/include/ 
CLIBS = -Wl,-rpath=./thirdparty/raylib/lib/ -L./thirdparty/raylib/lib/ -l:libraylib.so -lm

SRC = src
OBJ = obj

SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BINDIR = bin
BIN = $(BINDIR)/nbody

all: $(BIN)

$(BIN): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CINCLUDES) $(CLIBS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CINCLUDES) -c $< -o $@

clean:
	rm -rf $(BINDIR) $(OBJ)

$(OBJ):
	@mkdir -p $@

run: all
	./bin/nbody

