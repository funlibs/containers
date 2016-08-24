/*
 * MIT License 2016 Sebastien Serre <ssbx@sysmo.io>.
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

#ifndef FIFO_CONTAINER
#define FIFO_CONTAINER

#ifndef __FIFO_INITIAL_SIZE
#define __FIFO_INITIAL_SIZE 100
#endif

/*
 *
 * A FifoContainer is a dynamic sized circular FIFO buffer.
 *
 * __FIFO_CONTAINER_TYPE macro must be defined from the .c file it is used
 * from.
 * __FIFO_INITIAL_SIZE configure the initial size of the buffer.
 *
 * One .c file can not use more than one fifo_buffer type.
 *
 * In this code, we add (push) to tail and consume (pop) from head.
 *
 * TODO static void fifo_maybe_shrink(FifoContainer*) (maybe).
 *
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct FifoContainer {
    __FIFO_CONTAINER_TYPE* things;
    int used;
    int max;
    int tail_position;
    int head_position;
} FifoContainer;

FifoContainer fifo_new();
void fifo_push(FifoContainer*, __FIFO_CONTAINER_TYPE);
int  fifo_pop(FifoContainer*, __FIFO_CONTAINER_TYPE*);
void fifo_free(FifoContainer*);
static void fifo_maybe_expand(FifoContainer*);

/*
 * Return a new FifoContainer.
 * Every time FifoContainer reach its max size, it is expanded to twice
 * the current size.
 *
 */
FifoContainer
fifo_new()
{
    void* things;
    FifoContainer fifo_buffer;

    things = malloc(sizeof(__FIFO_CONTAINER_TYPE) * __FIFO_INITIAL_SIZE);
    if (things == NULL)
        abort();

    fifo_buffer.used = 0;
    fifo_buffer.max = __FIFO_INITIAL_SIZE;
    fifo_buffer.tail_position = 0;
    fifo_buffer.head_position = 0;
    fifo_buffer.things = things;

    return fifo_buffer;
}

void
fifo_free(FifoContainer* buffer)
{
    free(buffer->things);
}

void
fifo_push(
        FifoContainer*        buffer,
        __FIFO_CONTAINER_TYPE thing)

{
    buffer->things[buffer->tail_position] = thing;
    ++ buffer->used;

    if (buffer->tail_position + 1 == buffer->max)
        // end of circonvolution
        buffer->tail_position = 0;
    else
        buffer->tail_position ++;
    fifo_maybe_expand(buffer);

}

int
fifo_pop(
        FifoContainer*         buffer,
        __FIFO_CONTAINER_TYPE* value)
{
    if (buffer->used == 0)
        return -1;

    *value = buffer->things[buffer->head_position];
    -- buffer->used;

    if (buffer->head_position + 1 == buffer->max)
        // end of circonvolution
        buffer->head_position = 0;
    else
        buffer->head_position ++;

    return 0;
}

/*
 * If required, realoc and realign new FifoContainer for the new buffer size.
 * Realigning meaning puting the head (where we consume from) at buffer[0].
 */
static
void
fifo_maybe_expand(FifoContainer* buffer)
{
    int current_pos, new_size;
    __FIFO_CONTAINER_TYPE* new_things;

    if (buffer->used < buffer->max)
        return;

    new_size = buffer->max * 2;
    new_things = malloc(new_size * sizeof(__FIFO_CONTAINER_TYPE));
    if (new_things == NULL)
        abort();

    /*
     * Put head at 0 and reset head_position tail_position and max.
     */
    if (buffer->head_position > 0) {
        /*
         * Can look like this:
         * [...............T..........]
         *                 H
         * Copy:
         * - From head to end of array
         * Then:
         * - From begin of array to head
         */
        current_pos = buffer->head_position;
        memcpy(
                &new_things[0],
                &buffer->things[current_pos],
                (buffer->max - current_pos) * sizeof(__FIFO_CONTAINER_TYPE));
        memcpy(
                &new_things[buffer->max - current_pos],
                &buffer->things[0],
                current_pos * sizeof(__FIFO_CONTAINER_TYPE));
    } else {
        /* head_position is 0 it must be:
         * [H.........................]
         *  T
         * (Allready aligned)
         */
        memcpy(&new_things[0], &buffer->things[0],
                buffer->max * sizeof(__FIFO_CONTAINER_TYPE));
    }

    free(buffer->things);
    buffer->things = new_things;
    buffer->tail_position = buffer->max;
    buffer->head_position = 0;
    buffer->max = new_size;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // FIFO_CONTAINER
