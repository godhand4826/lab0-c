#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct {
    struct list_head list;
    int size;
} queue_t;

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }

    INIT_LIST_HEAD(&q->list);
    q->size = 0;

    return &q->list;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (head == NULL) {
        return;
    }

    queue_t *q = list_entry(head, queue_t, list);

    element_t *node, *safe;
    list_for_each_entry_safe (node, safe, head, list) {
        list_del(&(node->list));
        q_release_element(node);
    }

    free(q);
}

/* Insert an element at head of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_head(struct list_head *head, char *s)
{
    if (head == NULL) {
        return false;
    }

    size_t len = strlen(s);
    char *value = malloc((len + 1) * sizeof(char));
    if (value == NULL) {
        return NULL;
    }
    strncpy(value, s, len);
    value[len] = '\0';

    element_t *node = malloc(sizeof(element_t));
    if (node == NULL) {
        free(value);
        return NULL;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = value;

    queue_t *q = list_entry(head, queue_t, list);
    list_add(&(node->list), &(q->list));
    q->size += 1;

    // cppcheck-suppress memleak
    return true;
}

/* Insert an element at tail of queue */
// cppcheck-suppress constParameterPointer
bool q_insert_tail(struct list_head *head, char *s)
{
    if (head == NULL) {
        return false;
    }

    size_t len = strlen(s);
    char *value = malloc((len + 1) * sizeof(char));
    if (value == NULL) {
        return NULL;
    }
    strncpy(value, s, len);
    value[len] = '\0';

    element_t *node = malloc(sizeof(element_t));
    if (node == NULL) {
        free(value);
        return NULL;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = value;

    queue_t *q = list_entry(head, queue_t, list);
    list_add_tail(&(node->list), &(q->list));
    q->size += 1;

    // cppcheck-suppress memleak
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL) {
        return NULL;
    }

    queue_t *q = list_entry(head, queue_t, list);

    if (list_empty(head)) {
        return NULL;
    }

    element_t *node = list_first_entry(head, element_t, list);
    list_del_init(&(node->list));
    q->size -= 1;

    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return node;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL) {
        return NULL;
    }
    queue_t *q = list_entry(head, queue_t, list);

    if (q->size == 0) {
        return NULL;
    }

    element_t *node = list_last_entry(head, element_t, list);
    list_del_init(&(node->list));
    q->size -= 1;

    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    return node;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (head == NULL) {
        return 0;
    }

    queue_t const *q = list_entry(head, queue_t, list);

    return q->size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
