#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//resize callback function - when user resizes window, viewport is also adjusted
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    glfwInit();

    //initializes the major and minor versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //core profile = less features without backwards compatible features we don't need
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfwCreateWindow(width, height, name, ignore, ignore) - returns GLDWwindow object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    //terminate window if null
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //call on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //GLAD manages function pointers for OpenGL, initialize first
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //checks if GLFW has been instructed to close
    while(!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering goes here so it executes in each iteration/frame of the loop
        //example rendering (clears window with a specific color)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //swaps buffers (see what a double buffer is)
        glfwSwapBuffers(window);

        //checks if any events are triggered
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        //first two parameters set location of lower left corner
        //glViewport(0, 0, 800, 600);
        glViewport(0, 0, width, height);
    }

//allows some form of input control
void processInput(GLFWwindow *window)
    {
        //glfwGetKey takes in a window and a key input, and returns whether key is pressed
        //(in this case, we are checking if the escape key is pressed, and if it is, we close the window)
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

