main:
#	cc -std=c++0x -lstdc++ s2.cpp -o s2.out
#	cc -std=c++0x -lstdc++ test.cpp -o test.out && ./test.out < test-input.txt
#	cc -std=c++0x -lstdc++ s2.hpp s2.cpp testcase-s2.hpp testcase-s2.cpp -o s2.out && ./s2.out < test-input.txt
	cc -std=c++0x -lstdc++ s2.hpp s2.cpp s3.hpp s3.cpp testcase-s3.hpp testcase-s3.cpp -o s3.out && ./s3.out < test-input.txt