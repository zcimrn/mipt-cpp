test:
	diff --color -u vector.h <(clang-format -style="{BasedOnStyle: google, ColumnLimit: 0}" vector.h)
	clang++ -std=c++17 -I ../include -o vector_public_test vector_public_test.cpp

zip:
	rm -f vector.zip
	./vector_public_test
	zip vector.zip vector.h
