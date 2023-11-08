all:
	gcc -Wall -Wextra -Werror game.c -o ballsort

debug:
	gcc -g game.c -o test 

clean:
	$(RM) ballsort test
