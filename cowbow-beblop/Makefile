cppver = 17
files = main.cpp

a.out: main.cpp
	g++ --std=c++$(cppver) $(files)

dbg.out: main.cpp
	g++ -g --std=c++$(cppver) $(files) -o dbg.out

run: ./a.out
	./a.out

debug: ./dbg.out
	gdb ./dbg.out
