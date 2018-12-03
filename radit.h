#ifndef RADIT_H
#define RADIT_H

struct node
{
    uint8_t has_value:1;

    uint8_t is_compressed:1;

    uint8_t size:6;

    unsigned char data[];
};

struct radit_tree
{
    struct node *root;
};

void radit_init(struct radit_tree *tree);

int radit_destroy(struct radit_tree *tree);

void *radit_search(const struct radit_tree *tree, const char *key);

void radit_insert(struct radit_tree *tree, const char *key, void *value);

void radit_delete(struct radit_tree *tree, const char *key);

#endif
