#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// https://docs.gl
static unsigned int compileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    // TODO: ERROR HANDLING

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) { // GL_FALSE value is 0 by default.
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // alloca is given by c and allows you to allocate on the stack dynamically
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

// Read shader files as strings, could be read as a .txt file
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // GLint is the same as unsigned int. c++ type is more convenient between multiple graphics apis
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs); // glDetachShader removes debugging ablities, deletes shader from GPU
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
        glfwInit();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!~" << std::endl;
    
    std::cout << glGetString(GL_VERSION) << std::endl;


    // Create a vertex buffer. Start with these vertex positions. Note vertex can include more than positions. 
    float positions[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // acts like a layer in photoshop, buffer selects this layer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); // this size is in bytes


    // Specify the layout of the buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml

    // we're giving the shader a vec2 but opengl automatically converts vec2 to vec4. gl_Position is a vec4.
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 410 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n "
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(1.0, 0.0, 0.0, 1.0);\n "
        "}\n";
    // color vec4 is RGBA by default. 0 is 0 and 1 is 255
    unsigned int shader = CreateShader(vertexShader, fragmentShader); 
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT); // Clears the drawn triangle

        glDrawArrays(GL_TRIANGLES, 0, 3); // Redraws the driangle

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}