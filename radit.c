#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "radit.h"

#define INDEX_ADDRESS(node, index) ((uint64_t)(&node->data) + sizeof(unsigned char) * node->size + sizeof(struct node *) * index)

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
    memset(node->data, 0, sizeof(struct node) + num_children +
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
    memset(node, 0, sizeof(struct node) +
           sizeof(unsigned char) * compressed_length +
           sizeof(struct node *) + sizeof(void *));
    node->is_compressed = 1;
    node->size = compressed_length;
    memcpy(node->data, compressed_data, compressed_length);
    return node;
}

static uint64_t
get_value(struct node *node)
{
    uint64_t base_address = (uint64_t)node;
    return node->is_compressed ?
        *(uint64_t *)(base_address + sizeof(struct node) + node->size + sizeof(struct node *)) :
        *(uint64_t *)(base_address + sizeof(struct node) + node->size + sizeof(struct node *) * node->size);
}

static void
set_value(struct node *node, uint64_t data)
{
    node->has_value = 1;
    uint64_t base_address = (uint64_t)node;
    if (node->is_compressed)
    {
        *(uint64_t *)(base_address + sizeof(struct node) + node->size + sizeof(struct node *)) = data;
    }
    else
    {
        *(uint64_t *)(base_address + sizeof(struct node) + node->size + sizeof(struct node *) * node->size) = data;
    }
}

static void
set_link(struct node *parent, struct node *child, uint8_t index)
{
    /*
     * Truncate the child node 1 index (must move pointer value) and move it to
     * parent. Then point parent index to child.
     *
     * Child node:
     *  [is_compressed=1][abc][c-pointer][value-pointer]
     *  [is_compressed=1][bc][c-pointer][value-pointer]
     *
     * Parent node:
     *  [is_compressed=0][z][z-pointer][NULL]
     *  [is_compressed=0][z][a][z-pointer][a-pointer][NULL]
     */
    assert(!parent->is_compressed);
    assert(child->is_compressed);
    assert(index < parent->size);

    parent->data[index] = child->data[0];

    /* Shift child compressed data */
    memmove(child->data, child->data + 1, sizeof(unsigned char) * (child->size - 1));

    /* Shift child index by 1 char (compressed has only 1 index) */
    memmove((void *)(INDEX_ADDRESS(child, 0) - sizeof(unsigned char)), (void *)INDEX_ADDRESS(child, 0), sizeof(struct node *));

    /* Shift child value pointer */
    uint64_t old_value = get_value(child);
    child->size -= 1;
    set_value(child, old_value);

    /* Set parent to point to child */
    *((uint64_t *)INDEX_ADDRESS(parent, index)) = (uint64_t)child;
}

static uint8_t
prefix_matches_length(unsigned char *a, size_t alen, unsigned char * b, size_t blen)
{
    int i;
    int j = alen < blen ? alen : blen;
    for (i=0; i<j; i++)
    {
        if (a[i] != b[i])
        {
            break;
        }
    }
    return i;
}

static void
radit_insert_internal(struct node **node, unsigned char *key, size_t keylen, int64_t value)
{
    if ((*node)->is_compressed)
    {
        /*
         * realloc parent to include an additional child node.
         */
        struct node *new_parent = create_parent_node(2);
        struct node *new_child;

        int8_t length = prefix_matches_length((*node)->data, (*node)->size, key, keylen);

        if (length > 0)
        {
            new_child = create_compressed_node(key + length, keylen - length);
            //TODO old node needs to truncate off common prefix.
            // consider function truncate_data(struct node *n, uint8_t length);
        }
        else
        {
            new_child = create_compressed_node(key, keylen);
        }

        set_value(new_child, value);

        set_link(new_parent, *node, 0);
        set_link(new_parent, new_child, 1);

        *node = new_parent;
    }
    else
    {
        printf("internal insert uncompressed\n");
        // iterate over node's slots and either insert new child or recurse down the node
    }
}

static void *
radit_search_internal(struct node *node, const char * key)
{
    if (node->has_value && strncmp((char *)node->data, key, strlen(key)) == 0)
    {
        return (void *)get_value(node);
    }
    else if (!node->is_compressed)
    {
        int i;
        for (i=0; i<node->size; i++)
        {
            if (node->data[i] == key[0])
            {
                return radit_search_internal((struct node *)(*(uint64_t *)INDEX_ADDRESS(node, i)), key + 1);
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
    if (!tree->root)
    {
        return NULL;
    }
    return radit_search_internal(tree->root, key);
}

void
radit_insert(
    struct radit_tree *tree,
    const char *key,
    void *value)
{
    if (tree->root == NULL)
    {
        struct node *n;
        n = create_compressed_node((unsigned char *)key, strlen(key));
        set_value(n, (int64_t)value);

        tree->root = n;
    }
    else
    {
        radit_insert_internal(&tree->root, (unsigned char *)key, strlen(key), (int64_t)value);
    }
}
