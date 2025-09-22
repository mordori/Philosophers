# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: myli-pen <myli-pen@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/30 16:50:13 by myli-pen          #+#    #+#              #
#    Updated: 2025/09/22 16:27:50 by myli-pen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=philo

QUEUE_SIZE	?=1024
CONF		:=.config

CC			:=cc
DEFS		:=-D QUEUE_SIZE=$(QUEUE_SIZE)
CFLAGS		:=-Wall -Wextra -Werror -Wunreachable-code $(DEFS) -O3 -march=native -funroll-loops -fno-plt
LDFLAGS		:=-pthread -flto
MAKEFLAGS	+= --no-print-directory

DIR_INC		:=inc/
DIR_SRC		:=src/
DIR_OBJ		:=obj/
DIR_LIB		:=lib/
DIR_DEP		:=dep/

INCS		:=$(addprefix -I , \
				$(DIR_INC))
SRCS		:=$(addprefix $(DIR_SRC), \
				main.c errors.c philosopher.c simulation.c parsing.c \
				simulation_utils.c timing.c logging.c string_utils.c)
OBJS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_OBJ)%.o, $(SRCS))
DEPS		:=$(patsubst $(DIR_SRC)%.c, $(DIR_DEP)%.d, $(SRCS))

BLUE		:=\033[1;34m
YELLOW		:=\033[1;33m
GREEN		:=\033[1;32m
RED			:=\033[1;31m
COLOR		:=\033[0m

all: config $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	@echo "$(YELLOW) [✔] $@ built with QUEUE_SIZE=$(QUEUE_SIZE)$(COLOR)"

config:
	@if [ ! -f "$(CONF)" ] || [ "$$(cat "$(CONF)")" != "$(QUEUE_SIZE)" ]; then \
		echo "$(QUEUE_SIZE)" > "$(CONF)"; \
	elif [ -e "$(NAME)" ]; then \
		echo "$(NAME) is already configured with QUEUE_SIZE=$(QUEUE_SIZE)."; \
	fi

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(CONF)
	@mkdir -p $(dir $@) $(patsubst $(DIR_OBJ)%, $(DIR_DEP)%, $(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(patsubst $(DIR_OBJ)%.o, $(DIR_DEP)%.d, $@) $(INCS)
	@echo "$(GREEN) [+]$(COLOR) compiling $@"

clean:
	@if [ -d "$(DIR_OBJ)" ]; then \
		rm -rf $(DIR_OBJ); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_OBJ)"; \
	fi

fclean: clean
	@if [ -d "$(DIR_DEP)" ]; then \
		rm -rf $(DIR_DEP); \
		echo "$(RED) [-]$(COLOR) removed $(DIR_DEP)"; \
	fi
	@if [ -e "$(CONF)" ]; then \
		rm -f $(CONF); \
		echo "$(RED) [-]$(COLOR) removed $(CONF)"; \
	fi
	@if [ -e "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(RED) [-]$(COLOR) removed $(NAME)"; \
	fi

re: fclean all

.PHONY: all clean fclean re config
.SECONDARY: $(OBJS) $(DEPS) $(CONF)

-include $(DEPS)
