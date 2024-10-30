NAME = philo_bonus
TOOLS = tools_bonus.a
CC = cc
CFLAGS = #-Wall -Wextra -Werror #-fsanitize=thread
AR = ac rc
RM = rm -rf	
CFILES =	./src/init.c\
			./src/utils.c\
			./src/parsing.c\
			./src/start.c\
			./src/main.c\
			./src/libft_utils.c\

OFILES = $(CFILES:.c=.o)

$(NAME) :	$(OFILES)
			$(CC) $(CFLAGS) $(OFILES) -o $(NAME) 			

all :	$(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

re : fclean $(NAME)

clean : 
		$(RM) $(OFILES)

fclean : clean 
		$(RM) $(NAME)

.PHONY : all re clean fclean

.SECONDARY: $(OFILES)
# .SILENT: fclean clean