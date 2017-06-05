# Flags
CFLAGS = -I/usr/local/include -pedantic -O2  -Wno-deprecated -Wno-vla-extension

CC= g++
#LIBS := -lglfw3 -lopengl32 -lm -lGLU32 -lGLEW32
#LIBS := -lglfw -lGL -lm -lGLU -lGLEW
LIBS := -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm -lGLEW -L/usr/local/lib
EXT_LIBS := libassimp.3.1.1.dylib
OBJS = stb_image.o maratis_math.o plane_generator.o gplib.o

all: main

clean:
	@rm -rf $(OBJS)

main: ${OBJS} 
	$(CC) $(CFLAGS) $(OBJS) $(EXT_LIBS) -o main main.cpp $(LIBS)

stb_image.o: stb_image.cpp stb_image.h
maratis_math.o: maratis_math.cpp maratis_math.h
plane_generator.o: plane_generator.cpp plane_generator.h
gplib.o: gplib.cpp gplib.h