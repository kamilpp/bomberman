CC=gcc
LD=gcc

LIBS=`allegro-config --libs`
FLAGS=-std=c99 -fgnu89-inline

CFLAGS=$(FLAGS)
LDFLAGS=$(FLAGS) $(LIBS)

SRCS=main.c core.c stack.c func.c 
TARGET=Bomberman
OBJS=$(addprefix obj/, $(addsuffix .o, $(SRCS)))

$(TARGET): $(OBJS) 
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET)

obj/%.c.o: src/%.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm $(OBJS) $(TARGET)