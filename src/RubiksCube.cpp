#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "RubiksCube.h"

vector<ivec3> RubiksCube::getIndices(int fixed, int dir) {
    assert(0 <= fixed < CUBE_DIM);

    vector<ivec3> indices;
    for (int i = 0; i < CUBE_DIM; i++){
        for(int j = 0; j < CUBE_DIM; j++){
            switch (dir)
            {
                case FORWARD_TO_INWARDS:
                    indices.push_back(ivec3(i, j, fixed));
                    break;
                case LEFT_TO_RIGHT:
                    indices.push_back(ivec3(i, fixed, j));
                    break;
                case BOTTOM_TO_TOP:
                    indices.push_back(ivec3(fixed, i, j));
                    break;
                default:
                    break;
            }
        }
    }
    return indices;
}

RubiksCube::RubiksCube(vec3 pos) : localAxes(Axes(pos)) {
    currDegree = M_PI / 2.0f;
    for (int row = 0; row < CUBE_DIM; row++) {
        for (int col = 0; col < CUBE_DIM; col++) {
            for (int layer = 0; layer < CUBE_DIM; layer++) {

                float half = CUBE_DIM / 2.0f - glm::dot(this->localAxes.forward, this->localAxes.forward) / 2.0f;
                Axes axes(vec3((col - half), (row - half), (layer - half)));
                axes.scale(.5f);

                std::cout << glm::to_string(glm::ivec3(row, col, layer)) << " == " << glm::to_string(axes.origin) << std::endl;
                Cube *c = new Cube(axes);
                cubes[indexFlatten(row, col, layer)] = c;
            }
        }
    }

    for(int axis = 0; axis < 3; axis++){
        this->num_walls_unaligned[axis] = 0;
        this->rotation_offset[axis] = 0;
        for(int wall = 0; wall < CUBE_DIM; wall++){
            this->absolute_rotations[axis][wall] = 0;
        }
    }
};


int RubiksCube::indexFlatten(int row, int columns, int layer) const{
    return layer * pow(CUBE_DIM, 2) + row * CUBE_DIM + columns;
}

int RubiksCube::indexFlatten(ivec3 coords) const
{
    return indexFlatten(coords.x, coords.y, coords.z);
}

const Cube& RubiksCube::getCube(int row, int col, int layer) const {
    return *cubes[indexFlatten(row, col, layer)];
}

vector<ivec3> RubiksCube::getLayerInwards(int layer){
    assert(0 <= layer < CUBE_DIM);
    return getIndices(layer, FORWARD_TO_INWARDS);
}

vector<ivec3> RubiksCube::getLayerLeftToRight(int column){
    assert(0 <= column < CUBE_DIM);
    return getIndices(column, LEFT_TO_RIGHT);
}

vector<ivec3> RubiksCube::getLayerTopToButtom(int row){
    assert(0 <= row < CUBE_DIM);
    return getIndices(row, BOTTOM_TO_TOP);
}


vector<float> RubiksCube::getVBCube(int i) {
    return cubes[i]->getVB();
}

vector<int> RubiksCube::getIndicesCube(int i) {
    return cubes[i]->getIndices();
}

glm::mat4 RubiksCube::getModelMat(int i) {

    return this->localAxes.localToGlobal() * cubes[i]->getAxes().localToGlobal();
}

// rotate around Axis origin  (origin remains the same)
void RubiksCube::localRotate(float angle, vec3 axis) {
    this->localAxes.localRotate(angle, axis);
}

//rotate around (0,0,0) (origin moves too)
void RubiksCube::originRotate(float angle, vec3 axis) {
    this->localAxes.originRotate(angle, axis);
}

void RubiksCube::scale(float s) {
    this->localAxes.scale(s);
}

void RubiksCube::update() {
    for (int i = rotations.size(); i > 0; i--) {
        Rotation* rot = rotations.at(0);
        rotations.pop_front();

        if (rot->shouldTerminate()) {
            // std::cout << "absolute angle: " << glm::abs(absolute_rotations[rot->getDir()][rot->getDirID()]) << std::endl;
            // std::cout << num_walls_unaligned[0] << ", " << num_walls_unaligned[1] << ", " << num_walls_unaligned[2] << std::endl;
            if (glm::abs(absolute_rotations[rot->getDir()][rot->getDirID()]) <= ERROR)
                this->num_walls_unaligned[rot->getDir()] -= 1;
        }
        else {
            float da = rot->update(DT);
            vector<ivec3> indices;
            vec3 rotAxis(0,0,0);
            switch (rot->getDir()) {
                case FORWARD_TO_INWARDS:
                    indices = getLayerInwards(rot->getDirID());
                    rotAxis = vec3(0,0,1);
                    break;
                case BOTTOM_TO_TOP:
                    indices = getLayerTopToButtom(rot->getDirID());
                    rotAxis = vec3(0,1,0);
                    break;
                case LEFT_TO_RIGHT:
                    indices = getLayerLeftToRight(rot->getDirID());
                    rotAxis = vec3(1,0,0);
                    break;
            }
        
            for(ivec3 coord: indices){
                this->cubes[indexFlatten(coord)]->originRotate(da, rotAxis);
            }


            this->absolute_rotations[rot->getDir()][rot->getDirID()] += da;
            float currAbsRot = this->absolute_rotations[rot->getDir()][rot->getDirID()];
            if (glm::abs(currAbsRot) + ERROR >= M_PI / 2.0f){
                float diff = (glm::abs(currAbsRot) - M_PI / 2.0f);
                this->absolute_rotations[rot->getDir()][rot->getDirID()] = diff < ERROR ? 0 : diff * glm::sign(currAbsRot);
                //rotateIndices(indices, rot->getDir(), glm::sign(da));

            }
            
            rotations.push_back(rot);
        }
    }        
}

glm::imat4x4 castToInt(glm::mat4x4 mat) {
    glm::imat4x4 imat(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            imat[i][j] = static_cast<int>(std::round(mat[i][j]));
        }
    }

    return imat;
}

void RubiksCube::rotateIndices(vector<ivec3> indices, int axis, int sign) {

    glm::imat4x4 swapXY(0, 1, 0, 0,
                        1, 0, 0, 0,
                        0, 0, 1, 0, 
                        0, 0, 0, 1);

    glm::imat4x4 scale(2 - CUBE_DIM % 2);
    glm::imat4x4 scaleBack(-(2 - CUBE_DIM % 2));

    glm::imat4x4 translate(castToInt(glm::translate(glm::mat4(1.0f), -vec3(CUBE_DIM / 2))));
    glm::imat4x4 translateBack(castToInt(glm::translate(glm::mat4(1.0f), vec3(CUBE_DIM / 2))));

    glm::vec3 axisVec = axis == LEFT_TO_RIGHT ? vec3(1, 0, 0) :
    BOTTOM_TO_TOP ? vec3(0, 1, 0) : vec3(0, 0, 1);
    glm::imat4x4 rotate(castToInt(glm::rotate(glm::mat4(1.0f), M_PI / 2 * sign, axisVec)));

    glm::imat4x4 transform = swapXY * scaleBack * translateBack * rotate * translate * scale * swapXY;


    Cube* wall_cubes[indices.size()];

    for (int i = 0; i < indices.size(); i++) {

        glm::ivec4 coords = translate * scale * swapXY * glm::ivec4(indices[i], 1);
        std::cout << glm::to_string(indices[i]) << " == " << glm::to_string(cubes[indexFlatten(indices[i])]->getAxes().origin) << std::endl;

        wall_cubes[i] = cubes[indexFlatten(indices[i])];
    }

    for (int i = 0; i < indices.size(); i++) {
        glm::ivec4 rotated = transform * glm::ivec4(indices[i], 1);
        cubes[indexFlatten(glm::ivec3(rotated.x, rotated.y, rotated.z))] = wall_cubes[i];
    }

}

void RubiksCube::rotate_wall(int dir, int layer) {
    if (this->num_walls_unaligned[(dir - 1) % 3] > 0
     || this->num_walls_unaligned[(dir + 1) % 3] > 0) return;
    
    if (glm::abs(absolute_rotations[dir][layer]) <= ERROR)
        this->num_walls_unaligned[dir] += 1;

    Rotation * r = new Rotation(dir, layer);
    
    r->startRotation(currDegree, ANGLE_SPEED);
    rotations.push_back(r);
}

void RubiksCube::rotate_right_wall(){
    rotate_wall(LEFT_TO_RIGHT, CUBE_DIM - 1);
}

void RubiksCube::rotate_left_wall() {
    rotate_wall(LEFT_TO_RIGHT, 0);
}

void RubiksCube::rotate_top_wall() {
    rotate_wall(BOTTOM_TO_TOP, CUBE_DIM - 1); 
}

void RubiksCube::rotate_bottom_wall() {
    rotate_wall(BOTTOM_TO_TOP, 0);
}

void RubiksCube::rotate_back_wall(){
    rotate_wall(FORWARD_TO_INWARDS, CUBE_DIM-1);
}

void RubiksCube::rotate_front_wall()
{
    rotate_wall(FORWARD_TO_INWARDS, 0);
}
void RubiksCube::flipAngle()
{
    this->currDegree = -this->currDegree;
}

void RubiksCube::mulDegree(){
    if(glm::abs(this->currDegree) < M_PI) this->currDegree = 2.0f * this->currDegree; 
}

void RubiksCube::divDegree(){
    if(glm::abs(this->currDegree) > M_PI/4.0f) this->currDegree = 0.5f * this->currDegree;
}

void RubiksCube::moveZ(float z_offset){
    this->localAxes.origin.z += z_offset;
}
void RubiksCube::moveY(float y_offset){
    this->localAxes.origin.y += y_offset;
}
void RubiksCube::moveX(float x_offset){
    this->localAxes.origin.x += x_offset;
}