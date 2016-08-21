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

/*
 * No #ifdef FIFO_BUFFER here.
 *
 * The ____FIFO_BUFFER_TYPE macro must be defined from the .c file it is used
 * from.
 *
 * One .c file can not use more than one fifo_buffer.
 *
 */

# include <string.h>
# include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct FifoBuffer {
    __FIFO_BUFFER_TYPE* things;
    int   used;
    int   max;
    int   tail_position;
    int   head_position;
} FifoBuffer;

void fifo_maybe_realloc_buffer(FifoBuffer* buffer);

/*
 * Return a new FifoBuffer.
 * Every time FifoBuffer reach its max size, it is expanded to twice the current
 * size.
 */
FifoBuffer
fifo_new(int initial_size)
{
    void* things;
    FifoBuffer fifo_buffer;

    things = malloc(sizeof(__FIFO_BUFFER_TYPE) * initial_size);
    if (things == NULL)
        abort();

    fifo_buffer.used = 0;
    fifo_buffer.max = initial_size;
    fifo_buffer.tail_position = 0;
    fifo_buffer.head_position = 0;
    fifo_buffer.things = things;

    return fifo_buffer;
}


void
fifo_enqueue(FifoBuffer* buffer, __FIFO_BUFFER_TYPE thing)
{
    fifo_maybe_resize(buffer);
    buffer->things[buffer->tail_position] = thing;
    buffer->used ++;

    // loop over
    if (buffer->tail_position + 1 == buffer->max)
        buffer->tail_position = 0;
    else
        buffer->tail_position ++;
}

__FIFO_BUFFER_TYPE
fifo_dequeue(FifoBuffer* buffer)
{
    __FIFO_BUFFER_TYPE thing = buffer->things[buffer->head_position];
    buffer->used --;

    // loop over
    if (buffer->head_position + 1 == buffer->max)
        buffer->head_position = 0;
    else
        buffer->head_position ++;

    return thing;
}

/*
 * If required, realoc and realign new FifoBuffer for the new buffer size.
 * Realigning meaning puting the head at buffer[0].
 */
int
fifo_maybe_resize(FifoBuffer* buffer)
{
    void* new_things;
    int head_end, tail_end, copy_head_size, copy_tail_size, new_size;

    if (buffer->used < buffer->max)
        return;

    new_size = buffer->max * 2;
    new_things = malloc(new_size * sizeof(__FIFO_BUFFER_TYPE));
    if (new_things == NULL)
        abort();

    /*
     * Put head at 0 and reset head_position tail_position and max.
     */
    if (buffer->head_position > buffer->tail_position) {
        /*
         * [...............TH.........]
         *
         * From head_position(H) to end of array
         * THEN
         * From begin of array to tail_position(T)
         */
        memcpy(
                new_things,
                &buffer->things[buffer->head_position],
                (buffer->max -
                 buffer->head_position) * sizeof(__FIFO_BUFFER_TYPE));
        memcpy(
                new_things,
                &buffer->things[0],
                buffer->tail_position * sizeof(__FIFO_BUFFER_TYPE));
    } else {
        /*
         * Then it must look like this:
         * [H........................T]
         * (Allready aligned)
         */
        memcpy(
            new_things,
            &buffer->things[0],
            buffer->max * sizeof(__FIFO_BUFFER_TYPE));
    }

    free(buffer->things);
    buffer->things = new_things;
    buffer->head_position = 0;
    buffer->tail_position = buffer->max;
    buffer->max = new_size;
}

#ifdef __cplusplus
}
#endif // __cplusplus

