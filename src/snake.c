#include <gb/gb.h>
#include "snake.h"

Snake snake;

void snake_init()
{
    snake.tail = snake.nodes;
    snake.head = snake.nodes;
}

SnakeNode *snake_get_head()
{
    return snake.head;
}

SnakeNode *snake_get_tail()
{
    return snake.tail;
}

SnakeNode *snake_advance_head()
{
    snake.head++;
    if (snake.head >= (snake.nodes + SNAKE_BUFFER_CAPACITY))
    {
        snake.head = snake.nodes;
    }
    return snake.head;
}

SnakeNode *snake_advance_tail()
{
    snake.tail++;
    if (snake.tail >= (snake.nodes + SNAKE_BUFFER_CAPACITY))
    {
        snake.tail = snake.nodes;
    }
    return snake.tail;
}
