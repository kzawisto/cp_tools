

DEFAULT_PATH := $(shell if [ -z $${PYTHON_PREFIX+x} ]; then echo "$${INSTALL_PREFIX:-/usr}"; else python3 -c "import sys; print (sys.prefix)"; fi )

.PHONY: install clean all

all: test.bin perf_test.bin

test_perf.o : test_perf.cxx $(shell find include -type f -name "*.h")
	g++ --std=c++14 -O0 -c $< -o $@ -Iinclude

perf_test.bin : perftest/test_perf.o
	g++ --std=c++14 $^ -o $@ -lgtest -lpthread	

%.o : %.cxx $(shell find include -type f -name "*.h")
	g++ --std=c++14 -g -c $< -o $@ -Iinclude

test.bin: test/main.o test/eigen.o test/numeric.o test/test_time.o test/test_algo.o test/test_combined_variance.o
	g++ --std=c++14 $^ -o $@ -lgtest -lpthread	
	

install:
	cp -r include/* ${DEFAULT_PATH}/include

clean:
	find . \( -name "*.o" -or -name "*.bin" -and -not -path "*.git*" \) -exec rm {} \;
