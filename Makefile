NAME = ircserv

CC = c++
CFLAGS = -Wall -Wextra -Werror
CXXFLAGS = -std=c++98

OBJ_DIR = obj/
INCLUDES = $(shell find src/*  -name  "*.hpp")

SRC_FILES = $(shell find src/*  -name  "*.cpp")

OBJ_FILES = $(SRC_FILES:%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJ_FILES) 
	$(CC) $(OBJ_FILES) -o $(NAME) $(CXXFLAGS)

$(OBJ_DIR)%.o: %.cpp $(INCLUDES)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re