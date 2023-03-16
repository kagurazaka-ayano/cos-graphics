#if 0
#include "cos_graphics/graphics.h"
#include "cos_graphics/log.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    CGWindow* window = CGCreateWindow(640, 480, "Graphics test", CG_FALSE);
    if (window == NULL)
        return 0;
    CGTriangle *triangle = CGCreateTriangle( 
        (CGVector2){100, 100},
        (CGVector2){100, -100},
        (CGVector2){-100, 100});
    
    while(!CGShouldWindowClose(window))
    {
        CGTickRenderStart(window);
        CGDrawTriangle(triangle, window);

        CGTickRenderEnd(window);
    }
    CGDeleteGeometryProperty(triangle->property);
    free(triangle);
    CGTerminateGraphics();
    free(window);
    return 0;
}
#endif

#if 1
#include "cos_graphics/graphics.h"
#include "cos_graphics/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    CGWindow* window = CGCreateWindow(640, 480, "Graphics test", CG_FALSE);
    if (window == NULL)
        return 0;
    CGQuadrangle quad1 = CGConstructQuadrangle(
        (CGVector2){-100, -50},
        (CGVector2){-50, 50},
        (CGVector2){50, 100},
        (CGVector2){50, -100});
    CGQuadrangle quad2 = CGConstructQuadrangle(
        (CGVector2){-100, -100},
        (CGVector2){-50, 50},
        (CGVector2){50, 100},
        (CGVector2){100, -100});
    CGTriangle triangle = CGConstructTriangle( 
        (CGVector2){100, 100},
        (CGVector2){100, -100},
        (CGVector2){-100, 100});
    float rotation = 0.0f;
    triangle.z = 10;
    quad1.property = CGCreateGeometryProperty(
        CGConstructColor(1.0f, 0.0f, 0.0f, 0.8f),
        (CGVector2){-50, 0},
        (CGVector2){1, 1},
        rotation);
    quad2.property = CGCreateGeometryProperty(
        CGConstructColor(1.0f, 0.0f, 0.0f, 0.8f),
        (CGVector2){-50, 0},
        (CGVector2){1, 1},
        rotation
    );
    double tick_end_time = CGGetCurrentTime();
    while(!CGShouldWindowClose(window))
    {
        static double tick_start_time = 0;
        static double delta = 0.01;
        tick_start_time = CGGetCurrentTime();
        
        static float clock = 0;
        clock += delta * 3;
        quad1.property->scale.x = sin(clock);
        quad1.property->scale.y = cos(clock);
        quad1.property->transform.x = sin(clock + 1.3) * 300;
        quad2.property->scale.x = cos(clock - 2.5);
        quad2.property->scale.y = sin(clock - 2.5);
        CGTickRenderStart(window);
        CGDrawQuadrangle(&quad2, window);
        CGDrawQuadrangle(&quad1, window);
        CGDrawTriangle(&triangle, window);
        CGTickRenderEnd();
        tick_end_time = CGGetCurrentTime();
        delta = tick_end_time - tick_start_time;
    }
    CGDestroyWindow(window);
    CGTerminateGraphics();
    window = NULL;
    CGDeleteGeometryProperty(quad1.property);
    return 0;
}
#endif