test:
	diff --color -u array.h <(clang-format -style="{BasedOnStyle: google, ColumnLimit: 0}" array.h)
	clang++ -std=c++17 -I ../include -o array_public_test array_public_test.cpp

zip:
	rm -f array.zip
	zip array.zip array.h
