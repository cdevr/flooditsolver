all:
	clang -O3 solve.cpp -o solve -lstdc++

debug:
	clang -g solve.cpp -o solve -lstdc++
