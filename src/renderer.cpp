#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "../include/renderer.h"


void initGL(GraphicsData& gd, const size_t size)
{
    // window initialization
    gd.window.initialise();

    // create shaders
    gd.shaderMain.loadShaders("../shaders/shader.vert", "../shaders/shader.frag",
                            "../shaders/shader.geom");
    gd.shaderMain.validateProgram();

    gd.shaderMain.useProgram();
    gd.shaderMain.setUniform("size", (int)size);
    glUseProgram(0);

//----------------------------------------------------------------
    glGenVertexArrays(1, &gd.VAO);
    glGenBuffers(1, &gd.VBO);

    glBindVertexArray(gd.VAO);

    //position
    glBindBuffer(GL_ARRAY_BUFFER, gd.VBO);
    glBufferData(GL_ARRAY_BUFFER, buf_size * sizeof(unsigned int),
                                            nullptr, GL_STATIC_DRAW);
    glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, 
                                0, (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void renderGL(GraphicsData& gd)
{
    while(!gd.window.getShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        // process input
        glfwPollEvents();
    
        // draw
        gd.shaderMain.useProgram();
        glBindVertexArray(gd.VAO);
        glDrawArrays(GL_POINTS, 0, buf_size);
        glBindVertexArray(0);

        // swap buffers
        glUseProgram(0);
        gd.window.swapBuffers();
    }
}

void cleanupGL(GraphicsData& gd)
{
    glDeleteVertexArrays(1, &gd.VAO);
    glDeleteBuffers(1, &gd.VBO);
    gd.shaderMain.deleteShader();
}
