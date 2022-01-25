# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dhubleur <dhubleur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/08 10:05:58 by dhubleur          #+#    #+#              #
#    Updated: 2022/01/25 14:56:40 by dhubleur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#								  Utils										   #
################################################################################

NAME		= 	libunit.a
CC			= 	gcc
CFLAGS		=	-g3

RUN_PARAM	=

################################################################################
#								  Sources									   #
################################################################################

SRCS_EXTENSION	=	.c
SRCS_PATH		=	./srcs
SRCS			=	runner.c \
					printer.c \
					test_utils.c \
					stdout.c \
					malloc_hook.c \
					malloc_utils.c
					
###########################################s#####################################
#								  Includes									   #
################################################################################

INCLUDE_DIRS		=	./includes

#               /!\ Do not touch the rest of the file /!\ 

################################################################################
#								  Objects									   #
################################################################################

OBJS_PATH		=	./objs

OBJS			=	$(addprefix $(OBJS_PATH)/, ${SRCS:$(SRCS_EXTENSION)=.o})
OBJS_DEPEND		=	$(addprefix $(OBJS_PATH)/, ${SRCS:$(SRCS_EXTENSION)=.d})

################################################################################
#								  Constants									   #
################################################################################

BLUE		=	\033[1;34m
CYAN		=	\033[0;36m
GREEN		=	\033[0;32m
ORANGE  	=	\033[0;33m
NO_COLOR	=	\033[m

################################################################################
#								  Makefile									   #
################################################################################

INCLUDE_FLAGS 	=	$(addprefix -I , ${INCLUDE_DIRS})

ifeq (noflag, $(filter noflag,$(MAKECMDGOALS)))
	CFLAGS	+=	-Wall -Wextra
else
	CFLAGS	+=	-Wall -Wextra -Werror
endif

################################################################################
#								  Rules										   #
################################################################################

all:	header message $(NAME)

#beautiful
header:
		@echo -n "${BLUE}"
		@echo "  _  _ ___                    _         __ _ _      "
		@echo " | || |__ \                  | |       / _(_) |     "
		@echo " | || |_ ) |  _ __ ___   __ _| | _____| |_ _| | ___ "
		@echo " |__   _/ /  | '_ \` _ \ / _\` | |/ / _ \  _| | |/ _ \\"
		@echo "    | |/ /_  | | | | | | (_| |   <  __/ | | | |  __/"
		@echo "    |_|____| |_| |_| |_|\__,_|_|\_\___|_| |_|_|\___|"
		@echo "                                         by dhubleur"
		@echo -n "${NO_COLOR}"

message:
		@make -q $(NAME) && echo "$(GREEN)Everything is already up to date ! No work for me$(NO_COLOR)" || true

#Compilation
$(OBJS_PATH)/%.o:	$(SRCS_PATH)/%$(SRCS_EXTENSION)
			@mkdir -p $(dir $@)
			@echo "$(CYAN)Compiling $(BLUE)$@ ...$(NO_COLOR)"
			@$(CC) $(CFLAGS) -MMD -MF $(@:.o=.d)  ${INCLUDE_FLAGS} -c $< -o $@
			
#Link
-include $(OBJS_DEPEND)
$(NAME):	${OBJS}
		@echo "$(ORANGE)Linking $(BLUE)$@ ...$(NO_COLOR)"
		@cp libftprintf.a ${NAME}
		@ar rcs ${NAME} ${OBJS}
		@echo "$(GREEN)$@ created !$(NO_COLOR)"

#clean
clean:		header
		@rm -rf $(OBJS_PATH)
		@echo "$(GREEN)Removed objects !$(NO_COLOR)"

fclean:		header clean
		@rm -f $(NAME)
		@echo "$(GREEN)Removed $(NAME) !$(NO_COLOR)"

re:			header fclean all

#test
run:		header all
		@echo "$(BLUE)Executing...$(NO_COLOR)"
		@echo ""
		@./$(NAME) $(RUN_PARAM)

show:		header
		@echo "$(CYAN)Compilator: $(GREEN)$(CC)$(NO_COLOR)"
		@echo "$(CYAN)Flags: $(GREEN)$(CFLAGS)$(NO_COLOR)"
		@echo "$(CYAN)Includes: $(GREEN)$(INCLUDE_FLAGS)$(NO_COLOR)"
		@echo ""
		@echo "$(CYAN)Sources:$(NO_COLOR)"
		@echo "$(GREEN)$(MAIN) ${addprefix $(SRCS_PATH)/, $(SRCS)}$(NO_COLOR)"
		@echo ""
		@echo "$(CYAN)Libs:$(NO_COLOR)"
		@echo "$(GREEN)$(ALL_LIBS)$(NO_COLOR)"

#do nothing
noflag: 	all
		@echo -n ""
		
.PHONY:		all header clean fclean re run noflag show