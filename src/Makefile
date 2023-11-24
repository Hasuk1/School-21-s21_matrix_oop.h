CC = g++
CPPFLAGS = -lgtest -std=c++17
CHECKFLAGS = -Wall -Werror -Wextra

all: clean s21_matrix_oop.a test

test: test_program
	@./test_program

test_program: tests.o s21_matrix_oop.o
	@$(CC) -coverage $^ ${CPPFLAGS} ${CHECKFLAGS} -o $@
	

%.o: %.cc
	@$(CC) -std=c++17 ${CHECKFLAGS} -c $< -o $@

s21_matrix_oop.a: s21_matrix_oop.o
	@ar rcs $@ $^
	@ranlib $@

clean:
	@rm -rf *.o *.a test *.gcno *.gcda report *.info tests.out *.filtered main test_program

gcov_report: tests.cc s21_matrix_oop.cc
	@make clean
	@$(CC) -coverage $^ ${CPPFLAGS} ${CHECKFLAGS} -o test
	@./test
	@rm -rf tests.gcda tests.gcno
	@lcov --rc lcov_branch_coverage=1 -t "test_program" -o test.info -c -d ./
	@lcov --rc lcov_branch_coverage=1 -e test.info '*/s21_matrix_oop.cc' -o test.info.filtered
	@genhtml -o report test.info.filtered
	@open report/index.html

style:
	@cp ../materials/linters/.clang-format .
	@echo "===> Checking your code for Google Style <==="
	@clang-format -n *.cc *.h
	@rm ./.clang-format

clang:
	@cp ../materials/linters/.clang-format .
	@echo "===> Formatting your code for Google Style <==="
	@clang-format -i *.cc *.h
	@rm ./.clang-format

leaks: clean test
	@leaks -atExit -- ./test_program

valgrind: clean test
	@CK_FORK=no valgrind --vgdb=no --leak-check=full -s --show-leak-kinds=all --track-origins=yes --verbose ./test_program
