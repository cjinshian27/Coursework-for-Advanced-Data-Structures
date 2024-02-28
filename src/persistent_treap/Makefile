
build: ./treap/main.cpp ./persistent-treap/main.cpp
	@g++ -g -o ./treap/main ./treap/main.cpp
	@g++ -g -o ./persistent-treap/main ./persistent-treap/main.cpp

run1: ./treap/main  
	@(./treap/main < ./treap/input)

run2: ./persistent-treap/main
	@./persistent-treap/main < ./persistent-treap/input 

clean:
	@rm -f ./treap/main ./persistent-treap/main
