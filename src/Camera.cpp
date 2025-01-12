#include <Camera.h>

void Camera::SetOrthographic(float near, float far)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, near, far);
    m_View = glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
}



void Camera::SetPerspective(float near, float far, float FOV)
{
    m_Near = near;
    m_Far = far;

    // Rest Projection and View matrices
    m_Projection = glm::perspective(FOV, 1.0f, near, far);

    glm::vec3 right = glm::cross(m_Up, m_Orientation);
    // std::cout << "right: " << glm::to_string(right) << std::endl;
    // m_View = glm::mat4(right.x, right.y, right.z, 0.0f,
    //                          m_Up.x, m_Up.y, m_Up.z, 0.0f,
    //                          -m_Orientation.x, -m_Orientation.y, -m_Orientation.z, 0.0f,
    //                          -m_Position.x, -m_Position.y, -m_Position.z, 1);
    m_View = glm::mat4(1.0f);//glm::lookAt(m_Position, m_Position + m_Orientation, m_Up);
    m_View[2][2]=-1.0f;
    std::cout << glm::to_string(m_View) << std::endl;

}

glm::mat4 calcView(glm::vec3 up, glm::vec3 forward, glm::vec3 position) {
    glm::vec3 right = glm::cross(up, forward);
    return glm::mat4(right.x, right.y, right.z, 0.0f,
                             up.x, up.y, up.z, 0.0f,
                             -forward.x, -forward.y, -forward.z, 0.0f,
                             -position.x, -position.y, -position.z, 1);
}

void Camera::addPosition(glm::vec3 dx) {
    m_Position += dx;
    m_View = glm::translate(m_View, dx);
}

void Camera::tiltUp(float dangle) {
    glm::vec3 right = glm::cross(m_Up, m_Orientation);
    m_View = glm::rotate(m_View, dangle, -right);
    
}

/////////////////////
// Input Callbacks //
/////////////////////

void KeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_DOWN:
                std::cout << "UP Pressed" << std::endl;
                // camera->tiltUp(0.01f);
                camera->getCube()->localRotate(0.1f, vec3(-1,0,0));
                break;
            case GLFW_KEY_UP:
                std::cout << "DOWN Pressed" << std::endl;
                //camera->tiltUp(-0.01f);
                camera->getCube()->localRotate(-0.1f, vec3(-1,0,0));
                break;
            case GLFW_KEY_RIGHT:
                std::cout << "LEFT Pressed" << std::endl;
                //camera->addPosition(vec3(-0.1f, 0, 0));
                camera->getCube()->localRotate(-0.1f, vec3(0,1,0));
                break;
            case GLFW_KEY_LEFT:
                std::cout << "RIGHT Pressed" << std::endl;
                //camera->addPosition(vec3(0.1f, 0, 0));
                camera->getCube()->localRotate(0.1f, vec3(0,1,0));
                break;
            case GLFW_KEY_R:
                std::cout << "R Pressed" << std::endl;
                camera->getCube()->rotate_right_wall();
                break;
            case GLFW_KEY_L:
                std::cout << "L Pressed" << std::endl;
                camera->getCube()->rotate_left_wall();
                break;
            case GLFW_KEY_SPACE:
                std::cout << "SPACE Pressed" << std::endl;
                camera->getCube()->flipAngle();
                break;
            case GLFW_KEY_Z:
                std::cout << "Z Pressed" << std::endl;
                camera->getCube()->divDegree();
                break;
            case GLFW_KEY_A:
                std::cout << "A Pressed" << std::endl;
                camera->getCube()->mulDegree();
                break;

            case GLFW_KEY_U:
                std::cout << "U Pressed" << std::endl;
                camera->getCube()->rotate_top_wall();
                break;
            // case GLFW_KEY_D:
            //     std::cout << "D Pressed" << std::endl;
            //     camera->getCube()-rotate_bottom_wall();
            //     break;
            // case GLFW_KEY_B:
            //     std::cout << "B Pressed" << std::endl;
            //     camera->getCube()-rotate_back_wall();
            //     break;
            // case GLFW_KEY_F:
            //     std::cout << "F Pressed" << std::endl;
            //     camera->getCube()-rotate_front_wall();
            //     break;
            default:
                break;
        }
    }
}

void MouseButtonCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        std::cout << "MOUSE LEFT Click" << std::endl;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        std::cout << "MOUSE RIGHT Click" << std::endl;
    }
}

void CursorPosCallback(GLFWwindow* window, double currMouseX, double currMouseY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! KeyCallback is skipped" << std::endl;
        return;
    }

    camera->m_NewMouseX = camera->m_OldMouseX - currMouseX;
    camera->m_NewMouseY = camera->m_OldMouseY - currMouseY;
    camera->m_OldMouseX = currMouseX;
    camera->m_OldMouseY = currMouseY;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {   
        std::cout << "MOUSE LEFT Motion" << std::endl;
        camera->getCube()->localRotate((camera->m_NewMouseX) * ROTATE_ANGLE_SCALE, vec3(0.0f, 1.0f, 0.0f));
        camera->getCube()->localRotate((camera->m_NewMouseY) * ROTATE_ANGLE_SCALE, vec3(1.0f, 0.0f, 0.0f));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        std::cout << "MOUSE RIGHT Motion" << std::endl;
    }
}

void ScrollCallback(GLFWwindow* window, double scrollOffsetX, double scrollOffsetY)
{
    Camera* camera = (Camera*) glfwGetWindowUserPointer(window);
    if (!camera) {
        std::cout << "Warning: Camera wasn't set as the Window User Pointer! ScrollCallback is skipped" << std::endl;
        return;
    }
    if(scrollOffsetY > 0){
        std::cout << "SCROLL UP Motion" << std::endl;
        camera->getCube()->moveZ(-SCROLL_OFFSET);
    }
    else if(scrollOffsetY < 0){
        std::cout << "SCROLL DOWN Motion" << std::endl;
        camera->getCube()->moveZ(SCROLL_OFFSET);
    }
    
}

void Camera::EnableInputs(GLFWwindow* window)
{
    // Set camera as the user pointer for the window
    glfwSetWindowUserPointer(window, this);

    // Handle key inputs
    glfwSetKeyCallback(window, (void(*)(GLFWwindow *, int, int, int, int)) KeyCallback);

    // Handle cursor buttons
    glfwSetMouseButtonCallback(window, (void(*)(GLFWwindow *, int, int, int)) MouseButtonCallback);

    // Handle cursor position and inputs on motion
    glfwSetCursorPosCallback(window , (void(*)(GLFWwindow *, double, double)) CursorPosCallback);

    // Handle scroll inputs
    glfwSetScrollCallback(window, (void(*)(GLFWwindow *, double, double)) ScrollCallback);
}