#define GLM_ENABLE_EXPERIMENTAL 1
#define SCALE 2.0f
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Debugger.h>
#include <VertexBuffer.h>
#include <VertexBufferLayout.h>
#include <IndexBuffer.h>
#include <VertexArray.h>
#include <Shader.h>
#include <Texture.h>
#include <Camera.h>

#include <iostream>
#include <RubiksCube.h>

/* Window size */
const unsigned int width = 800;
const unsigned int height = 800;


// const float FOVdegree = 45.0f;  // Field Of View Angle
const float near = 0.1f;
const float far = 100.0f;

/* Shape vertices coordinates with positions, colors, and corrected texCoords */
float vertices[] = {
    // positions            // colors            // texCoords
    -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f,  // Bottom-left
     0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,  // Bottom-right
     0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f,  // Top-right
    -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, 0.0f,    0.0f, 1.0f,  // Top-left
};


/* Indices for vertices order */
unsigned int indices[] = {
    0, 1, 2, 
    2, 3, 0
};

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }




    /* Set OpenGL to Version 3.3.0 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }



    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load GLAD so it configures OpenGL */
    gladLoadGL();


    /* Control frame rate */
    glfwSwapInterval(1);






    /* Print OpenGL version after completing initialization */
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    /* Set scope so that on window close the destructors will be called automatically */
    {

        /* Blend to fix images with transperancy */
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        /* Create texture */
        Texture texture("res/textures/plane.png");
        texture.Bind();
        


        /* Create shaders */
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        /* Enables the Depth Buffer */
    	GLCall(glEnable(GL_DEPTH_TEST));

        RubiksCube cube = RubiksCube(vec3(0, 0, 5));
        //Cube cube(vec3(0, 0, 5));


        /* Create camera */
        Camera camera(width, height, &cube);
        camera.SetPerspective(near, far, M_PI / 4);
        //camera.SetOrthographic(near, far);
        camera.EnableInputs(window);
        float count = 0.1f;
        cube.localRotate(count, vec3(1, 1, 0));




        //std::cout << "ours: " << glm::to_string(cube.getModelMat(0)) << std::endl;

        cube.scale(SCALE / CUBE_DIM);
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Set white background color */
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));


            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            glm::vec4 color = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);

            cube.update();
            //cube.localRotate(count, vec3(1, 1, 0));
            for (int i = 0; i < CUBE_DIM * CUBE_DIM * CUBE_DIM; i++) {
                //if (i == 0) {
                    VertexArray va;

                    vector<float>cubeVertices = cube.getVBCube(i);
                    vector<int>trias = cube.getIndicesCube(i);

                    float vs[cubeVertices.size()];
                    std::copy(cubeVertices.begin(), cubeVertices.end(), vs);

                    // for (int i = 0; i < 24; i++) {
                    //     std::cout << i <<  ": (" << vs[8 * i] << ", " << vs[8 * i + 1] << ", " << vs[8 * i + 2] << ") (" << vs[8 * i + 3] << ", " << vs[8 * i + 4] << ", " << vs[8 * i + 5] << ") (" << vs[8 * i + 6] << ", " << vs[8 * i + 7] << ")" << std::endl;
                    // }



                    //std::cout << "-----------------------------------------" << std::endl ;
                    
                    unsigned int indices2[trias.size()];
                    
                    std::copy(trias.begin(), trias.end(), indices2);

                    // for (int i = 0; i < 6; i++) {
                    //     std::cout << trias[6 * i] << " -> " << trias[6*i + 1] << " -> " << trias[6*i + 2] << " | " << trias[6 * i + 3] << " -> " << trias[6*i + 4] << " -> " << trias[6*i + 5] << std::endl;
                    // }
                    VertexBuffer vb(vs, sizeof(vs));
                    IndexBuffer ib(indices2, sizeof(indices2));

                    VertexBufferLayout layout;
                    layout.Push<float>(3);  // positions
                    layout.Push<float>(3);  // colors
                    layout.Push<float>(2);  // texCoords
                    va.AddBuffer(vb, layout);

                    
                    glm::mat4 model = cube.getModelMat(i);
                    //std::cout << glm::to_string(model) << std::endl;
                    glm::mat4 view = camera.GetViewMatrix();
                    glm::mat4 proj = camera.GetProjectionMatrix();
                    glm::mat4 mvp = proj * view * model;


                    /* Update shaders paramters and draw to the screen */
                    shader.Bind();
                    shader.SetUniform4f("u_Color", color);
                    shader.SetUniformMat4f("u_MVP", mvp);
                    shader.SetUniform1i("u_Texture", 0);
                    va.Bind();
                    ib.Bind();
                    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
                    va.Unbind();
                    ib.Unbind();
                //}



            }


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}