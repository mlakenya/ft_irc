NAME		= ircserv

SRCS		= 	main.cpp 			\
				ServerManager.cpp 	\
				Client.cpp			\
				Server.cpp			\



DIR_SRCS	= srcs/

DIR_OBJS	= objs/

INCLUDES	=	Client.hpp 			\
				Server.hpp			\
				stdafx				\

OBJS		= ${SRCS:%.cpp=${DIR_OBJS}%.o}

CXX			= c++

DEP			= ${OBJS:%.o=%.d}

CPPFLAGS	= -Wall -Wextra -Werror -MMD -MP -g3 -D_GLIBCXX_DEBUG -std=c++98 -c -I includes/ 

RM 			= rm -f

all:	${NAME}

${NAME} : ${OBJS}  ${INCLUDES} 
	${CXX} $^ -o $@

${OBJS} : ${DIR_OBJS}%.o: ${DIR_SRCS}%.cpp
	mkdir -p ${@D}
	${CXX} ${CPPFLAGS} $< -o $@
-include ${DEP}

clean:
	${RM} -r ${DIR_OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re
