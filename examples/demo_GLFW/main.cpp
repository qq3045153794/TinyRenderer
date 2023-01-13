#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(void)
{
    // GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    // window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    // if (!window)
    // {
    //     glfwTerminate();
    //     return -1;
    // }

    /* Make the window's context current */
    // glfwMakeContextCurrent(window);


    GLuint vao, vbo;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      return -1;
    }

    glGenVertexArrays(1, &vao);

    glGenBuffers(1, &vbo);


    glBufferData(GL_ARRAY_BUFFER, 6, NULL, GL_STATIC_DRAW);


   /* Loop until the user closes the window */
   /*
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
   */ 
    glfwTerminate();
    return 0;
}
