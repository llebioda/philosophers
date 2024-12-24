CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCFILES = main.c utils.c philosopher.c philo_monitor.c ft_atol.c ft_strcmp.c
SRCDIR = ./
SRCS = $(addprefix $(SRCDIR), $(SRCFILES))
OBJDIR = ./build/
OBJS = $(addprefix $(OBJDIR), $(SRCFILES:.c=.o))
DEPS = $(OBJS:.o=.d)

HEADERDIR = ./
INCLUDES = -I $(HEADERDIR)

NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -MMD -MF $(@:.o=.d) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean
	@$(MAKE) --no-print-directory all

-include $(DEPS)

.PHONY: all clean fclean re
