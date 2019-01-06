# radit

[![Build Status](https://travis-ci.org/dgkimura/radit.svg?branch=master)](https://travis-ci.org/dgkimura/radit)

Radit is a radix tree implementation.

## Build
```
$ make && ./test_radit
```

## Usage
Intialize a radix tree.

```c
#include "radit.h"

struct radit_tree t;
radit_init(&t);
```

Then add or search for items.

```c
radit_insert(&t, "key", "value");
void *value = radit_search(&t, "key"));
```
