common_main.o: common_main.c solver.h
	gcc -c -Wall common_main.c solver.h

day1.o: day1.c
	gcc -c -Wall day1.c solver.h

day1: day1.c common_main.o solver.h
	gcc -o day1 day1.o common_main.o

day2: day2.c common_main.c solver.h
	gcc -Wall -o day2 day2.c

day3: day3.c common_main.c solver.h
	gcc -Wall -o day3 day3.c

day4: day4.c common_main.c solver.h
	gcc -Wall -o day4 day4.c

