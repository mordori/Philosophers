# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 16:50:13 by myli-pen          #+#    #+#              #
#    Updated: 2025/09/01 18:54:23 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=philo

CC			=cc
CFLAGS		=-Wall -Wextra -Werror -Wunreachable-code -O3
LDFLAGS		=-pthread
MAKEFLAGS	+= --no-print-directory -j$(shell nproc)

DIR_INC		=inc/
DIR_SRC		=src/
DIR_OBJ		=obj/
DIR_LIB		=lib/
DIR_DEP		=dep/

INCS		=$(addprefix -I , \
				$(DIR_INC))
SRCS		=$(addprefix $(DIR_SRC), \
				main.c errors.c philosopher.c simulation.c parsing.c \
				simulation_utils.c)
OBJS		=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

BLUE		=\033[1;34m
YELLOW		=\033[1;33m
GREEN		=\033[1;32m
RED			=\033[1;31m
COLOR		=\033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS)
	@echo "$(YELLOW) [✔] $(NAME) created$(COLOR)"

$(DIR_OBJ)%.o: $(DIR_SRC)%.c
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@if [ -d "$(DIR_OBJ)" ]; then \
		rm -rf $(DIR_OBJ) $(DIR_DEP); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_OBJ)"; \
		echo "$(RED) [-]$(COLOR) removed $(DIR_DEP)"; \
	fi

fclean: clean
	@if [ -e "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(RED) [-]$(COLOR) removed $(NAME)"; \
	fi

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OBJS) $(DEPS)

-include $(DEPS)
