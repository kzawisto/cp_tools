

DEFAULT_PATH := $(shell if [ -z $${PYTHON_PREFIX+x} ]; then echo "$${INSTALL_PREFIX:-/usr}"; else python3 -c "import sys; print (sys.prefix)"; fi )


%.o : %.cxx
	g++ --std=c++14 -c $^ -o $@ -Iinclude

test.bin: test/main.o test/eigen.o
	g++ --std=c++14 $^ -o $@ -lgtest -lpthread	
	

.PHONY: install clean
install:
	cp -r include/* ${DEFAULT_PATH}/include

clean:
	find . \( -name "*.o" -or -name "*.bin" -and -not -path "*.git*" \) -exec rm {} \;
