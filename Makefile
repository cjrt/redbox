CC      = clang
CFLAGS  = -Wall -Wextra -std=c11
INCLUDES= -Iinclude $(shell pkg-config --cflags assimp)   # add assimp includes

LIBS    = -lglfw -ldl -lm -lGL $(shell pkg-config --libs assimp)   # add assimp libs

SRC_DIR = src
OBJ_DIR = build
BIN     = redbox

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN)
