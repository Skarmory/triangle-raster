CC=g++
NAME=raster
FLAGS=-Wall
SRCS=raster.c
OBJS=raster.o

.PHONY: $(NAME) clean

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@  


clean:
	@rm -f $(NAME)
	@rm -f triangle.ppm
	@rm -f $(OBJS)
