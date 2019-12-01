CC := cc
CFLAGS := -Os -Wall -Werror -Wextra -pedantic -Isrc -flto

C_FILES   := $(wildcard src/*.c)
OBJ_FILES := $(patsubst src/%.c, bin/%.o, $(C_FILES))

bin/unbird: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o "$@" $^

bin/%.o: src/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c -o "$@" "$<"

run: bin/unbird
	@"$<"
.PHONY: run

clean:
	@rm -rf bin
.PHONY: clean
