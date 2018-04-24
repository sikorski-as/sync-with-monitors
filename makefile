default: main.cpp SharedBuffer.cpp
	g++ -ggdb -std=c++11 -pthread -o app \
		main.cpp \
		SharedBuffer.cpp 
	
run: default
	./app

clean:
	rm app
	rm *.txt
	del app
	del *.txt