NAME	=	minishell

CC		=	cc
FLAG	=	-Wall -Wextra -Werror -g3

DIRLIB	=	./Libft/
FILELIB	=	libft.a
NAMELFT	=	$(addprefix $(DIRLIB), $(FILELIB))

SRC_DIR	=	src/
OBJ_DIR	=	obj/
INCLUDE	=	-I ./include -I ./Libft
HEADER 	=	include/minishell.h

# Color
DEF_COLOR	= \033[0;39m
GRAY 		= \033[0;90m
RED 		= \033[0;91m
GREEN 		= \033[0;92m
YELLOW 		= \033[0;93m
BLUE		= \033[0;94m
MAGENTA		= \033[0;95m
CYAN		= \033[0;96m
WHITE		= \033[0;97m

# Source directories
SRC_SUBDIRS = builtin exec parsing utils
FILES	=	$(SRC_DIR)main.c $(foreach dir, $(SRC_SUBDIRS), $(wildcard $(SRC_DIR)$(dir)/*.c))

# Object files
OBJS	=	$(FILES:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

OBJF	=	.cache_exits

# Create object directories
$(OBJF) :
	@mkdir -p $(OBJ_DIR) $(addprefix $(OBJ_DIR), $(SRC_SUBDIRS))

# Add the path to the builtin folder to vpath
vpath %.c $(SRC_DIR) $(addprefix $(SRC_DIR), $(SRC_SUBDIRS))

all : $(NAME)

# Compile the libft library if not compiled yet
$(NAMELFT) :
	@make -C $(DIRLIB)

$(NAME) : $(OBJS) $(NAMELFT)
	$(CC) $(FLAG) $(OBJS) $(NAMELFT) -lreadline $(INCLUDE) -o $(NAME)
	@echo "$(GREEN)Minishell Compiled!$(DEF_COLOR)"

$(OBJ_DIR)%.o : %.c $(HEADER) Makefile | $(OBJF)
	$(CC) $(FLAG) $(INCLUDE) -c $< -o $@

clean :
	@make clean -sC $(DIRLIB)
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJF)
	@echo "$(MAGENTA)Minishell objects cleaned!$(DEF_COLOR)"

fclean : clean
	@make fclean -sC $(DIRLIB)
	@rm -rf $(NAME)
	@echo "$(YELLOW)Minishell cleaned!$(DEF_COLOR)"

re : fclean all
	@echo "$(GREEN)Cleaned and rebuilt!$(DEF_COLOR)"

.PHONY : all clean fclean re