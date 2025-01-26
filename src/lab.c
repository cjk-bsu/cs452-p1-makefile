#include "lab.h"
#include <stdio.h>

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {

    // allocate memory for new list
    list_t *list = (list_t *)malloc(sizeof(list_t));

    // return null if unsuccessful
    if (!list) {
        return NULL;
    }

    // assign starter values to each list member
    list->destroy_data = destroy_data;
    list->compare_to = compare_to;
    list->size = 0;

    // allocate memory for list head node
    list->head = (node_t *)malloc(sizeof(node_t));

    // if unsuccessful, destroy the list and return null
    if (!list->head) {
        free(list);
        return NULL;
    }

    // assign starter values for each node member
    list->head->data = NULL;
    list->head->next = list->head;
    list->head->prev = list->head;

    // return the empty, initialized list
    return list;
}

void list_destroy(list_t **list) {

    // if there isn't a list or a pointer to a list, return
    if (!list || !*list) {
        return;
    }

    // set the current node to the list head's next node
    node_t *current = (*list)->head->next;

    // while the current node is not the list's head's node, consume and destroy each subsequent node
    while (current != (*list)->head) {

        node_t *next = current->next;

        if ((*list)->destroy_data) {
            (*list)->destroy_data(current->data);
        }

        free(current);
        current = next;
    }

    // finally, destroy the head node, the list itself, and set it to null
    free((*list)->head);
    free(*list);
    *list = NULL;
}

list_t *list_add(list_t *list, void *data) {

    // if there isn't a list, return
    if (!list) {
        return NULL;
    }

    // allocate memory for a new list node
    node_t *new_node = (node_t *)malloc(sizeof(node_t));

    // if unsuccessful, return null
    if (!new_node) {
        return NULL;
    }

    // populate the new node with data
    new_node->data = data;

    // insert the new node at the front
    new_node->prev = list->head;
    new_node->next = list->head->next;
    list->head->next->prev = new_node;
    list->head->next = new_node;
    list->size++;

    // return the list
    return list;
}

void *list_remove_index(list_t *list, size_t index) {

    // if no list or if the index is bigger than the size of the list, return null
    if (!list || index >= list->size) {
        return NULL;
    }

    // set the current node to the list head's next node 
    node_t *current = list->head->next;

    // iterate through the list to get to the index to remove
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    // collect the data from the node being removed
    void *data = current->data;

    // close the gap in the list
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // destroy the node and decrement the list size
    free(current);
    list->size--;

    // return the value from the removed node
    return data;
}

int list_indexof(list_t *list, void *data) {

    // if no list or empty list, return -1
    if (!list || list->size == 0) {
        return -1;
    }

    // set the current node to the list's head's next node
    node_t *current = list->head->next;

    // initialize an index and interate until you find the first index with that value and return it
    int index = 0;
    while (current != list->head) {
        if (list->compare_to(current->data, data) == 0) {
            return index;
        }
        current = current->next;
        index++;
    }

    // otherwise, if not found or does not exist, return -1
    return -1;
}
