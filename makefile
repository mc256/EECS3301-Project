main:
#	cc -std=c++0x -lstdc++ s2.cpp -o s2.out
#	cc -std=c++0x -lstdc++ test.cpp -o test.out && ./test.out < test-input.txt
	cc -std=c++0x -lstdc++ s2.hpp s2.cpp testcase-s2.cpp -o s2.out && ./s2.out < test-input.txt