#include <stdlib.h>

#include "radit.h"

struct leaf
{
    void *value;
};

struct node
{
};

struct node_4
{
    /* keys are sorted */
    unsigned char keys[4];

    /* stores between 1 - 4 children*/
    struct node *children[4];
};

struct node_16
{
    /* keys are sorted */
    unsigned char keys[16];

    /* stores between 5 - 16 children*/
    struct node *children[16];
};

struct node_48
{
    /* keys are sorted */
    unsigned char keys[48];

    /* stores between 17 - 48 children*/
    struct node *children[48];
};

struct node_256
{
    /* keys are sorted */
    unsigned char keys[256];

    /* stores between 49 - 256 children*/
    struct node *children[256];
};

static void
radit_insert_internal(
    struct node **node,
    const unsigned char *key,
    void *value)
{
    if (!node)
    {
        struct leaf *l = (struct leaf *)malloc(sizeof(struct leaf));
        l->value = value;
        *node = (struct node *)l;
    }
}

void
radit_insert(
    struct radit_tree *tree,
    const unsigned char *key,
    void *value)
{
    radit_insert_internal(&tree->root, key, value);
}
