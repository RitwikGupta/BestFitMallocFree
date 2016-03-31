#include <stdio.h>
#include <unistd.h>
#include "mymalloc.h"

void *my_bestfit_malloc(int size) {
        void *ret;
        char *alignment;
        int alloc_size;
        int best_size;
        Node *curr = NULL;
        Node *best = NULL;
        Node *temp = NULL;

        // If allocation size required <= 0, then there's nothing to be allocated.
        if(size == 0 || size < 0) {
                return NULL;
        }

        alloc_size = size + sizeof(Node); // total allocation size is the size of the Node + the request

        if(first == NULL) {
                last = first = (Node *)sbrk(alloc_size);

                initializeStruct(first, NULL, alloc_size, 1, NULL);

                alignment = (char *)first;
                ret = alignment + sizeof(Node);
        } else {
                curr = first;
                best_size = 0;

                // Start best fit algorithm
                while(curr != NULL) {
                        if(curr->in_use == 0) {
                                if(curr->size >= alloc_size && (curr->size < best_size || best_size == 0)) {
                                        best_size = curr->size;
                                        best = curr;
                                        if(curr->size == alloc_size) {
                                                break;
                                        }
                                }
                        }
                        curr = curr->next;
                }
                // End best fit algorithm

                // No space fitting the allocation request was found
                if(best == NULL) {
                        best = (Node *)sbrk(alloc_size);
                        initializeStruct(best, NULL, alloc_size, 1, NULL);

                        alignment = (char *)best;
                        ret = alignment + sizeof(Node);
                        temp = last;
                        last->next = best;
                        last = best;
                        last->previous = temp;
                } else {
                        if(best_size > alloc_size) {
                                curr = (Node *)((char*)best + alloc_size);

                                initializeStruct(curr, best, (best->size - alloc_size), 0, best->next);

                                temp = curr->next;
                                temp->previous = curr;

                                initializeStruct(best, best->previous, alloc_size, 1, curr);

                                alignment = (char *)best;
                                ret = alignment + sizeof(Node);
                        } else {
                                best->in_use = 1;

                                alignment = (char *)best;
                                ret = alignment + sizeof(Node);
                        }
                }
        }
        return ret;
}

void my_free(void *ptr) {
        Node *pointer;
        Node *prev, *next;
        int temp_size_holder;

        if(ptr == NULL) {
                return;
        }

        pointer = (Node *)((char *)ptr - sizeof(Node)); //offset the node
        prev = pointer->previous;
        next = pointer->next;

        if(next == NULL) {
                last = pointer;
        }

        if(next == NULL) {
                if(prev == NULL) {
                        first = last = NULL;
                        sbrk(0 - pointer->size);
                        return;
                } else {
                        if(prev->in_use == 1) {
                                last = prev;
                                last->next = NULL;
                                sbrk(0 - pointer->size);
                        } else {
                          last = prev->previous;
                          last->next = NULL;
                          sbrk(0 - (pointer->size + prev->size));
                        }
                }
        }
        // Coalesce nodes
        if (!(prev != NULL && next == NULL)) {
                pointer->in_use = 0;
                if(prev != NULL && prev->in_use == 0) {
                        pointer->previous->next = pointer->next;
                        pointer->next->previous = pointer->previous;
                        pointer->previous->size += pointer->size;
                        pointer = pointer->previous;
                        next = pointer->next;
                        prev = pointer->previous;
                }
                if(next != NULL && next->in_use == 0) {
                        temp_size_holder = pointer->next->size;
                        pointer->next = pointer->next->next;
                        pointer->next->previous = pointer;
                        pointer->size += temp_size_holder;
                }
        }
}

void initializeStruct(Node *a, Node *prev, int size, int in_use, Node *next) {
        a->previous = prev;
        a->size = size;
        a->in_use = in_use;
        a->next = next;
}
