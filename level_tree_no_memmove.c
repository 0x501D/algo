#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_s;

typedef struct node_s {
    struct node_s *left;
    struct node_s *rigth;
    int ctx;
} node_t;

typedef struct {
    size_t n_memb;  /* unit count */
    size_t q_len;   /* queue len */
    size_t n_alloc; /* count of allocated memory in units */
    size_t shift;
    void **data;    /* ctx */
} queue_t;

enum {QUEUE_INIT_NMEMB = 10};

void queue_push(queue_t *q, const void *data)
{
    if (q == NULL) {
        fprintf(stderr, "null queue\n");
        exit(1);
    }

    if (q->data == NULL) { /* queue not initialized */
        q->data = malloc(sizeof(void *) * QUEUE_INIT_NMEMB);
        q->n_alloc = QUEUE_INIT_NMEMB;
        memset(q->data, 0, q->n_alloc * sizeof(void *));
    }

    if (q->n_memb == q->n_alloc) {
        q->data = realloc(q->data, sizeof(void *) * (q->n_alloc * 2));
        memset(q->data + q->n_alloc, 0, q->n_alloc * sizeof(void *));
        q->n_alloc *= 2;
    }

    q->data[q->n_memb] = (node_t *) data;

    q->n_memb++;
    q->q_len++;
}

void *queue_front(const queue_t *q)
{
    if (q == NULL) {
        fprintf(stderr, "null queue\n");
        exit(1);
    }

    return q->data[q->shift];
}

void queue_pop(queue_t *q)
{
    if (q == NULL) {
        fprintf(stderr, "null queue\n");
        exit(1);
    }

    q->q_len--;
    q->shift += 1;
}

void queue_free(queue_t *q)
{
    if (q == NULL) {
        fprintf(stderr, "null queue\n");
        exit(1);
    }

    free(q->data);

    q->data = NULL;
    q->n_memb = 0;
    q->n_alloc = 0;
}

void print_tree(const node_t *root)
{
    queue_t queue = { 0, 0, 0, 0, NULL };

    if (!root) {
        return;
    }

    queue_push(&queue, root);

    while (queue.q_len) {
        node_t *cur = queue_front(&queue);
        printf("%d ", cur->ctx);

        if (cur->left != NULL) {
            queue_push(&queue, cur->left);
        }

        if (cur->rigth != NULL) {
            queue_push(&queue, cur->rigth);
        }

        queue_pop(&queue);
    }

    queue_free(&queue);
}

int main()
{
#if 0
           8
         *  *
        2     7
      *  *   *   *
     1    3  5   9
    *           *  *
   44          12   42
#endif

    node_t root = { NULL, NULL, 8 };
    node_t l0 = { NULL, NULL, 2 };
    node_t l1 = { NULL, NULL, 7 };
    node_t l2 = { NULL, NULL, 1 };
    node_t l3 = { NULL, NULL, 3 };
    node_t l4 = { NULL, NULL, 5 };
    node_t l5 = { NULL, NULL, 9 };
    node_t l6 = { NULL, NULL, 44 };
    node_t l7 = { NULL, NULL, 12 };
    node_t l8 = { NULL, NULL, 42 };

    root.left = &l0;
    root.rigth = &l1;
    l0.left = &l2;
    l0.rigth = &l3;
    l1.left = &l4;
    l1.rigth = &l5;
    l2.left = &l6;
    l5.left = &l7;
    l5.rigth = &l8;

    print_tree(&root);
    putchar('\n');
    printf("%s\n", "8 2 7 1 3 5 9 44 12 42");

    return 0;
}
