#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "radit.h"

#define INDEX_ADDRESS(n, index) ((uint64_t)(&n->data) + sizeof(unsigned char) * n->size + sizeof(struct node *) * index)

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
truncate_compressed_node(struct node *node, size_t length)
{
    /*
     * Truncate the node by shifting compressed data, index pointer, and value
     * pointer.
     *
     *  [is_compressed=1][abc][c-pointer][value-pointer]
     *  [is_compressed=1][bc][c-pointer][value-pointer]
     */
    assert(node->is_compressed);

    /* Shift compressed data */
    memmove(node->data, node->data + length, sizeof(unsigned char) * (node->size - length));

    /* Shift index (compressed has only 1 index) */
    memmove((void *)(INDEX_ADDRESS(node, 0) - sizeof(unsigned char) * length), (void *)INDEX_ADDRESS(node, 0), sizeof(struct node *));

    /* Shift value pointer */
    uint64_t old_value = get_value(node);
    node->size -= length;
    set_value(node, old_value);
}

static void
set_node_index(struct node *parent, struct node *child, uint8_t index)
{
    assert(!parent->is_compressed || index == 0);
    assert(index < parent->size);

    parent->data[index] = child->data[0];

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
        struct node *new_parent;
        struct node *new_child;
        struct node *new_grandchild;

        int8_t length = prefix_matches_length((*node)->data, (*node)->size, key, keylen);

        if (length == keylen)
        {
            /*
             * Key being added is a substring.
             */
            new_parent = create_compressed_node(key, length);
            set_value(new_parent, value);

            new_child = create_parent_node(1);
            *((uint64_t *)INDEX_ADDRESS(new_parent, 0)) = (uint64_t)new_child;

            truncate_compressed_node(*node, length);
            set_node_index(new_child, *node, 0);
            truncate_compressed_node(*node, 1);
        }
        else if (length == (*node)->size)
        {
            /*
             * Current compressed node is substring of key being added.
             */
            new_parent = *node;

            new_child = create_compressed_node(key + length, keylen - length);
            set_value(new_child, value);

            *((uint64_t *)INDEX_ADDRESS(new_parent, 0)) = (uint64_t)new_child;
        }
        else if (length > 0)
        {
            /*
             * Neither current node nor key is a substring of the other, but
             * there is a common prefix.
             */
            new_parent = create_compressed_node(key, length);
            new_child = create_parent_node(2);

            *((uint64_t *)INDEX_ADDRESS(new_parent, 0)) = (uint64_t)new_child;

            truncate_compressed_node(*node, length);
            set_node_index(new_child, *node, 0);
            truncate_compressed_node(*node, 1);

            new_grandchild = create_compressed_node(key + length, keylen - length);
            set_value(new_grandchild, value);
            set_node_index(new_child, new_grandchild, 1);
            truncate_compressed_node(new_grandchild, 1);
        }
        else
        {
            /*
             * There is no common prefix so we need to create a parent branch.
             */
            new_parent = create_parent_node(2);
            new_child = create_compressed_node(key + length, keylen - length);

            set_value(new_child, value);

            set_node_index(new_parent, *node, 0);
            truncate_compressed_node(*node, 1);

            set_node_index(new_parent, new_child, 1);
            truncate_compressed_node(new_child, 1);
        }

        *node = new_parent;
    }
    else
    {
        /*
         * Iterate over node's slots and either insert new child or recurse
         * down the node
         */

        struct node *old_parent = *node;
        struct node *new_parent;
        struct node *child;

        int i;
        for (i=0; i<old_parent->size; i++)
        {
            if (old_parent->data[i] == key[0])
            {
                struct node *n = (struct node *)INDEX_ADDRESS(old_parent, i);
                radit_insert_internal(&n, key + 1, keylen - 1, value);
                return;
            }
        }

        new_parent = create_parent_node(old_parent->size + 1);

        /* copy parent indexes */
        memmove(new_parent->data,
                old_parent->data,
                sizeof(unsigned char) * old_parent->size);

        /* copy pointers */
        memmove(new_parent->data + sizeof(unsigned char) * new_parent->size,
                old_parent->data + sizeof(unsigned char) * old_parent->size,
                sizeof(struct node *) * old_parent->size);

        new_parent->data[old_parent->size] = key[0];
        child = create_compressed_node(key + 1, keylen - 1);
        set_value(child, value);

        *((uint64_t *)INDEX_ADDRESS(new_parent, old_parent->size)) = (uint64_t)child;

        *node = new_parent;
        free(old_parent);
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
    else if (node->is_compressed && strncmp((char *)node->data, key, node->size) == 0)
    {
        return radit_search_internal((struct node *)(*(uint64_t *)INDEX_ADDRESS(node, 0)), key + node->size);
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

void
radit_delete(
    struct radit_tree *tree,
    const char *key)
{
}
