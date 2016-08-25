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

#ifndef QUEUE_CONTAINER
#define QUEUE_CONTAINER

#ifndef __QUEUE_INITIAL_SIZE
#define __QUEUE_INITIAL_SIZE 100
#endif

/*
 *
 * A QueueContainer is a dynamic sized circular QUEUE buffer.
 *
 * __QUEUE_CONTAINER_TYPE macro must be defined from the .c file it is used
 * from.
 * __QUEUE_INITIAL_SIZE configure the initial size of the buffer.
 *
 * One .c file can not use more than one queue_buffer type.
 *
 * In this code, we add (enqueue) to tail and consume (dequeue) from head.
 *
 * TODO static void queue_maybe_shrink(QueueContainer*) (maybe).
 *
 */

#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct QueueContainer {
    __QUEUE_CONTAINER_TYPE* things;
    int used;
    int max;
    int tail_position;
    int head_position;
} QueueContainer;

QueueContainer queue_new();
void queue_enqueue(QueueContainer*, __QUEUE_CONTAINER_TYPE);
int  queue_dequeue(QueueContainer*, __QUEUE_CONTAINER_TYPE*);
void queue_free(QueueContainer*);
static void queue_maybe_expand(QueueContainer*);

/*
 * Return a new QueueContainer.
 * Every time QueueContainer reach its max size, it is expanded to twice
 * the current size.
 *
 */
QueueContainer
queue_new()
{
    __QUEUE_CONTAINER_TYPE* things;
    QueueContainer queue_buffer;

    things = malloc(sizeof(__QUEUE_CONTAINER_TYPE) * __QUEUE_INITIAL_SIZE);
    if (things == NULL)
        abort();

    queue_buffer.used = 0;
    queue_buffer.max = __QUEUE_INITIAL_SIZE;
    queue_buffer.tail_position = 0;
    queue_buffer.head_position = 0;
    queue_buffer.things = things;

    return queue_buffer;
}

void
queue_free(QueueContainer* buffer)
{
    free(buffer->things);
}

void
queue_enqueue(
        QueueContainer*        buffer,
        __QUEUE_CONTAINER_TYPE thing)

{
    buffer->things[buffer->tail_position] = thing;
    ++ buffer->used;

    if (buffer->tail_position + 1 == buffer->max)
        // end of circonvolution
        buffer->tail_position = 0;
    else
        buffer->tail_position ++;
    queue_maybe_expand(buffer);

}

int
queue_dequeue(
        QueueContainer*         buffer,
        __QUEUE_CONTAINER_TYPE* value)
{
    if (buffer->used == 0)
        return 0;

    *value = buffer->things[buffer->head_position];
    -- buffer->used;

    if (buffer->head_position + 1 == buffer->max)
        // end of circonvolution
        buffer->head_position = 0;
    else
        buffer->head_position ++;

    return 1;
}

/*
 * If required, realoc and realign new QueueContainer for the new buffer size.
 * Realigning meaning puting the head (where we consume from) at buffer[0].
 */
static
void
queue_maybe_expand(QueueContainer* buffer)
{
    int current_pos, new_size;
    __QUEUE_CONTAINER_TYPE* new_things;

    if (buffer->used < buffer->max)
        return;

    new_size = buffer->max * 2;
    new_things = malloc(new_size * sizeof(__QUEUE_CONTAINER_TYPE));
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
                (buffer->max - current_pos) * sizeof(__QUEUE_CONTAINER_TYPE));
        memcpy(
                &new_things[buffer->max - current_pos],
                &buffer->things[0],
                current_pos * sizeof(__QUEUE_CONTAINER_TYPE));
    } else {
        /* head_position is 0 it must be:
         * [H.........................]
         *  T
         * (Allready aligned)
         */
        memcpy(&new_things[0], &buffer->things[0],
                buffer->max * sizeof(__QUEUE_CONTAINER_TYPE));
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

#endif // QUEUE_CONTAINER
