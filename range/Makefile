test:
	diff --color -u range.h <(clang-format -style="{BasedOnStyle: google, ColumnLimit: 0}" range.h)
	clang++ -std=c++17 -I ../include -o range_public_test range_public_test.cpp

zip:
	rm -f range.zip
	./range_public_test
	zip range.zip range.h
