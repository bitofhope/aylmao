CC ?= clang
CFLAGS += -std=c99 -Wall

aylmao-cli: obj/aylmao-cli.o obj/kone.o
	${CC} ${LDFLAGS} $^ -o $@

obj/%.o : src/%.c
	${CC} ${CFLAGS} $^ -c -o $@

%.o : obj/%.o

clean:
	rm obj/*
	rm aylmao-cli

