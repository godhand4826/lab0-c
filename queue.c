#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->head = NULL;
    q->last = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL) {
        return;
    }

    list_ele_t *cur = q->head;
    list_ele_t *next = NULL;
    while (cur != NULL) {
        next = cur->next;
        free(cur->value);
        free(cur);
        cur = next;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }
    size_t len = strlen(s);
    char *s2 = malloc(len + 1);
    if (s2 == NULL) {
        return false;
    }
    strncpy(s2, s, len);
    s2[len] = '\0';

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        free(s2);
        return false;
    }
    newh->next = q->head;
    newh->value = s2;

    q->head = newh;
    q->size += 1;
    if (q->size == 1) {
        q->last = newh;
    }
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL) {
        return false;
    }
    if (q->size == 0) {
        return q_insert_head(q, s);
    }
    size_t len = strlen(s);
    char *s2 = malloc(len + 1);
    if (s2 == NULL) {
        return false;
    }
    strncpy(s2, s, len);
    s2[len] = '\0';

    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        free(s2);
        return false;
    }
    newh->value = s2;
    newh->next = NULL;
    q->last->next = newh;
    q->last = newh;
    q->size += 1;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->size == 0 || sp == NULL) {
        return false;
    }

    size_t len = strlen(q->head->value);
    size_t min = len < bufsize ? len : bufsize - 1;
    strncpy(sp, q->head->value, min);
    sp[min] = '\0';

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free(tmp->value);
    free(tmp);
    q->size -= 1;
    if (q->size == 0) {
        q->last = NULL;
    }
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size <= 1) {
        return;
    }
    list_ele_t *last = NULL;
    list_ele_t *tmp = NULL;
    list_ele_t *cur = q->head;
    while (cur != NULL) {
        tmp = cur->next;
        cur->next = last;
        last = cur;
        cur = tmp;
    }
    tmp = q->head;
    q->head = q->last;
    q->last = tmp;
}

list_ele_t *merge(list_ele_t *listA, list_ele_t *listB)
{
    list_ele_t *head = &(list_ele_t){.next = NULL, .value = ""};
    list_ele_t *cur = head;
    while (listA != NULL && listB != NULL) {
        if (strcmp(listA->value, listB->value) < 0) {
            cur->next = listA;
            cur = cur->next;
            listA = listA->next;
        } else {
            cur->next = listB;
            cur = cur->next;
            listB = listB->next;
        }
    }
    if (listA == NULL) {
        cur->next = listB;
    }
    if (listB == NULL) {
        cur->next = listA;
    }
    return head->next;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next) {
        return head;
    }
    list_ele_t *slow = head;
    list_ele_t *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;
    return merge(merge_sort(head), merge_sort(fast));
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (q == NULL || q->size <= 1) {
        return;
    }
    q->head = merge_sort(q->head);
    list_ele_t *cur = q->head->next;
    while (cur != NULL && cur->next != NULL) {
        cur = cur->next;
    }
    q->last = cur;
}