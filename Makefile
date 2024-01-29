FILES = srcs\main.c srcs\parse_file.c srcs\utils.c
TARGET = main.exe
FLAGS = -ISDL2/include -LSDL2/lib -O3
COMPILFLAGS = -lmingw32 -lSDL2_image -lSDL2main -lSDL2
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