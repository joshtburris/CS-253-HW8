# To compile your code, do this command: make
# To produce a tar file, do this: make tar
# To test your tar file: make test
# To clean up: make clean

# You are required to at least use -Wall for warnings.
# More -W flags are even better.
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -O3
#-Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option -fstack-protector-all -g -O9 -Wunused-const-variable=1 -Wzero-as-null-pointer-constant -Wduplicated-cond -Wduplicated-branches -Walloc-zero -Walloca
assignment=hw8
out=$(assignment).out
lib=$(assignment).a
ofiles=Alpha.o PGM.o Image.o
ccfiles=Alpha.cc PGM.cc Image.cc

$(lib): $(ofiles)
	$(AR) -rcs $@ $?

mydefaulttesting: $(ofiles) main.o
	$(CXX) $(CXXFLAGS) -o $(out) $(ofiles) main.o
	clear
	./$(out)


main.o:: main.cc
	g++ $(CXXFLAGS) -c main.cc
Image.o:: Image.cc Image.h
	g++ $(CXXFLAGS) -c Image.cc
Alpha.o:: Alpha.cc Alpha.h Image.h
	g++ $(CXXFLAGS) -c Alpha.cc
PGM.o:: PGM.cc PGM.h Image.h
	g++ $(CXXFLAGS) -c PGM.cc


test:
	cd test_directory
	cp $(assignment).tar test_directory
	tar -x <$(assignment).tar
	make
	cp test_directory/main.cc .
	g++ -Wall main.cc hw5.a
	./a.out

tar:
	tar -cv $(MAKEFILE_LIST) $(ccfiles) *.h >$(assignment).tar

library:
	ar -rcs $(lib) $(ofiles)

clean:
	rm -rf .test-dir $(out) $(assignment).{a,tar} *.o *.gch
