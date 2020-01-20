#ifndef _RENDERER_
#define _RENDERER_

#include <GL/glew.h>
#include "Window.h"
#include "Shader.h"

static constexpr unsigned int buf_size = 256u;

struct GraphicsData
{
    Window window;
    Shader shaderMain;
    GLuint VAO;
    GLuint VBO;
};

void initGL(GraphicsData& gd, const size_t size);
void renderGL(GraphicsData& gd);
void cleanupGL(GraphicsData& gd);

#endif // _RENDERER_
