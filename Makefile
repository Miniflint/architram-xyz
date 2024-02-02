FILES = srcs\main.c srcs\parse_file.c srcs\utils.c srcs\image_utils.c srcs\init.c srcs\handle_things.c
TARGET = main.exe
FLAGS = -ISDL2/include -LSDL2/lib -Wall -Wextra -Werror -g3 -O3
COMPILFLAGS = -lmingw32 -lSDL2_image -lSDL2main -lSDL2_ttf -lSDL2
OBJS = ${FILES:.c=.o}
CC = gcc

%.o: %.c
	@echo "Generating $@"
	@${CC} ${FLAGS} -c $< -o $@

${TARGET}: ${OBJS}
	@echo "generating executable"
	@${CC} ${FLAGS} -o ${TARGET} ${OBJS} ${COMPILFLAGS}

all: ${TARGET}

clean:
	@echo "Cleaning object files"
	@del ${OBJS}

fclean: clean
	@echo "Cleaning executable"
	@del ${TARGET}

re: fclean all

.PHONY: fclean clean re all