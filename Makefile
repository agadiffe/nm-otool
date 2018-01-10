# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agadiffe <agadiffe@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/14 20:31:10 by agadiffe          #+#    #+#              #
#    Updated: 2017/12/23 17:30:50 by agadiffe         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ----------------------------------------------------------------------------
# VARIABLES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
NAME_NM = ft_nm
NAME_OTOOL = ft_otool

CC = clang
CFLAGS += -Wall -Werror -Wextra

SRC_PATH_NM = ./srcs/nm/
SRC_NAME_NM = ft_nm.c	\
			  get_type.c	\
			  handle_32_64_nm.c	\
			  handle_fat_nm.c	\
			  print_nm.c	\
			  sort_nlist.c

SRC_PATH_OTOOL = ./srcs/otool/
SRC_NAME_OTOOL = ft_otool.c \
				 handle_32_64_otool.c	\
				 handle_fat_otool.c	\
				 print_otool.c

OBJ_PATH = ./obj/
OBJ_NAME_NM = $(SRC_NAME_NM:.c=.o)
OBJ_NM = $(addprefix $(OBJ_PATH),$(OBJ_NAME_NM))
OBJ_NAME_OTOOL = $(SRC_NAME_OTOOL:.c=.o)
OBJ_OTOOL = $(addprefix $(OBJ_PATH),$(OBJ_NAME_OTOOL))

INC_PATH = ./includes/
INC = $(addprefix -I,$(INC_PATH))
INC_PATH_MACHO = ./includes/mach-o/
INC_MACHO = $(addprefix -I,$(INC_PATH_MACHO))
INC_FILE_NM = $(INC_PATH)$(NAME_NM).h
INC_FILE_OTOOL = $(INC_PATH)$(NAME_NM).h

LIBFT_PATH = ./libft/
LIBFT_NAME = libft.a
LIBFT_INC_PATH = ./libft/includes/
LIBFT = $(addprefix -L,$(LIBFT_PATH))
LIBFT_INC = $(addprefix -I,$(LIBFT_INC_PATH))

# ----------------------------------------------------------------------------
# OBJS |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
$(OBJ_PATH)%.o: $(SRC_PATH_NM)%.c $(INC_FILE_NM)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(LIBFT_INC) $(INC) $(INC_MACHO) -o $@ -c $<

$(OBJ_PATH)%.o: $(SRC_PATH_OTOOL)%.c $(INC_FILE_OTOOL)
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) $(LIBFT_INC) $(INC) $(INC_MACHO) -o $@ -c $<
# ----------------------------------------------------------------------------
# RULES ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
# ----------------------------------------------------------------------------
all: $(NAME_NM) $(NAME_OTOOL)

$(NAME_NM): $(LIBFT_PATH)$(LIBFT_NAME) $(OBJ_NM)
	$(CC) -o $@ $(OBJ_NM) $(LIBFT) -lft

$(NAME_OTOOL): $(LIBFT_PATH)$(LIBFT_NAME) $(OBJ_OTOOL)
	$(CC) -o $@ $(OBJ_OTOOL) $(LIBFT) -lft

$(LIBFT_PATH)$(LIBFT_NAME):
	@$(MAKE) -C $(LIBFT_PATH)

clean:
	@$(MAKE) -C $(LIBFT_PATH) clean
	@rm -rf $(OBJ_NM)
	@rm -rf $(OBJ_OTOOL)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@rm -rf $(NAME_NM)
	@rm -rf $(NAME_OTOOL)

re: fclean all

.PHONY: all clean fclean re
