UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	TEST_LIBS =-lcheck_pic -lpthread -lrt -lm
endif
ifeq ($(UNAME_S), Darwin)
	TEST_LIBS =-lcheck
endif

all: test_radit

test_radit: test_radit.o
	$(CC) -o $@ $^ $(TEST_LIBS)

clean:
	rm -f test_radit test_radit.o
