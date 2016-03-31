typedef struct node {
        struct node *previous;
        int size;
        char in_use;
        struct node *next;
} Node;

void *my_bestfit_malloc(int size);
void my_free(void *ptr);
void initializeStruct(Node *a, Node *prev, int size, int in_use, Node *next);

Node *first;
Node *last;
