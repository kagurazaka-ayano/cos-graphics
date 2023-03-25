#ifndef _CG_GRAPHICS_H_
#define _CG_GRAPHICS_H_
#include "defs.h"


#ifndef CG_RENDER_FAR
    /**
     * @brief The largest "z"(or depth) that this program is capable 
     * to render. you can set this by defining @ref CG_RENDER_FAR 
     * before you include this header file
     * @example 
     * #define CG_RENDER_FAR 256
     * #include "graphics.h"        // now the largest z value is set to 256
     */
    #define CG_RENDER_FAR 256
#endif

#ifndef CG_RENDER_NEAR
    /**
     * @brief The smallest "z"(or depth) that this program is capable 
     * to render. you can set this by defining @ref CG_RENDER_NEAR 
     * before you include this header file
     * @example 
     * #define CG_RENDER_FAR -256
     * #include "graphics.h"        // now the smallest z value is set to -256
     */
    #define CG_RENDER_NEAR -256
#endif

/**
 * @brief 2D vector
 */
typedef struct {
    float x;
    float y;
} CGVector2;

/**
 * @brief Window
 */
typedef struct {
    char title[256];
    int width;
    int height;
    CG_BOOL use_full_screen;
    void* glfw_window_instance;
    unsigned int triangle_vao;
    unsigned int quadrangle_vao;
    unsigned int sprite_vao;
} CGWindow;

/**
 * @brief Viewport
 */
typedef struct {
    /**
     * @brief position of viewport
     */
    CGVector2 position;
    /**
     * @brief demension of viewport
     */
    CGVector2 demension;
} CGViewport;

/**
 * @brief Color
 */
typedef struct {
    float r, g, b, alpha;
}CGColor;

/**
 * @brief Construct a Color
 * 
 * @param r red value (range from 0.0 to 1.0)
 * @param g green value (range from 0.0 to 1.0)
 * @param b blue value (range from 0.0 to 1.0)
 * @param alpha alpha value (range from 0.0 to 1.0)
 * @return CGColor the color object
 */
CGColor CGConstructColor(float r, float g, float b, float alpha);

CGVector2 CGConstructVector2(float x, float y);

/**
 * @brief Create window object
 * 
 * @param width width of the window
 * @param height height of the window
 * @param title title of the window
 * @param use_full_screen use full screen
 * @return CGWindow* The window instance. Returns NULL if failed to create window.
 */
CGWindow* CGCreateWindow(int width, int height,const char* title, CG_BOOL use_full_screen);

/**
 * @brief Destroy the window
 * 
 * @param window 
 */
void CGDestroyWindow(CGWindow* window);

/**
 * @brief Create viewport object
 * 
 * @param window window instance
 * @return CGViewport* viewport instance
 */
void CGCreateViewport(CGWindow* window);

/**
 * @brief terminate graphics. 
 * 
 */
void CGTerminateGraphics();

/**
 * @brief Set clear screen color
 * 
 * @param color color to be set
 */
void CGSetClearScreenColor(const CGColor color);

/**
 * @brief start the tick render. Call this every frame before the render.
 */
void CGTickRenderStart(CGWindow* window);

/**
 * @brief end of the tick render. Call this every frame after the render
 * 
 * @param window window instance
 */
void CGTickRenderEnd();

/**
 * @brief should the window be closed
 * 
 * @param window window instance
 */
CG_BOOL CGShouldWindowClose(CGWindow* window);

/**
 * @brief Get current time
 * 
 * @return double number of seconds after this program is initialized
 */
double CGGetCurrentTime();

/**************SHADER**************/

/**
 * @brief Shader sources
 * 
 */
typedef struct{
    /**
     * @brief Vertex shader source
     * 
     */
    char* vertex;
    /**
     * @brief fragment shader source
     * 
     */
    char* fragment;
    /**
     * @brief geometry shader source
     * 
     */
    char* geometry;
    /**
     * @brief use geometry shader source
     * 
     */
    CG_BOOL use_geometry;
}CGShaderSource;

/**
 * @brief Create a shader source object
 * 
 * @param vertex vertex source. If you want to set this manually, you can set this parameter to NULL.
 * @param fragment fragment source If you want to set this manually, you can set this parameter to NULL.
 * @param geometry geometry source If you want to set this manually, you can set this parameter to NULL.
 * @param use_geometry use geometry source. If false, this shader source will ignore the geometry source
 * @return CGShaderSource The CGShaderSource object instance. Returns NULL if failed.
 */
CGShaderSource* CGCreateShaderSource(const char* vertex, const char* fragment, 
    const char* geometry, CG_BOOL use_geometry);

/**
 * @brief Create a CGShaderSource object from file
 * 
 * @param vertex_path vertex shader path
 * @param fragment_path fragment shader path
 * @param geometry_path geometry shader source
 * @param use_geometry use geometry source
 * @return CGShaderSource* 
 */
CGShaderSource* CGCreateShaderSourceFromPath(const char* vertex_path, const char* fragment_path, 
    const char* geometry_path, CG_BOOL use_geometry);

/**
 * @brief Delete the CGShaderSource object used by the shader source
 * 
 * @param source shader source
 */
void CGDeleteShaderSource(CGShaderSource* shader_source);

/**
 * @brief Shaders
 * 
 */
typedef struct{
    /**
     * @brief vertex shader
     * 
     */
    unsigned int vertex;
    /**
     * @brief fragment shader
     * 
     */
    unsigned int fragment;
    /**
     * @brief geometry shader
     * 
     */
    unsigned int geometry;
    /**
     * @brief use geometry shader
     * 
     */
    CG_BOOL use_geometry;
}CGShader;

/**
 * @brief create a CGShader program from shader_source
 * 
 * @param shader_source shader source
 * @return CGShader shader object with shader ids of the compiled shader source
 */
CGShader* CGCreateShader(CGShaderSource* shader_source);

/**
 * @brief Delete shader object
 * 
 * @param shader shader object to be deleted
 */
void CGDeleteShader(CGShader* shader);

typedef unsigned int CGShaderProgram;

/**
 * @brief Create shader program
 * 
 * @param shader linked shader object. If you want to link it manually, you should set this to NULL.
 * @return CGShaderProgram shader program
 */
CGShaderProgram CGCreateShaderProgram(CGShader* shader);

/**
 * @brief use shader program
 * 
 * @param program the shader program. If program is 0, the shader program will be set to the default shader program
 */
void CGUseShaderProgram(CGShaderProgram program);

/**
 * @brief delete shader program
 * 
 * @param program shader program to be deleted
 */
void CGDeleteShaderProgram(CGShaderProgram program);

/**
 * @brief Set opengl shader 1f uniform
 * 
 * @param shader_program shader program
 * @param uniform_name uniform name
 * @param value value
 */
void CGSetShaderUniform1f(CGShaderProgram shader_program, const char* uniform_name, float value);

/**
 * @brief Set opengl shader vector 4f uniform
 * 
 * @param shader_program shader program
 * @param uniform_name uniform name
 * @param val_1 value 1
 * @param val_2 value 2
 * @param val_3 value 3
 * @param val_4 value 4
 */
void CGSetShaderUniformVec4f(
    CGShaderProgram shader_program, const char* uniform_name,
    float val_1, float val_2, float val_3, float val_4
);

/**
 * @brief Set opengl shader matrix 4f uniform
 * 
 * @param shader_program shader program
 * @param uniform_name uniform name
 * @param data uniform data
 */
void CGSetShaderUniformMat4f(CGShaderProgram shader_program, const char* uniform_name, const float* data);

/************GEOMETRIES************/

/**
 * @brief Geometry properties for geometry objects.
 */
typedef struct{
    /**
     * @brief color of the geometry
     */
    CGColor color;
    /**
     * @brief transform of the geometry
     */
    CGVector2 transform;
    /**
     * @brief scale of the geometry
     */
    CGVector2 scale;
    /**
     * @brief rotation of the geometry (in radians)
     */
    float rotation;
}CGGeometryProperty;

/**
 * @brief Create CGGeometry property object
 * 
 * @param color color
 * @param transform transform
 * @param scale scale
 * @param rotation rotation (clockwise)
 * @return CGGeometryProperty* geometry property object instance
 */
CGGeometryProperty* CGCreateGeometryProperty(CGColor color, CGVector2 transform, CGVector2 scale, float rotation);

/**
 * @brief Triangle
 */
typedef struct{
    /**
     * @brief first vertex position
     */
    CGVector2 vert_1;
    /**
     * @brief second vertex position
     */
    CGVector2 vert_2;
    /**
     * @brief third vertex position
     */
    CGVector2 vert_3;
    /**
     * @brief The depth of the geometry. This value must between @ref CG_RENDER_NEAR 
     * and @ref CG_RENDER_FAR (-512 to 512 by default)
     */
    float z;
    /**
     * @brief Geometry properties of the triangle.
     */
    CGGeometryProperty* property;
}CGTriangle;

/**
 * @brief Construct a triangle
 * 
 * @param vert_1 vertex 1
 * @param vert_2 vertex 2
 * @param vert_3 vertex 3
 * @return CGTriangle trangle instance
 */
CGTriangle CGConstructTriangle(CGVector2 vert_1, CGVector2 vert_2, CGVector2 vert_3);
/**
 * @brief Create a triangle
 * 
 * @param vert_1 vertex 1
 * @param vert_2 vertex 2
 * @param vert_3 vertex 3
 * @return CGTriangle* triangle instance
 */
CGTriangle* CGCreateTriangle(CGVector2 vert_1, CGVector2 vert_2, CGVector2 vert_3);

/**
 * @brief Set property to a triangle
 * 
 * @param triangle triangle to set on
 * @param property triangle property
 */
void CGSetTriangleProperty(CGTriangle* triangle, CGGeometryProperty* property);

/**
 * @brief Draw triangle on screen.
 * 
 * @param triangle triangle to draw on screen
 * @param window window to draw on
 */
void CGDrawTriangle(CGTriangle* triangle, CGWindow* window);

typedef struct{
    /**
     * @brief first vertex position
     */
    CGVector2 vert_1;
    /**
     * @brief second vertex position
     */
    CGVector2 vert_2;
    /**
     * @brief third vertex position
     */
    CGVector2 vert_3;
    /**
     * @brief forth vertex position
     */
    CGVector2 vert_4;
    /**
     * @brief The depth of the geometry. This value must between @ref CG_RENDER_NEAR 
     * and @ref CG_RENDER_FAR (-512 to 512 by default)
     */
    float z;
    /**
     * @brief Geometry property for the quadrangle
     */
    CGGeometryProperty* property;
}CGQuadrangle;

/**
 * @brief Construct a CGQuadrangle object
 * 
 * @param vert_1 vertex 1
 * @param vert_2 vertex 2
 * @param vert_3 vertex 3
 * @param vert_4 vertex 4
 * @return CGQuadrangle constructed triangle data
 */
CGQuadrangle CGConstructQuadrangle(CGVector2 vert_1, CGVector2 vert_2, CGVector2 vert_3, CGVector2 vert_4);

/**
 * @brief Create quadrangle object
 * 
 * @param vert_1 vertex 1
 * @param vert_2 vertex 2
 * @param vert_3 vertex 3
 * @param vert_4 vertex 4
 * @return CGQuadrangle* created quadrangle object
 */
CGQuadrangle* CGCreateQuadrangle(CGVector2 vert_1, CGVector2 vert_2, CGVector2 vert_3, CGVector2 vert_4);

/**
 * @brief Draw a quadrangle on window
 * 
 * @param quadrangle the quadrangle to draw
 * @param window the window to draw on
 */
void CGDrawQuadrangle(CGQuadrangle* quadrangle, CGWindow* window);


/************SPRITES************/

typedef struct{
    /**
     * @brief transform of the geometry
     */
    CGVector2 transform;
    /**
     * @brief scale of the geometry
     */
    CGVector2 scale;
    /**
     * @brief rotation of the geometry (in radians)
     */
    float rotation;
}CGSpriteProperty;

/**
 * @brief Create CGSpriteProperty object
 * 
 * @param transform transform
 * @param scale scale
 * @param rotation rotation
 * @return CGSpriteProperty* CGSpriteProperty object
 */
CGSpriteProperty* CGCreateSpriteProperty(CGVector2 transform, CGVector2 scale, float rotation);

typedef struct{
    /**
     * @brief Texture's OpenGL ID
     */
    unsigned int texture_id;
    /**
     * @brief The depth of the geometry. This value must between @ref CG_RENDER_NEAR 
     * and @ref CG_RENDER_FAR (-512 to 512 by default)
     */
    float z;
    /**
     * @brief Sprite property.
     */
    CGSpriteProperty* property;
    /**
     * @brief The window that the sprite is created in.
     */
    CGWindow* in_window;
    /**
     * @brief the width and height of the sprite
     */
    CGVector2 demention;
}CGSprite;

/**
 * @brief Create CGSprite object
 * 
 * @param img_path texture image path
 * @param property sprite property object
 * @param window the window that the sprite is going to be drawn.
 * @return CGSprite* The created CGSprite object
 */
CGSprite* CGCreateSprite(const char* img_path, CGSpriteProperty* property, CGWindow* window);

/**
 * @brief Delete CGSprite object
 * 
 * @param sprite sprite object instance to be deleted
 */
void CGDeleteSprite(CGSprite* sprite);

/**
 * @brief Draw sprite on window
 * 
 * @param sprite sprite to be drawn
 * @param window window to draw on
 */
void CGDrawSprite(CGSprite* sprite, CGWindow* window);

#endif  //_CG_GRAPHICS_H_
