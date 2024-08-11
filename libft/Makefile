# Standard
NAME = libft.a

# Compiler & Flags
CC	= cc
CFLAGS	= -Wall -Wextra -Werror -I
RM	= rm -f

# Directories
INC		= includes/
OBJ_DIR	= obj/
LIBFT_DIR	= libft_func/
FT_PRINTF_DIR	= ft_printf/
GNL_DIR		= get_next_line/

# Source files
LIBFT_SRCS	=	$(wildcard libft_func/*.c)
FT_PRINTF_SRCS	=	$(wildcard ft_printf/*.c)
GNL_SRCS	=	$(wildcard get_next_line/*.c)

# Object files
LIBFT_OBJS	=	$(LIBFT_SRCS:$(LIBFT_DIR)%.c=$(OBJ_DIR)%.o)
FT_PRINTF_OBJS	=	$(FT_PRINTF_SRCS:$(FT_PRINTF_DIR)%.c=$(OBJ_DIR)%.o)
GNL_OBJS	=	$(GNL_SRCS:$(GNL_DIR)%.c=$(OBJ_DIR)%.o)


#SRC	=	$(LIBFT_SRCS) $(FT_PRINTF_SRCS) $(GNL_SRCS)
OBJS = $(LIBFT_OBJS) $(FT_PRINTF_OBJS) $(GNL_OBJS)

# Build rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "Creating $(NAME)..."
	@ar rc $(NAME) $(OBJS)
	@echo "libft is ready."

$(OBJ_DIR)%.o: $(LIBFT_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
$(OBJ_DIR)%.o: $(FT_PRINTF_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
$(OBJ_DIR)%.o: $(GNL_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(OBJS): | pre-compile-message

pre-compile-message:
	@echo "Compiling libft..."

clean:
	@echo "Removing libft object files..."
	@$(RM) $(OBJ_DIR)*.o

fclean: clean
	@echo "Removing $(NAME)..."
	@$(RM) $(NAME)

re: fclean all

.PHONY:  all clean fclean re
