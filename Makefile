# Name of the executable
NAME = filter

# Compiler to use
CC = clang

# Compiler flags
CFLAGS = -Wall -Werror -Wextra -std=c11

# Linker flags (libraries)
LFLAGS = -lm

# Source files
SRCS = filter.c helpers.c

# Object files (automatically derived from source files)
OBJS = $(SRCS:.c=.o)

# Clean command
RM = rm -f

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

# Default target: build the executable
all: $(NAME)

# Rule to link object files into the final executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LFLAGS)

# Generic rule for compiling .c files to .o files
%.o: %.c helpers.h bmp.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	$(RM) $(OBJS)

# Clean object files and the executable
fclean: clean
	$(RM) $(NAME)

# Rebuild the project from scratch
re: fclean all

# Declare phony targets to prevent conflicts with files of the same name
.PHONY: all clean fclean re