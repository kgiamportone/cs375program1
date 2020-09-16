FLAGS = -g -Wall -Werror -std=c++14
RUNNAME = program1
.PHONY: program1 clean run

program1: MaxProfit.o
	g++ $(FLAGS) MaxProfit.o -o program1

MaxProfit.o: MaxProfit.cpp
	g++ -c $(FLAGS) MaxProfit.cpp -o MaxProfit.o

run: program1
	./$(RUNNAME)

clean:
	rm *.o $(RUNNAME)

memcheck: program1
	valgrind --leak-check=yes ./$(RUNNAME)
