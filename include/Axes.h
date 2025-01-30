#pragma once
#include <glm/glm.hpp>

using glm::vec3;
using glm::rotate;

class Axes{
    public:

        vec3 origin;
        vec3 right;
        vec3 up;
        vec3 forward;

        Axes(vec3 origin) : origin(origin), right(1, 0, 0), up(0, 1, 0), forward(0, 0, 1) {};
        Axes(vec3 origin, vec3 right, vec3 up, vec3 forward) : origin(origin), right(right), up(up), forward(forward){

        }
        glm::mat4 localToGlobal() {
            return glm::mat4(right.x, right.y, right.z, 0,
                             up.x, up.y, up.z, 0,
                             forward.x, forward.y, forward.z, 0,
                             origin.x, origin.y, origin.z, 1);
            // return glm::mat4(right.x, up.x, forward.x, 0,
            //                  right.y, up.y, forward.y, 0,
            //                  right.z, up.z, forward.z, 0,
            //                  origin.x, origin.y, origin.z, 1);
        };


        // rotate around Axis origin  (origin remains the same)
        void localRotate(float angle, vec3 axis) {
            glm::mat4 id = glm::mat4(1.0f);
            this->right = rotate(id, angle, axis) * glm::vec4(this->right, 1);
            this->up = rotate(id, angle, axis) * glm::vec4(this->up, 1);
            this->forward = rotate(id, angle, axis) * glm::vec4(this->forward, 1);
        }

        //rotate around (0,0,0) (origin moves too)
        void originRotate(float angle, vec3 axis) {
            glm::mat4 rotMat = rotate(glm::mat4(1.0f), angle, axis);
            this->origin = rotMat * glm::vec4(this->origin, 1);
            localRotate(angle, axis);
        }

        void scale(float s) {
            this->forward = this->forward * s;
            this->up = this->up * s;
            this->right = this->right * s;
        }

        void translate(vec3 trans) {
            this->origin += trans;
        }
};