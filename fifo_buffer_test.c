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

#define __FIFO_BUFFER_TYPE int
#define __FIFO_INITIAL_SIZE 10
#include <fifo_buffer.h>

void debug_fifo_buffer(FifoBuffer* buffer)
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
    int i, j, r;

    FifoBuffer buffer = fifo_new(10);

    for (i=0; i<5; i++) {
        fifo_enqueue(&buffer, i);
    }

    fifo_dequeue(&buffer, &j);

    for (i=5; i<11; i++) {
        fifo_enqueue(&buffer, i);
    }


    fifo_dequeue(&buffer, &j);
    //debug_fifo_buffer(&buffer);

    for (i=11; i<30; i++) {
        fifo_enqueue(&buffer, i);
    }

    fifo_dequeue(&buffer, &j);
    fifo_dequeue(&buffer, &j);
    fifo_dequeue(&buffer, &j);
    fifo_dequeue(&buffer, &j);
    fifo_dequeue(&buffer, &j);
    //debug_fifo_buffer(&buffer);

    for (i=30; i<100; i++) {
        fifo_enqueue(&buffer, i);
    }

    fifo_dequeue(&buffer, &i);
    while (fifo_dequeue(&buffer, &j) == 0) {
        ++i;
        if (i != j)
            abort();
    }

    fifo_free(&buffer);

    printf("Unaligned test OK\n");

    buffer = fifo_new();
    for (i=0; i< 100; i++)
        fifo_enqueue(&buffer, i);

    fifo_dequeue(&buffer, &i);
    while (fifo_dequeue(&buffer, &j) == 0) {
        ++i;
        if (i != j)
            abort();
    }

    printf("Aligned test OK\n");


    return 0;
}
