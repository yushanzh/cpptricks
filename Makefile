CC=/opt/compiler/gcc-8.2/bin/g++

string_format_test:string_format_test.o
	$(CC) string_format_test.o -o string_format_test -pthread  -lfmt -lrt -lstdc++ -I /usr/include/ -L /usr/lib64
string_format_test.o:string_format_test.cpp
	$(CC) -std=c++11 -c  string_format_test.cpp  -pthread  -lfmt -lrt -lstdc++ -I /usr/include/ -L /usr/lib64


number_to_string : number_to_string.o
	$(CC) number_to_string.o -o number_to_string -pthread  -lfmt -lrt -lstdc++ -I /usr/include/ -L /usr/lib64
number_to_string.o:number_to_string.cpp
	$(CC) -std=c++11 -c number_to_string.cpp  -pthread  -lfmt -lrt -lstdc++ -I /usr/include/ -L /usr/lib64


clean:
	string_format_test.o string_format_test number_to_string.o number_to_string

