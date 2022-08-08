CFLAGS =  -Werror=implicit     \
		          		-Werror=implicit-function-declaration  \
				        -Werror=implicit-int \
						-Werror=int-conversion \
						-Werror=incompatible-pointer-types \
						-Werror=int-to-pointer-cast        \
						-fsanitize=address

CFILES = $(shell find src/ -name "*.c")

bin/kessedit: $(CFILES)
	mkdir -p bin/
	gcc $(CFLAGS) -Iinclude $^ -o $@
