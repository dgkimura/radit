#include <stdint.h>
#include <stdlib.h>

#include "radit.h"

#define NODE_LEAF 0
#define NODE_4    1
#define NODE_16   2
#define NODE_48   3
#define NODE_256  4

struct leaf
{
    uint8_t type;

    void *value;
};

struct node_4
{
    uint8_t type;

    /* keys are sorted */
    char keys[4];

    uint8_t num_children;

    /* stores between 1 - 4 children*/
    void *children[4];
};

struct node_16
{
    uint8_t type;

    /* keys are sorted */
    char keys[16];

    uint8_t num_children;

    /* stores between 5 - 16 children*/
    void *children[16];
};

struct node_48
{
    uint8_t type;

    /* keys are sorted */
    char keys[256];

    uint8_t num_children;

    /* stores between 17 - 48 children*/
    void *children[48];
};

struct node_256
{
    uint8_t type;

    uint8_t num_children;

    /* stores between 49 - 256 children*/
    void *children[256];
};

static void
radit_insert_internal(
    void **node,
    const char *key,
    void *value)
{
    if (!*node)
    {
        struct leaf *l = (struct leaf *)malloc(sizeof(struct leaf));
        l->type = NODE_LEAF;
        l->value = value;
        *node = (void *)l;
        return;
    }

    if (NODE_LEAF == ((struct leaf *)(*node))->type)
    {
        struct leaf *ol = (struct leaf *)*node;

        struct node_4 *n = (struct node_4 *)malloc(sizeof(struct node_4));
        n->type = NODE_4;
        n->num_children = 0;
        *node = n;

        struct leaf *nl = (struct leaf *)malloc(sizeof(struct leaf));
        nl->type = NODE_LEAF;
        nl->value = value;


        n->children[0] = ol;
        n->children[1] = nl;
        n->num_children = 2;
        return;
    }
}

void *
radit_search(
    const struct radit_tree *tree,
    const char *key)
{
    return ((struct leaf *)tree->root)->value;
}

void
radit_insert(
    struct radit_tree *tree,
    const char *key,
    void *value)
{
    radit_insert_internal(&tree->root, key, value);
}
