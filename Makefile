CC=gcc -c -Wall
LD=gcc
OBJ_FILES=day1.o day2.o day3.o day4.o day5.o common_main.o
TARGETS=day1 day2 day3 day4 day5

common_main.o: common_main.c solver.h
	$(CC) $^

day1.o: day1.c solver.h
	$(CC) $^

day1: day1.o common_main.o
	$(LD) -o $@ $^

day2.o: day2.c solver.h
	$(CC) $^

day2: day2.o common_main.o
	$(LD) -o $@ $^

day3.o: day3.c solver.h
	$(CC) $^

day3: day3.o common_main.o
	$(LD) -o $@ $^

day4.o: day4.c solver.h
	$(CC) $^

day4: day4.o common_main.o
	$(LD) -o $@ $^

day5.o: day5.c solver.h
	$(CC) $^

day5: day5.o common_main.o
	$(LD) -o $@ $^

clean:
	\rm $(OBJ_FILES) $(TARGETS)
