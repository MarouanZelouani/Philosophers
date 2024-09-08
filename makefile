NAME = philo
TOOLS = tools.a
CFILES =	./src/death.c\
			./src/init.c\
			./src/libft_utils.c\
			./src/parsing.c\
			./src/routine.c\
			./src/threads.c\
			./src/utils.c\
			./src/main.c\

OFILES = $(CFILES:.c=.o)

CC = cc
CFLAGS = #-3Wall -Wextra -Werror #-fsanitize=thread
AR = ac rc
RM = rm -rf	

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