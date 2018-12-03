#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "radit.h"

static struct node *
create_parent_node(uint8_t num_children)
{
    /*
     * A parent node has format:
     *
     * [is_compressed=0][abc][a-pointer][b-pointer][c-pointer][value-pointer]
     */
    struct node *node;
    node = malloc(sizeof(struct node) + num_children +
                  sizeof(struct node *) * num_children + sizeof(void *));
    node->is_compressed = 0;
    node->size = num_children;
    return node;
}

static struct node *
create_compressed_node(unsigned char *compressed_data, uint8_t compressed_length)
{
    /*
     * A compressed node has format:
     *
     * [is_compressed=1][abc][c-pointer][value-pointer]
     */
    struct node *node;
    node = malloc(sizeof(struct node) +
                  sizeof(unsigned char) * compressed_length +
                  sizeof(struct node *) + sizeof(void *));
    node->is_compressed = 1;
    node->size = compressed_length;
    memcpy(node->data, compressed_data, compressed_length);
    return node;
}

static void *
get_value(struct node *node)
{
    unsigned char *base_ptr = (unsigned char *)node;
    return node->is_compressed ?
        base_ptr + sizeof(struct node) + node->size + sizeof(struct node *) * node->size :
        base_ptr + sizeof(struct node) + node->size + sizeof(struct node *);
}

static void
set_value(struct node *node, void *data)
{
    node->has_value = 1;
    memcpy(get_value(node), data, sizeof(void *));
}

void *
radit_search(
    const struct radit_tree *tree,
    const char *key)
{
    if (tree->root->has_value)
    {
        return get_value(tree->root);
    }
    return NULL;
}

void
radit_insert(
    struct radit_tree *tree,
    const char *key,
    void *value)
{
    if (!tree->root)
    {
        struct node *n;
        n = create_compressed_node((unsigned char *)key, strlen(key));
        set_value(n, value);

        tree->root = n;
    }
}
