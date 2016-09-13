CC ?= clang
CFLAGS += -std=c99 -Wall

all: kone.o

clean:
	rm obj/*

%.o: src/%.c
	${CC} ${CFLAGS} $^ -c -o obj/$@

