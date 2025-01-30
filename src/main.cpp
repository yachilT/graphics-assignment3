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
        cube.scale(SCALE / CUBE_DIM);
        float count = 0.1f;
        cube.localRotate(count, vec3(1, 1, 0));


        /* Create camera */
        Camera camera(width, height, &cube);
        camera.SetPerspective(near, far, M_PI / 4);
        camera.EnableInputs(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if(!camera.picked){
                /* Set white background color */
                GLCall(glClearColor(25 / 255.0f, 25 / 255.0f, 25 / 255.0f, 1.0f));


                /* Render here */
                GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
                glm::vec4 color = glm::vec4(1.0, 1.0f, 1.0f, 1.0f);

                cube.update();

                for (int i = 0; i < CUBE_DIM * CUBE_DIM * CUBE_DIM; i++) {

                    vector<float>cubeVertices = cube.getVBCube(i);
                    vector<int>trias = cube.getIndicesCube(i);

                    float vs[cubeVertices.size()];
                    std::copy(cubeVertices.begin(), cubeVertices.end(), vs);
                    
                    unsigned int indices2[trias.size()];
                    
                    std::copy(trias.begin(), trias.end(), indices2);

                    VertexBuffer vb(vs, sizeof(vs));
                    IndexBuffer ib(indices2, sizeof(indices2));

                    VertexBufferLayout layout;
                    layout.Push<float>(3);  // positions
                    layout.Push<float>(3);  // colors
                    layout.Push<float>(2);  // texCoords

                    VertexArray va;
                    va.AddBuffer(vb, layout);

                    
                    glm::mat4 model = cube.getModelMat(i);
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
                }


                /* Swap front and back buffers */
                glfwSwapBuffers(window);

                /* Poll for and process events */
                glfwPollEvents();
            }
            else{
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glGetIntegerv(GL_VIEWPORT, viewport);
                glReadPixels(x, viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zz)
            }
        }
    }

    glfwTerminate();
    return 0;
}