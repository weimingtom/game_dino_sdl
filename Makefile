CC := gcc
CPP := g++
LD := ld
RM := rm -rf

CFLAGS := -O0 -g3
CPPFLAGS := -O0 -g3
LDFLAGS := -lSDL

OBJS := 
OBJS += AdafruitImageload.o Board.o DigitalIO.o dino.o
OBJS += Display.o DisplayIO.o sdl_api.o TextureLoader.o

all: dino
dino: $(OBJS)
	$(CPP)  -o $@ $(OBJS) $(LDFLAGS)

%.o : %.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS)
