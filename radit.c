#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

    const char *key;
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
        l->key = key;
        *node = (void *)l;
        return;
    }

    if (NODE_LEAF == ((struct leaf *)(*node))->type)
    {
        struct leaf *ol = (struct leaf *)*node;

        struct node_4 *n = (struct node_4 *)malloc(sizeof(struct node_4));
        n->type = NODE_4;
        n->num_children = 0;
        memset(n->children, 0, sizeof(n->children));
        *node = n;

        n->children[0] = ol;
        n->keys[0] = ol->key[0];
        radit_insert_internal(&n->children[1], key, value);
        n->keys[1] = ((struct leaf *)(n->children[1]))->key[0];
        n->num_children = 2;
        return;
    }

    if (NODE_4 == ((struct leaf *)(*node))->type)
    {
        struct node_4 *n4 = (struct node_4 *)*node;

        if (n4->num_children < 4)
        {
            radit_insert_internal(&n4->children[n4->num_children], key, value);
            n4->keys[n4->num_children] = ((struct leaf *)(n4->children[n4->num_children]))->key[0];
            n4->num_children += 1;
            return;
        }

        struct node_16 *n = (struct node_16 *)malloc(sizeof(struct node_16));
        n->type = NODE_16;
        memset(n->children, 0, sizeof(n->children));

        int i;
        for (i=0; i<n4->num_children; i++)
        {
            n->children[i] = n4->children[i];
            n->keys[i] = n4->keys[i];
        }
        free(n4);

        radit_insert_internal(&n->children[i], key, value);
        n->keys[i] = ((struct leaf *)(n->children[i]))->key[0];
        n->num_children = n4->num_children + 1;
        *node = n;
        return;
    }

    if (NODE_16 == ((struct node_16 *)(*node))->type)
    {
        struct node_16 *n16 = (struct node_16 *)*node;

        if (n16->num_children < 16)
        {
            radit_insert_internal(&n16->children[n16->num_children], key, value);
            n16->keys[n16->num_children] = ((struct leaf *)(n16->children[n16->num_children]))->key[0];
            n16->num_children += 1;
            return;
        }
    }
}

static void *
radit_search_internal(
    const struct leaf *node,
    const char *key)
{
    uint8_t i;
    struct node_4 *n4;
    struct node_16 *n16;

    if (NODE_LEAF == node->type)
    {
        return node->value;
    }

    if (NODE_4 == node->type)
    {
        n4 = (struct node_4 *)node;

        for (i = 0; i < n4->num_children; i++)
        {
            if (n4->keys[i] == key[0])
            {
                return radit_search_internal(n4->children[i], key + 1);
            }
        }
    }

    if (NODE_16 == node->type)
    {
        n16 = (struct node_16 *)node;

        for (i = 0; i < n16->num_children; i++)
        {
            if (n16->keys[i] == key[0])
            {
                return radit_search_internal(n16->children[i], key + 1);
            }
        }
    }

    return NULL;
}

void *
radit_search(
    const struct radit_tree *tree,
    const char *key)
{
    return radit_search_internal(tree->root, key);
}

void
radit_insert(
    struct radit_tree *tree,
    const char *key,
    void *value)
{
    radit_insert_internal(&tree->root, key, value);
}
