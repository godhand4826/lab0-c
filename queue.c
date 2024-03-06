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
        list_del(&node->list);
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

    char *value = strdup(s);
    if (value == NULL) {
        return NULL;
    }

    element_t *node = malloc(sizeof(element_t));
    if (node == NULL) {
        free(value);
        return NULL;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = value;

    queue_t *q = list_entry(head, queue_t, list);
    list_add(&node->list, &q->list);
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

    char *value = strdup(s);
    if (value == NULL) {
        return NULL;
    }

    element_t *node = malloc(sizeof(element_t));
    if (node == NULL) {
        free(value);
        return NULL;
    }
    INIT_LIST_HEAD(&node->list);
    node->value = value;

    queue_t *q = list_entry(head, queue_t, list);
    list_add_tail(&node->list, &q->list);
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

    if (q->size == 0) {
        return NULL;
    }

    element_t *node = list_first_entry(head, element_t, list);
    list_del_init(&node->list);
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
    list_del_init(&node->list);
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
    if (head == NULL) {
        return false;
    }

    queue_t *q = list_entry(head, queue_t, list);
    if (q->size == 0) {
        return false;
    }

    int mid = q->size / 2;
    struct list_head *cur = q->list.next;
    for (int i = 0; i < mid; i++) {
        cur = cur->next;
    }

    element_t *node = list_entry(cur, element_t, list);
    list_del_init(&node->list);
    q_release_element(node);
    q->size -= 1;

    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (head == NULL) {
        return false;
    }

    queue_t *q = list_entry(head, queue_t, list);

    const struct list_head *end = &q->list;
    struct list_head *cur = &q->list;

    while (cur->next != end && cur->next->next != end) {
        struct list_head *check = cur->next;
        element_t *c = list_entry(check, element_t, list);
        element_t *cc = list_entry(check->next, element_t, list);
        if (strcmp(c->value, cc->value) == 0) {
            while (check->next != end && strcmp(c->value, cc->value) == 0) {
                list_del_init(&cc->list);
                q_release_element(cc);
                q->size -= 1;

                cc = list_entry(c->list.next, element_t, list);
            }
            list_del_init(check);
            q_release_element(list_entry(check, element_t, list));
            q->size -= 1;
        } else {
            cur = cur->next;
        }
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (head == NULL) {
        return;
    }

    queue_t *q = list_entry(head, queue_t, list);

    const struct list_head *end = &q->list;
    struct list_head *cur = &q->list;
    while (cur->next != end && cur->next->next != end) {
        element_t *a = list_entry(cur->next, element_t, list);
        element_t *b = list_entry(cur->next->next, element_t, list);

        char *tmp = a->value;
        a->value = b->value;
        b->value = tmp;

        cur = cur->next->next;
    }
}

void list_reverse(struct list_head *head)
{
    if (head == NULL) {
        return;
    }

    struct list_head *cur = head->next;
    while (cur != head) {
        struct list_head *tmp = cur->next;
        cur->next = cur->prev;
        cur->prev = tmp;

        cur = cur->prev;
    }

    struct list_head *tmp = head->next;
    head->next = head->prev;
    head->prev = tmp;
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (head == NULL) {
        return;
    }

    queue_t *q = list_entry(head, queue_t, list);

    list_reverse(&q->list);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (head == NULL) {
        return;
    }

    queue_t *q = list_entry(head, queue_t, list);

    struct list_head result;
    INIT_LIST_HEAD(&result);

    for (int j = q->size; j >= k; j -= k) {
        const struct list_head *end = &q->list;
        struct list_head *cur = q->list.next;

        int i = 0;
        for (i = 0; i < k - 1; i++) {
            if (cur != end) {
                cur = cur->next;
            } else {
                break;
            }
        }

        struct list_head tmp;
        INIT_LIST_HEAD(&tmp);
        list_cut_position(&tmp, &q->list, cur);


        if (cur != end) {
            list_reverse(&tmp);
        }

        list_splice_tail_init(&tmp, &result);
    }

    list_splice_init(&result, &q->list);
}

/**
 * list_merge() - Merge two sorted lists
 * @head_a: the first list
 * @head_b: the second list
 *
 * Merge two sorted lists into one sorted list. The result is stored in head_a.
 */
void list_merge(struct list_head *head_a,
                struct list_head *head_b,
                bool descend)
{
    struct list_head head;
    INIT_LIST_HEAD(&head);

    while (!list_empty(head_a) && !list_empty(head_b)) {
        element_t *a = list_first_entry(head_a, element_t, list);
        element_t *b = list_first_entry(head_b, element_t, list);

        if ((strcmp(a->value, b->value) < 0) == !descend) {
            list_del_init(&a->list);
            list_add_tail(&a->list, &head);
        } else {
            list_del_init(&b->list);
            list_add_tail(&b->list, &head);
        }
    }

    list_splice_tail_init(head_a, &head);
    list_splice_tail_init(head_b, &head);
    list_splice_tail_init(&head, head_a);
}

void list_mergesort(struct list_head *head, int size, bool descend)
{
    if (list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head *node = head;
    for (int i = 0; i < size / 2; i++) {
        node = node->next;
    }

    struct list_head left;
    INIT_LIST_HEAD(&left);
    list_cut_position(&left, head, node);

    list_mergesort(&left, size / 2, descend);
    list_mergesort(head, size - size / 2, descend);
    list_merge(head, &left, descend);
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (head == NULL) {
        return;
    }

    queue_t *q = list_entry(head, queue_t, list);
    if (q->size <= 1) {
        return;
    }

    list_mergesort(&q->list, q->size, descend);
}


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL) {
        return 0;
    }

    queue_t *q = list_entry(head, queue_t, list);

    if (q->size <= 1) {
        return q->size;
    }

    const struct list_head *end = &q->list;
    struct list_head *cur = q->list.next->next;
    while (cur != end) {
        if (cur->prev == head) {
            cur = cur->next;
            continue;
        }

        const element_t *a = list_entry(cur, element_t, list);
        element_t *b = list_entry(cur->prev, element_t, list);

        if (strcmp(a->value, b->value) < 0) {
            list_del_init(&b->list);
            q_release_element(b);
            q->size -= 1;
        } else {
            cur = cur->next;
        }
    }

    return q->size;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (head == NULL) {
        return 0;
    }

    queue_t *q = list_entry(head, queue_t, list);

    if (q->size <= 1) {
        return q->size;
    }

    const struct list_head *end = &q->list;
    struct list_head *cur = q->list.next->next;
    while (cur != end) {
        if (cur->prev == head) {
            cur = cur->next;
            continue;
        }

        const element_t *a = list_entry(cur, element_t, list);
        element_t *b = list_entry(cur->prev, element_t, list);

        if (strcmp(a->value, b->value) > 0) {
            list_del_init(&b->list);
            q_release_element(b);
            q->size -= 1;
        } else {
            cur = cur->next;
        }
    }

    return q->size;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (head == NULL) {
        return 0;
    }

    queue_contex_t *ctx = list_entry(head, queue_contex_t, chain);
    if (ctx->size == 0) {
        return 0;
    }

    const queue_contex_t *first =
        list_first_entry(&ctx->chain, queue_contex_t, chain);

    queue_contex_t *cur = NULL;
    list_for_each_entry (cur, &ctx->chain, chain) {
        if (cur == first) {
            continue;
        }

        queue_t *q1 = list_entry(first->q, queue_t, list);
        queue_t *q2 = list_entry(cur->q, queue_t, list);
        list_merge(&q1->list, &q2->list, descend);
        q1->size += q2->size;
        q2->size = 0;
    }

    return list_entry(first->q, queue_t, list)->size;
}