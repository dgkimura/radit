#ifndef RADIT_H
#define RADIT_H

struct node
{
    /*
     * Used to indicate whether the full key path of this node maps to a value.
     */
    uint8_t has_value:1;

    /*
     * A compressed node uses `data[]` to store multiple bytes which represent a
     * subsequence of bytes of key(s).
     */
    uint8_t is_compressed:1;

    /*
     * When the node is compresed then `size` indicates the total number of bytes
     * a compressed node stores in the `data[]`.
     *
     * When the node is not compressed then `size` indicates the number of
     * children nodes linked from this parent node.
     *
     * NB: 6 bit field restricts a compressed node to store at most 64
     * contiguous bytes and a parent node to reference at most 64 children.
     */
    uint8_t size:6;

    /*
     * When the node is compressed then `data[]` stores multiple bytes of the
     * compressed key, the pointer to the next child node, and the value
     * pointer (or NULL if no corresponding value for key).
     *
     * For example:
     *   [abc][c-pointer][value-pointer]
     *
     * When the node is not compressed then `data[]` stores each of the child
     * nodes.
     *
     * For example:
     *   [abc][a-pointer][b-pointer][c-pointer][value-pointer]
     */
    unsigned char data[];
};

struct radit_tree
{
    struct node *root;
};

void radit_init(struct radit_tree *tree);

void radit_destroy(struct radit_tree *tree);

void *radit_search(const struct radit_tree *tree, const char *key);

void radit_insert(struct radit_tree *tree, const char *key, void *value);

void radit_delete(struct radit_tree *tree, const char *key);

#endif
