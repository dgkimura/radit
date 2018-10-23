UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	TEST_LIBS =-lcheck_pic -lpthread -lrt -lm
endif
ifeq ($(UNAME_S), Darwin)
	TEST_LIBS =-lcheck
endif

all: radit test_radit

radit:
	$(CC) -c radit.c -o $@.o ${LIBS}
	$(AR) rcs $@.a $@.o

test_radit: radit
	$(CC) test_radit.c radit.a -o test_radit $(TEST_LIBS)

clean:
	rm -f test_radit *.o *.a
