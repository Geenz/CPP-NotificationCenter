CXX=g++ -std=c++0x -Wall -Werror
BUILD_DIR=./build

default: notification_center_example

NotificationCenter.o: NotificationCenter.cpp NotificationCenter.hpp
	$(CXX) -c $^

main.o: main.cpp NotificationCenter.hpp
	$(CXX) -c $^

notification_center_example: NotificationCenter.o main.o
	$(CXX) $^ -o $@
	rm -f NotificationCenter.o
	rm -f main.o
	rm -f NotificationCenter.hpp.gch

clean:
	rm -f notification_center_example
