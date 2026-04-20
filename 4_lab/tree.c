#include "tree.h"
#include <string.h>

BTreeNode *create_node(bool is_leaf)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    if (!node)
        return NULL;
    node->leaf = is_leaf;
    node->keyCount = 0;
    for (int i = 0; i < MAX_CHILDREN; i++)
        node->children[i] = NULL;
    return node;
}

void free_node(BTreeNode *node)
{
    if (!node)
        return;
    free(node);
}

void free_tree(BTreeNode *root)
{
    if (!root)
        return;

    if (!root->leaf)
    {
        for (int i = 0; i <= root->keyCount; i++)
            free_tree(root->children[i]);
    }
    free_node(root);
}

Item *btree_search(BTreeNode *root, const char *key)
{
    if (!root)
        return NULL;

    int i = 0;
    while (i < root->keyCount && strcmp(key, root->items[i].key) > 0)
        i++;

    if (i < root->keyCount && strcmp(key, root->items[i].key) == 0)
        return &root->items[i];

    if (root->leaf)
        return NULL;

    return btree_search(root->children[i], key);
}

static void btree_print_internal(BTreeNode *node, int level)
{
    if (!node)
        return;

    for (int i = node->keyCount - 1; i >= 0; i--)
    {
        if (!node->leaf)
            btree_print_internal(node->children[i + 1], level + 1);
        for (int j = 0; j < level; j++)
            printf("    ");
        printf("%s: %.2lf\n", node->items[i].key, node->items[i].value);
    }
    if (!node->leaf)
        btree_print_internal(node->children[0], level + 1);
}

void btree_print(BTreeNode *root)
{
    if (!root)
    {
        printf("Tree is empty\n");
        return;
    }
    btree_print_internal(root, 0);
}

static void split_child(BTreeNode *parent, int i, BTreeNode *y)
{
    BTreeNode *z = create_node(y->leaf);
    if (!z)
        return;
    z->keyCount = T - 1;

    for (int j = 0; j < T - 1; j++)
        z->items[j] = y->items[j + T];

    if (!y->leaf)
    {
        for (int j = 0; j < T; j++)
            z->children[j] = y->children[j + T];
    }

    y->keyCount = T - 1;

    for (int j = parent->keyCount; j >= i + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[i + 1] = z;

    for (int j = parent->keyCount - 1; j >= i; j--)
        parent->items[j + 1] = parent->items[j];

    parent->items[i] = y->items[T - 1];
    parent->keyCount++;
}

static void insert_non_full(BTreeNode *node, const char *key, double value)
{
    int i = node->keyCount - 1;

    if (node->leaf)
    {
        while (i >= 0 && strcmp(key, node->items[i].key) < 0)
        {
            node->items[i + 1] = node->items[i];
            i--;
        }
        strcpy(node->items[i + 1].key, key);
        node->items[i + 1].value = value;
        node->keyCount++;
    }
    else
    {
        while (i >= 0 && strcmp(key, node->items[i].key) < 0)
            i--;
        i++;

        if (node->children[i]->keyCount == MAX_KEYS)
        {
            split_child(node, i, node->children[i]);
            if (strcmp(key, node->items[i].key) > 0)
                i++;
        }
        insert_non_full(node->children[i], key, value);
    }
}

BTreeNode *btree_insert(BTreeNode *root, const char *key, double value)
{
    if (!root)
    {
        root = create_node(true);
        if (!root)
            return NULL;
        strcpy(root->items[0].key, key);
        root->items[0].value = value;
        root->keyCount = 1;
        return root;
    }

    Item *existing = btree_search(root, key);
    if (existing)
    {
        existing->value = value;
        return root;
    }

    if (root->keyCount == MAX_KEYS)
    {
        BTreeNode *s = create_node(false);
        if (!s)
            return root;
        s->children[0] = root;
        split_child(s, 0, root);

        int i = 0;
        if (strcmp(s->items[0].key, key) < 0)
            i++;
        insert_non_full(s->children[i], key, value);

        return s;
    }

    insert_non_full(root, key, value);
    return root;
}

static void merge(BTreeNode *node, int idx);
static void delete_internal(BTreeNode *node, const char *key);

static int find_key(BTreeNode *node, const char *key)
{
    int idx = 0;
    while (idx < node->keyCount && strcmp(node->items[idx].key, key) < 0)
        idx++;
    return idx;
}

static void remove_from_leaf(BTreeNode *node, int idx)
{
    for (int i = idx + 1; i < node->keyCount; ++i)
        node->items[i - 1] = node->items[i];
    node->keyCount--;
}

static void remove_from_non_leaf(BTreeNode *node, int idx)
{
    Item k = node->items[idx];

    if (node->children[idx]->keyCount >= T)
    {
        BTreeNode *cur = node->children[idx];
        while (!cur->leaf)
            cur = cur->children[cur->keyCount];
        Item pred = cur->items[cur->keyCount - 1];
        node->items[idx] = pred;
        delete_internal(node->children[idx], pred.key);
    }
    else if (node->children[idx + 1]->keyCount >= T)
    {
        BTreeNode *cur = node->children[idx + 1];
        while (!cur->leaf)
            cur = cur->children[0];
        Item succ = cur->items[0];
        node->items[idx] = succ;
        delete_internal(node->children[idx + 1], succ.key);
    }
    else
    {
        merge(node, idx);
        delete_internal(node->children[idx], k.key);
    }
}

static void borrow_from_prev(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx - 1];

    for (int i = child->keyCount - 1; i >= 0; --i)
        child->items[i + 1] = child->items[i];

    if (!child->leaf)
    {
        for (int i = child->keyCount; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->items[0] = node->items[idx - 1];

    if (!child->leaf)
        child->children[0] = sibling->children[sibling->keyCount];

    node->items[idx - 1] = sibling->items[sibling->keyCount - 1];

    child->keyCount += 1;
    sibling->keyCount -= 1;
}

static void borrow_from_next(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->items[child->keyCount] = node->items[idx];

    if (!child->leaf)
        child->children[child->keyCount + 1] = sibling->children[0];

    node->items[idx] = sibling->items[0];

    for (int i = 1; i < sibling->keyCount; ++i)
        sibling->items[i - 1] = sibling->items[i];

    if (!sibling->leaf)
    {
        for (int i = 1; i <= sibling->keyCount; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->keyCount += 1;
    sibling->keyCount -= 1;
}

static void merge(BTreeNode *node, int idx)
{
    BTreeNode *child = node->children[idx];
    BTreeNode *sibling = node->children[idx + 1];

    child->items[T - 1] = node->items[idx];

    for (int i = 0; i < sibling->keyCount; ++i)
        child->items[i + T] = sibling->items[i];

    if (!child->leaf)
    {
        for (int i = 0; i <= sibling->keyCount; ++i)
            child->children[i + T] = sibling->children[i];
    }

    for (int i = idx + 1; i < node->keyCount; ++i)
        node->items[i - 1] = node->items[i];

    for (int i = idx + 2; i <= node->keyCount; ++i)
        node->children[i - 1] = node->children[i];

    child->keyCount += sibling->keyCount + 1;
    node->keyCount--;
    free_node(sibling);
}

static void fill(BTreeNode *node, int idx)
{
    if (idx != 0 && node->children[idx - 1]->keyCount >= T)
        borrow_from_prev(node, idx);
    else if (idx != node->keyCount && node->children[idx + 1]->keyCount >= T)
        borrow_from_next(node, idx);
    else
    {
        if (idx != node->keyCount)
            merge(node, idx);
        else
            merge(node, idx - 1);
    }
}

static void delete_internal(BTreeNode *node, const char *key)
{
    int idx = find_key(node, key);

    if (idx < node->keyCount && strcmp(node->items[idx].key, key) == 0)
    {
        if (node->leaf)
            remove_from_leaf(node, idx);
        else
            remove_from_non_leaf(node, idx);
    }
    else
    {
        if (node->leaf)
            return;

        bool flag = (idx == node->keyCount);

        if (node->children[idx]->keyCount < T)
            fill(node, idx);

        if (flag && idx > node->keyCount)
            delete_internal(node->children[idx - 1], key);
        else
            delete_internal(node->children[idx], key);
    }
}

BTreeNode *btree_delete(BTreeNode *root, const char *key)
{
    if (!root)
        return NULL;

    delete_internal(root, key);

    if (root->keyCount == 0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->children[0];
        free_node(tmp);
    }

    return root;
}
