FLAGS = -Wall -Werror -Wextra  -g 
SRC = s21_containers/stack/stack.tpp s21_containers/vector/vector.tpp s21_containers/set/set.tpp s21_containersplus/multiset/Multiset.tpp s21_containersplus/Array/s21_array.tpp s21_containers/List/s21_list.tpp s21_containers/Map/s21_map.tpp
SRC_TEST = $(SRC) tests/ContainerTest.cc
OBJ = $(SRC:.cc =.o)

all: test

static_container.a:$(OBJ)
	ar rc static_container.a $(OBJ)
	ranlib static_container.a
	rm -rf *.o
	
test : 
	g++ -std=c++17 $(FLAGS) ContainerTest.cc -L.   -lgtest  --coverage -o test
	./test
gcov_report: test    
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info    
	open ./report/index.html
clean:    
	rm -rf *.o *.a *.gcno *.gcda test test.info *.out
	rm -rf report
	rm -rf .txt *.dSYM

	
