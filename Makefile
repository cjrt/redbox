CC = clang
OUTPUT = engine
SOURCES = main.c glad.c
INCLUDES = -Iinclude          # path to GLAD and GLFW headers
LIBS = -lglfw -ldl -lm -lGL   # Linux: link GLFW, OpenGL, math, dynamic loader

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(SOURCES) $(INCLUDES) $(LIBS) -o $(OUTPUT)

clean:
	rm -f $(OUTPUT)