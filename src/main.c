
#if 0
#include "cos_graphics/graphics.h"
#include "cos_graphics/log.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    CGWindow* window = CGCreateWindow(640, 480, "Graphics test", CG_FALSE, CG_FALSE);
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
        CGWindowDraw(window);

        CGTickRenderEnd(window);
    }
    free(triangle->property);
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
#ifdef CG_TG_WIN
#include<windows.h>
#include<conio.h>
#elif defined CG_TG_LINUX
#include <unistd.h>
#endif

void KeyCallback(CGWindow* window, int key, int action, int mods);
int main()
{
    CGWindow* window = CGCreateWindow(640, 480, "Graphics test", CG_FALSE, CG_TRUE);
    CGSetKeyCallback(KeyCallback);
    
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
    CGRenderObjectProperty* quad1_property = CGCreateRenderObjectProperty(
        CGConstructColor(1.0f, 0.0f, 0.0f, 0.5f),
        (CGVector2){-50, 0},
        (CGVector2){1, 1},
        rotation);
    CGRenderObjectProperty* quad2_property = CGCreateRenderObjectProperty(
        CGConstructColor(0.0f, 1.0f, 0.0f, 0.8f),
        (CGVector2){0, 0},
        (CGVector2){1, 1},
        rotation
    );
    
    CGVisualImage* sprite = CGCreateVisualImage("test2", 
        window);
    quad2_property->z = -3;
    double tick_end_time = CGGetCurrentTime();
    const double fixed_delta = 1.0 / 60;
    while(!CGShouldWindowClose(window))
    {
        static double tick_start_time = 0;
        static double delta = 0.01;
        tick_start_time = CGGetCurrentTime();
        static float clock = 0;
        clock += delta * 3;
        quad1.vert_1 = CGConstructVector2(sin(clock / 0.3) * 30 + 150, sin(clock) * 20 - 50);
        quad1.vert_2 = CGConstructVector2(sin(clock / 0.5) * 30 + 100, sin(clock / 0.7) * 20 + 50);
        quad1.vert_3 = CGConstructVector2(sin(clock / 2) * 30 - 150, sin(clock / 0.3 - 3) * 20 + 50);
        quad1.vert_4 = CGConstructVector2(sin(clock / 0.8) * 30 - 100, sin(clock / 0.9 + 5) * 20 - 50);
        CGTickRenderStart(window);
        CGDrawTriangle(&triangle, NULL, window);
        CGDrawTriangle(&triangle, quad2_property, window);
        CGRenderObjectProperty* prop = CGCreateRenderObjectProperty((CGColor){1.0f, 1.0f, 1.0f, 1.0f}, (CGVector2){0, 0}, (CGVector2){1, 1}, 0);
        CGDrawVisualImage(sprite, 
            prop, 
            window);
        CGRotateRenderObject(quad1_property, delta, (CGVector2){100, 100});
        //quad2_property->rotation = clock * 0.2;
        CGDrawQuadrangle(&quad1, quad1_property, window);
        CGWindowDraw(window);
        CGTickRenderEnd();
        tick_end_time = CGGetCurrentTime();
        delta = tick_end_time - tick_start_time;
        CGFreeResource(prop);
    }
    CGFreeResource(window);
    CGTerminateGraphics();
    
    return 0;
}
void KeyCallback(CGWindow* window, int key, int action, int mods)
{
    if (key == CG_KEY_ESCAPE && action == CG_PRESS)
        CG_PRINT("QAQ");
}

#endif

#if 0
#include "cos_graphics/graphics.h"
#include "cos_graphics/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    CGWindow* window = CGCreateWindow(1920, 1080, "Graphics test", CG_FALSE, CG_FALSE);
    CGSetClearScreenColor((CGColor){1.0f, 1.0f, 1.0f, 1.0f});
    window->width = 1920;
    window->height = 1080;
    if (window == NULL)
        return 0;
    float rotation = 0.0f;
    CGVisualImage* sprite = CGCreateVisualImage("./test2.png", 
        CGCreateRenderObjectProperty((CGColor){1.0f, 1.0f, 1.0f, 0.5f}, (CGVector2){0, 0}, (CGVector2){1, 1}, 0), window);
    CGVisualImage* sprite3 = CGCreateVisualImage("./test2.png", 
        CGCreateRenderObjectProperty((CGColor){1.0f, 1.0f, 1.0f, 0.5f}, (CGVector2){300, 600}, (CGVector2){1, 1}, 0), window);
    double tick_end_time = CGGetCurrentTime();
    const double fixed_delta = 1.0 / 60;
    while(!CGShouldWindowClose(window))
    {
        static double tick_start_time = 0;
        static double delta = 0.01;
        tick_start_time = CGGetCurrentTime();

        CGTickRenderStart(window);
        CGDrawVisualImage(sprite3, window);
        CGDrawVisualImage(sprite, window);
        CGWindowDraw(window);
        CGTickRenderEnd();
        while(CGGetCurrentTime() < tick_start_time + fixed_delta);
        tick_end_time = CGGetCurrentTime();
        delta = tick_end_time - tick_start_time;
        CG_PRINT("%f", delta);
    }
    free(sprite->property);
    CGDeleteVisualImage(sprite);
    CGDestroyWindow(window);
    CGTerminateGraphics();
    window = NULL;
    return 0;
}
#endif