# Name
NAME	= minishell
# Compiler and Flags
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g

# OS-specific flags
UNAME_S	:= $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	# macOS settings
	CPPFLAGS += -I/opt/homebrew/opt/readline/include
	LDFLAGS = -L/opt/homebrew/opt/readline/lib
else
	# Linux settings
	CPPFLAGS += -I/usr/include/readline
	LDFLAGS = -L/usr/lib
endif

LINKS = -L$(LIBFT_DIR) -lft -lreadline

# Libft
LIBFT_DIR		= libft/
LIBFT_NAME		= libft.a
LIBFT			= $(LIBFT_DIR)$(LIBFT_NAME)

# Includes
INC	=	-I ./includes/header_mig.h \
		-I ./libft/includes \
		$(CPPFLAGS)

# Source files
SRC_DIR		=	sources/
SRC_SUBDIRS =	$(shell find $(SRC_DIR) -type d)
SRC			=	$(SRC_DIR)/executor/executor.c \
				$(SRC_DIR)/executor/executor_heredoc.c \
				$(SRC_DIR)/executor/executor_pipe.c \
				$(SRC_DIR)/executor/executor_single.c \
				$(SRC_DIR)/executor/executor_single_utils.c \
				$(SRC_DIR)/lexer/lexer.c \
				$(SRC_DIR)/lexer/lexer_error_check.c \
				$(SRC_DIR)/lexer/lexer_quotes.c \
				$(SRC_DIR)/lexer/lexer_tokens.c \
				$(SRC_DIR)/lexer/lexer_utils_1.c \
				$(SRC_DIR)/lexer/lexer_utils_2.c \
				$(SRC_DIR)/parser/parser.c \
				$(SRC_DIR)/parser/parser_utils_1.c \
				$(SRC_DIR)/parser/parser_utils_2.c \
				$(SRC_DIR)/builtins/builtins_utils.c \
				$(SRC_DIR)/builtins/cd.c \
				$(SRC_DIR)/builtins/echo.c \
				$(SRC_DIR)/builtins/env.c \
				$(SRC_DIR)/builtins/exit.c \
				$(SRC_DIR)/builtins/export.c \
				$(SRC_DIR)/builtins/pwd.c \
				$(SRC_DIR)/builtins/unset.c \
				$(SRC_DIR)/error/error.c \
				$(SRC_DIR)/expander/expander_utils.c \
				$(SRC_DIR)/expander/expander_variables.c \
				$(SRC_DIR)/expander/expander.c \
				$(SRC_DIR)/utils/enviroment.c \
				$(SRC_DIR)/utils/free.c \
				$(SRC_DIR)/utils/init.c \
				$(SRC_DIR)/utils/signals.c \
				$(SRC_DIR)/utils/utils.c \
				$(SRC_DIR)/main.c

# Object files
OBJ_DIR = obj/
OBJ	= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)


# Build rules
all:			$(LIBFT) $(NAME)

# Compile object files from source files
$(OBJ_DIR)%.o:  $(SRC_DIR)%.c
				@mkdir -p $(dir $@)
				@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@ $(INC)

$(LIBFT):
				@echo "Making Libft..."
				@make -sC $(LIBFT_DIR)

$(NAME):		$(OBJ)
				@echo "Compiling Minishell..."
				@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(LDFLAGS) $(LINKS)
				@echo "Minishell ready."

clean_minishell:
				@echo "Removing Minishell object files..."
				@rm -rf $(OBJ_DIR)

clean_libft:
				@make clean -C $(LIBFT_DIR)

clean:			clean_minishell clean_libft

fclean_minishell: clean_minishell
				@echo "Removing Minishell..."
				@rm -f $(NAME)

fclean:			fclean_minishell
				@make fclean -C $(LIBFT_DIR)

re:				fclean all

# Phony targets represent actions not files
.PHONY: all clean fclean re

