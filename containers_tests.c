/*
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>

#define __QUEUE_CONTAINER_TYPE int
#define __QUEUE_INITIAL_SIZE 10
#include <queue.h>

#define __STACK_CONTAINER_TYPE int
#define __STACK_INITIAL_SIZE 10
#include <stack.h>

void debug_queue_buffer(QueueContainer* buffer)
{
    printf("\n");
    printf("used %i\n", buffer->used);
    printf("max %i\n", buffer->max);
    printf("tail_position %i\n", buffer->tail_position);
    printf("head_position %i\n", buffer->head_position);

}

int
main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    if (strcmp(argv[1], "queue") == 0)
        return run_queue_test();

    if (strcmp(argv[1], "stack") == 0)
        return run_stack_test();

    return 1;
}

int
run_stack_test()
{
    int i, j;
    StackContainer stack;

    stack = stack_new();
    i = 0;

    for (i=0; i < 10; i++) {
        stack_push(&stack, i);
        printf("stack %i\n", i);
    }

    while (stack_pop(&stack, &j) != 0) {
        --i;
        if (i != j)
            abort();
    }
    stack_free(&stack);

    printf("Stack test OK\n");
    return 0;
}

int
run_queue_test()
{
    int i, j, r;
    QueueContainer queue;

    /*
     * Unaligned test
     */
    queue = queue_new(10);
    for (i=0; i<5; i++)
        queue_enqueue(&queue, i);

    queue_dequeue(&queue, &j);

    for (i=5; i<11; i++)
        queue_enqueue(&queue, i);

    queue_dequeue(&queue, &j);

    for (i=11; i<30; i++)
        queue_enqueue(&queue, i);

    queue_dequeue(&queue, &j);
    queue_dequeue(&queue, &j);
    queue_dequeue(&queue, &j);
    queue_dequeue(&queue, &j);
    queue_dequeue(&queue, &j);

    for (i=30; i<100; i++)
        queue_enqueue(&queue, i);

    queue_dequeue(&queue, &i);
    while (queue_dequeue(&queue, &j) != 0) {
        ++i;
        if (i != j)
            abort();
    }

    queue_free(&queue);
    printf("Unaligned test OK\n");


    /*
     * Aligned test
     */
    queue = queue_new();
    for (i=0; i< 100; i++)
        queue_enqueue(&queue, i);

    queue_dequeue(&queue, &i);
    while (queue_dequeue(&queue, &j) != 0) {
        ++i;
        if (i != j)
            abort();
    }
    queue_free(&queue);
    printf("Aligned test OK\n");

    return 0;
}
