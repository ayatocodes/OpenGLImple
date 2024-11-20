#include <iostream>
#include <string>

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "glfw3.h"

static void GetSystemInfo()
{
    std::cout << glGetString(GL_VENDOR) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

int main()
{
    /* Initialize the library */
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    const bool shouldFullscreen = false;

    GLFWwindow* window;

    /* Get primary monitor for width and height of the window */ 
    GLFWmonitor* mainMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(mainMonitor);

    /* Create a fullscreen mode window and its OpenGL context */
    if (shouldFullscreen) window = glfwCreateWindow(videoMode->width, videoMode->height, "OpenGL", mainMonitor, nullptr);
    /* Create a windowed mode window and its OpenGL context */  
    else window = glfwCreateWindow(videoMode->width-640, videoMode->height-360, "OpenGL", nullptr, nullptr);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) std::cout << "An Error has occurred while initializing GLEW\n";
    
    GetSystemInfo();

    const float vertices[] = {
        -0.25f, -0.45f, 0.0f,
        0.25f, -0.45f, 0.0f,
        0.25f, 0.45f, 0.0f,
        -0.25f, 0.45f, 0.0f,
        0.25f, -0.45f, -0.25f,
        0.25f, 0.45f, -0.25f,
        -0.25f, -0.45f, -0.25f,
        -0.25f, 0.45f, -0.25f,
    };
    
    unsigned int indices[] = {
        0, 1, 2,        //frontface 1
        2, 3, 0,        //frontface 2
        1, 4, 5,        //rightside 1
        5, 2, 1,        //rightside 2
        6, 4, 5,        //backface 1
        5, 7, 6,         //backface 2
        6, 0, 2,        //leftside 1
        2, 7 ,6,        //leftside 2
        3, 2, 5,        //upside 1
        5, 6, 3,        //upside 2
        0, 1, 4,
        4, 6, 0
    };
    
    VertexArray* va = new VertexArray();

    VertexBuffer* vb = new VertexBuffer(vertices, sizeof(vertices));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    va->AddBuffer(*vb, layout);

    IndexBuffer* ib = new IndexBuffer(indices, 36);
    
    Shader* shader = new Shader("res/shaders/vertexshaders/vShader.shader", "res/shaders/fragmentshaders/fShader.shader");
    shader->Bind();
    float r = 0.2f, g = 0.7f, b = 1.0f;
    shader->SetUniform4f("u_Color", r, g, b, 1.0f);

    float increment = 0.05f;

    va->Unbind();
    shader->Unbind();
    vb->Unbind();
    ib->Unbind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //Bind each buffer for each draw call
        GLCALL(shader->Bind());

        GLCALL(shader->SetUniform4f("u_Color", r, g, b, 1.0f));
        GLCALL(va->Bind());
        GLCALL(ib->Bind());

        GLCALL(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr));
        
        if (g >= 1.0f) increment = -0.005f;
        else if (g <= 0.0f) increment = 0.005f;
        g += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    delete ib, vb, va, shader;
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}