# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vkuzmin <vkuzmin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/09 17:09:43 by egeorgel          #+#    #+#              #
#    Updated: 2023/03/10 20:15:05 by vkuzmin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
FILES = main.c \
error.c \
builtins/mini_echo.c \
builtins/mini_env.c \
builtins/mini_export.c \
builtins/mini_pwd.c \
builtins/mini_unset.c \
builtins/mini_cd.c \
parsing/create_env.c \
parsing/sep_token.c \
piping/redirection.c \
piping/pipes.c \
utils/utils.c \
signals/signals.c \
builtins/mini_exit.c

SRCS = $(addprefix srcs/, $(FILES))
RDLINE = libs/readline
OBJ = ${SRCS:.c=.o}
HDR = includes/minishell.h
NAME = minishell
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
LFLAGS = -Llibs/libft -Llibs/readline/lib -lft -lreadline -I$(RDLINE)/include

all: $(NAME)

$(RDLINE):
	bash install_readline.sh

$(NAME): $(RDLINE) $(OBJ) $(HDR) Makefile
	cd libs/libft ; make bonus
	$(CC) $(LFLAGS) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)
	cd libs/libft ; make clean

fclean: clean
	rm -f $(NAME)
	rm -f libs/libft/libft.a
	rm -fR libs/readline

re: fclean all

.c.o: $(LIB) $(HDR) Makefile
	$(CC) $(CFLAGS) -I$(RDLINE)/include -c -o $@ $<

.PHONY: all clean fclean re