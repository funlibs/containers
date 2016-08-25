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

#ifndef STACK_CONTAINER
#define STACK_CONTAINER

#ifndef __STACK_INITIAL_SIZE
#define __STACK_INITIAL_SIZE 100
#endif

#include <stdlib.h>

typedef struct StackContainer {
    __STACK_CONTAINER_TYPE* things;
    int max;
    int used;
} StackContainer;

StackContainer stack_new();
void stack_push(StackContainer*, __STACK_CONTAINER_TYPE);
int  stack_pop(StackContainer*,  __STACK_CONTAINER_TYPE*);
void stack_free(StackContainer*);
static void stack_expand(StackContainer*);

/*
 * Return a new StackContainer.
 * Every time StackContainer reach its max size, it is expanded to twice
 * the current size.
 *
 */
StackContainer
stack_new()
{
    StackContainer stack_buffer;
    __STACK_CONTAINER_TYPE* things;

    things = malloc(sizeof(__STACK_CONTAINER_TYPE) * __STACK_INITIAL_SIZE);
    if (things == NULL)
        abort();

    stack_buffer.used = 0;
    stack_buffer.max = __STACK_INITIAL_SIZE;
    stack_buffer.things = things;

    return stack_buffer;
}

void
stack_free(StackContainer* buffer)
{
    free(buffer->things);
}

void
stack_push(
        StackContainer*        buffer,
        __STACK_CONTAINER_TYPE thing)

{
    buffer->things[buffer->used] = thing;
    ++ buffer->used;

    if (buffer->used == buffer->max)
        stack_expand(buffer);
}


int
stack_pop(
        StackContainer*         buffer,
        __STACK_CONTAINER_TYPE* value)
{
    if (buffer->used == 0)
        return 0;

    *value = buffer->things[buffer->used - 1];
    -- buffer->used;

    return 1;
}

/*
 * Double the container size
 */
static
void
stack_expand(StackContainer* buffer)
{
    __STACK_CONTAINER_TYPE* new_things;
    size_t new_max;

    new_max    = 2 * buffer->max * sizeof(__STACK_CONTAINER_TYPE);
    new_things = realloc(&buffer->things[0], new_max);

    if (new_things == NULL) {
        abort();
    }

    if (buffer->things != new_things) {
        free(buffer->things);
        buffer->things = new_things;
    }

    buffer->max = new_max;
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STACK_CONTAINER
