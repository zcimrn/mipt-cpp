test:
	diff --color -u rational.h <(clang-format -style="{BasedOnStyle: google, ColumnLimit: 0}" rational.h)
	diff --color -u rational.cpp <(clang-format -style="{BasedOnStyle: google, ColumnLimit: 0}" rational.cpp)
	clang++ -std=c++17 -I ../include -o rational_public_test rational.cpp rational_public_test.cpp

zip:
	rm -f rational.zip
	zip rational.zip rational.h rational.cpp
