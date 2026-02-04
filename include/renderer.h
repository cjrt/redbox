#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>

bool renderer_init(void);

void renderer_draw(void);

// free resources
void renderer_shutdown(void);

#endif
