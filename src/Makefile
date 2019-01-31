CC = g++
CFLAGS = -Wall
PROG = test

winDir = "/usr/include/wine/windows/"

SRCS = main.cpp visuals.cpp BMPLoader.cpp OBJLoader.cpp

ifeq ($(shell uname),Darwin)
	LIBS = -framework OpenGL -framework GLUT
else
	LIBS = -lglut -lGLU
endif

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -I /usr/include/wine/windows/ -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
