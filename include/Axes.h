#pragma once
#include <glm/glm.hpp>

using glm::vec3;

class Axes{
    private:
        vec3 origin;
        vec3 right;
        vec3 up;
        vec3 forward;


    public:
        Axes(vec3 origin) : origin(origin), right(1, 0, 0), up(0, 1, 0), forward(0, 0, 1) {};
        glm::mat4 localToGlobal() {
            return glm::mat4(right.x, right.y, right.z, origin.x,
                             up.x, up.y, up.z, origin.y,
                             forward.x, forward.y, forward.z, origin.z,
                             0, 0, 0, 1);
        };



};