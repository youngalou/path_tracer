# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lazuli <lazuli@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/16 15:53:08 by lazuli            #+#    #+#              #
#    Updated: 2018/02/20 16:20:39 by lazuli           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = path_tracer
CFLAG = -Wall -Werror -Wextra
MLXFLAG = -framework OpenGL -framework AppKit
MLX = mac-mlx/libmlx.a

FILES = main.c \
		vector.c \
		scene.c \
		halton.c \
		utils.c \

SRC = $(addprefix src/, $(FILES))

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(MLX) $(OBJ)
	@gcc -o $(NAME) $(CFLAG) $(OBJ) $(MLX) $(MLXFLAG)
	@echo "\033[32m- $(NAME) executable compiled\033[0m"

$(OBJ): %.o: %.c path_tracer.h
	gcc -c $(CFLAG) $< -o $@

$(MLX):
	@make -C mac-mlx
	@echo "\033[32m- libmlx.a compiled\033[0m"

clean:
	@rm -f $(OBJ)
	@echo "\033[31m- $(NAME) object files removed\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[31m- $(NAME) executable removed\033[0m"
	@make -C mac-mlx clean
	@echo "\033[31m- libmlx.a removed\033[0m"

re: fclean all