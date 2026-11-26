#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
    double data;
    struct Node *next;
} Node;

typedef struct {
    Node *top;
} Stack;

void init_stack(Stack *s) {
    s->top = NULL;
}

int is_empty(const Stack *s) {
    return s->top == NULL;
}

int is_full(const Stack *s) {
    (void)s;
    return 0;
}

int push(Stack *s, double value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        printf("Pomylka: ne vdalosia vydilyty pamiat.\n");
        return 0;
    }
    new_node->data = value;
    new_node->next = s->top;
    s->top = new_node;
    return 1;
}

int pop(Stack *s, double *out) {
    if (is_empty(s)) {
        printf("Pomylka: stek porozhnii (nemaye operanda).\n");
        return 0;
    }
    Node *temp = s->top;
    *out = temp->data;
    s->top = temp->next;
    free(temp);
    return 1;
}

int peek(const Stack *s, double *out) {
    if (is_empty(s)) {
        return 0;
    }
    *out = s->top->data;
    return 1;
}

void clear_stack(Stack *s) {
    double dummy;
    while (!is_empty(s)) {
        pop(s, &dummy);
    }
}

int evaluate_postfix(const char *expr, double *result) {
    Stack s;
    init_stack(&s);

    const char *p = expr;
    while (*p != '\0') {
        if (isspace((unsigned char)*p)) {
            p++;
            continue;
        }

        if (isdigit((unsigned char)*p) || ((*p == '-' || *p == '+') && isdigit((unsigned char)*(p + 1)))) {
            char *endptr;
            double val = strtod(p, &endptr);
            if (!push(&s, val)) {
                clear_stack(&s);
                return 0;
            }
            p = endptr;
        } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            double b, a;
            if (!pop(&s, &b) || !pop(&s, &a)) {
                clear_stack(&s);
                return 0;
            }
            double res = 0.0;
            if (*p == '+') res = a + b;
            else if (*p == '-') res = a - b;
            else if (*p == '*') res = a * b;
            else if (*p == '/') {
                if (b == 0.0) {
                    printf("Pomylka: dilennia na nul.\n");
                    clear_stack(&s);
                    return 0;
                }
                res = a / b;
            }
            if (!push(&s, res)) {
                clear_stack(&s);
                return 0;
            }
            p++;
        } else {
            printf("Pomylka: nevidomyi symvol '%c'.\n", *p);
            clear_stack(&s);
            return 0;
        }
    }

    if (!pop(&s, result)) {
        return 0;
    }
    if (!is_empty(&s)) {
        printf("Pomylka: zayvi operandy u vyrazi.\n");
        clear_stack(&s);
        return 0;
    }
    return 1;
}

int main() {
    char expr[256];
    printf("Vvedit postfiksnyi vyraz: ");
    if (!fgets(expr, sizeof(expr), stdin)) {
        return 1;
    }

    double result;
    if (evaluate_postfix(expr, &result)) {
        printf("Rezultat: %.2f\n", result);
    }

    return 0;
}