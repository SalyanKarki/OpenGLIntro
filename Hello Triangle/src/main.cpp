#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//source code for a vertex shader
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

//source code for a fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    glfwInit();


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //vertex shader is a shader that is programmable, need to set one up to render

    //creates shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //next, we attach the shader code to the shader object
    //glShaderSource(shader_object, num_of_strings, source_code, NULL)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /*
    
    the last shader we create is a fragment shader, which calculates the color output of pixerls

    in this case, we will make it an orange color
    
    */
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    //next, we need to create a shader program which links the two shaders together
    unsigned int shaderProgram;

    //creates a program and returns ID reference to program object
    shaderProgram = glCreateProgram();

    //then we link the shaders to the program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glUseProgram(shaderProgram);

    //delete shader objects once linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //stores 3 vertices in normalized device coordinates
    //vertices have z - value of 0 which makes it 2D
    //NDC: withon -1.0 - 1.0
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f, 
        0.0f, 0.5f, 0.0f
    };

    //we can managae memory using vertex buffer objects than can stores large number
    //of vertices in the GPU's memory
   
   // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
   
    unsigned int VBO;

     /*
    
    we can now use a VAO (vertex array object) 

    helps us encapsulates all the object needed to get vertices in a GPU with a
    single object

    the VAO stores vertex bindings, and when you draw it, it uses the vertex bindings
    in the current VAO

    instead of redoing the vertex bindings everytime we change what we are drawing, we
    can do that in a single call with a VAO
    
    */
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    //binds created buffer to the vertex buffer object
    //any buffer calls we make on GL_ARRAY_BUFFER will configure current bound buffer (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*we can use glBufferData which copies the vertex data into the buffer's memory
      glBufferData(type_of_buffer, size_of_data, data, how_data_is_managed)
      how data is managed: 
      GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
      GL_STATIC_DRAW: the data is set only once and used many times.
      GL_DYNAMIC_DRAW: the data is changed a lot and used many times.*/
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /*vertex data has been stored in the graphics card, and managed by vertex buffer object names VBO

      
    
    */

    

    /*
    what is done so far - sent vertex data to GPU, instructed GPU on how to process vertex data 
    using the vertex and fragment shader
    */

    //next we tell OpenGL how to interpret the vertex data 

    /*
    glVertexAttribPointer parameters:

    first - which vertex atttribute we want to configure
            
            in the vertex shader, we specified the location of the position 
            vertex attribute with layout (location = 0), which sets the vertex 
            attribute to 0
    
    second - size of the vertex attribute
             
             since the vertex attribute is vec3, it is composed of 3 values (x, y, z)
    
    third - type of argument

            vec* is a floating point value

    fourth - if we want the data to be normalized

             not relevant, so we set it to false
    
    fifth -  stride -> tells us the space between consecutive vertex attributes
             
             next set of position data is located 3 timex the size of a float (since
             each points is 3 float values)

    sixth - last parameter is type void* - offsett of where the position data begins
            in the buffer

            position data is start of the array, so value is 0

    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //we have now specified how openGL should interpret the vertex data

    //unbinds the VAO and VBO
   glBindBuffer(GL_ARRAY_BUFFER, 0); 
   glBindVertexArray(0);
  



    while(!glfwWindowShouldClose(window))
    {

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //draws triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    //deallocate resouces once done
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
    
        glViewport(0, 0, width, height);
    }

void processInput(GLFWwindow *window)
    {
      
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

/*

things to note:

vertex shader - manipulates attributes of vertices eg. corner points of polygons
fragment shader - how pixels between vertices look eg. color of pixels

VBO (vertex buffer object) - 



*/
