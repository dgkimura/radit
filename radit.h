#ifndef RADIT_H
#define RADIT_H

struct radit_tree
{
    void *root;
};

int radit_init(struct radit_tree *tree);

int radit_destroy(struct radit_tree *tree);

void *radit_search(const struct radit_tree *tree, const unsigned char *key);

void radit_insert(struct radit_tree *tree, const unsigned char *key, void *value);

void radit_delete(struct radit_tree *tree, const unsigned char *key);

#endif
