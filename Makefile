CC = clang
OUTPUT = greybox
SOURCES = src/main.c src/glad.c
INCLUDES = -Iinclude
LIBS = -lglfw -ldl -lm -lGL

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(SOURCES) $(INCLUDES) $(LIBS) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)