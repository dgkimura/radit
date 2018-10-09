all: test_radit

test_radit: test_radit.o
	$(CC) -o $@ $^ $(LDFLAGS) $(DEBUG) -pthread -lcheck

clean:
	rm -f test_radit test_radit.o
