#ifndef RADIT_H
#define RADIT_H

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

struct radit_tree
{
};

int radit_init(struct radit_tree *tree);

int radit_destroy(struct radit_tree *tree);

void *radit_search(const struct radit_tree *tree, const unsigned char *key);

void radit_insert(struct radit_tree *tree, const unsigned char *key, void *value);

void radit_delete(struct radit_tree *tree, const unsigned char *key);

#endif
