all: test_radit

test_radit: test_radit.o
	$(CC) -o $@ $^ $(LDFLAGS) $(DEBUG) -lcheck
