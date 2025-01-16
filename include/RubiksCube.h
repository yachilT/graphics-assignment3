#pragma once
#include <glm/glm.hpp>
#include "Cube.h"
#include <Axes.h>
#include <math.h>
#include "Rotation.h"
#include <deque>
#define M_PI 3.14159265358979323846f

#define CUBE_DIM 3
#define DT 1.0f
#define ANGLE_SPEED 0.07f

using std::pow;
using std::deque;

/* Cube Topology to indexes:
   going by layers, from front to back,
   by cols, left to right, 
   by row bottom to top,
*/

class RubiksCube{
    protected:

        int rotatingAxis;
        Cube* cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
        deque<Rotation*> rotations;

        float currDegree;

        vector<int> getIndices(int fixed, int dir){
            assert(0 <= fixed < CUBE_DIM);

            vector<int> indices;
            for (int i = 0; i < CUBE_DIM; i++){
                for(int j = 0; j < CUBE_DIM; j++){
                    switch (dir)
                    {
                    case FORWARD_TO_INWARDS:
                        indices.push_back(indexFlatten(i, j, fixed));
                        break;
                    case LEFT_TO_RIGHT:
                        indices.push_back(indexFlatten(i, fixed, j));
                        break;
                    case BOTTOM_TO_TOP:
                        indices.push_back(indexFlatten(fixed, i, j));
                        break;
                    default:
                        break;
                    }
                }
            }
            return indices;
        }

    public:
        RubiksCube(): RubiksCube(vec3(0)) {}

        RubiksCube(vec3 pos) : localAxes(Axes(pos)) {
            currDegree = M_PI / 2.0f;
            rotatingAxis = -1;
            for (int row = 0; row < CUBE_DIM; row++) {
                for (int col = 0; col < CUBE_DIM; col++) {
                    for (int layer = 0; layer < CUBE_DIM; layer++) {

                        float half = CUBE_DIM / 2.0f - glm::dot(this->localAxes.forward, this->localAxes.forward) / 2.0f;
                        Axes axes(vec3((col - half), (row - half), (layer - half)));
                        axes.scale(.5f);
                        Cube *c = new Cube(axes);
                        cubes[indexFlatten(row, col, layer)] = c;
                    }
                }
            }
        };
        int indexFlatten(int row, int columns, int layer) const{
            return layer * pow(CUBE_DIM, 2) + row * CUBE_DIM + columns;
        }

        const Cube& getCube(int row, int col, int layer) const {
            return *cubes[indexFlatten(row, col, layer)];
        }

        vector<int> getLayerInwards(int layer){
            assert(0 <= layer < CUBE_DIM);
            return getIndices(layer, FORWARD_TO_INWARDS);
        }

        vector<int> getLayerLeftToRight(int column){
            assert(0 <= column < CUBE_DIM);
            return getIndices(column, LEFT_TO_RIGHT);
        }

        vector<int> getLayerTopToButtom(int row){
            assert(0 <= row < CUBE_DIM);
            return getIndices(row, BOTTOM_TO_TOP);
        }

        
        vector<float> getVBCube(int i) {
            return cubes[i]->getVB();
        }

        vector<int> getIndicesCube(int i) {
            return cubes[i]->getIndices();
        }

        glm::mat4 getModelMat(int i) {

            return this->localAxes.localToGlobal() * cubes[i]->getAxes().localToGlobal();
        }

        // rotate around Axis origin  (origin remains the same)
        void localRotate(float angle, vec3 axis) {
            this->localAxes.localRotate(angle, axis);
        }

        //rotate around (0,0,0) (origin moves too)
        void originRotate(float angle, vec3 axis) {
            this->localAxes.originRotate(angle, axis);
        }

        void scale(float s) {
            this->localAxes.scale(s);
        }

        void update() {
            for (int i = rotations.size(); i > 0; i--) {
                Rotation* rot = rotations.at(0);
                rotations.pop_front();

                if (!rot->shouldTerminate()) {
                    float da = rot->update(DT);
                    vector<int> indices;
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

                    
                    for(int i: indices){
                        this->cubes[i]->originRotate(da, rotAxis);
                    }
                    rotations.push_back(rot);
                }
            }

            if (rotations.size() == 0)
                rotatingAxis = -1; 

                
        }
        
        void rotate_wall(int dir, int layer) {
            if (rotatingAxis != -1 && rotatingAxis != dir) return;

            rotatingAxis = dir;
            Rotation * r = new Rotation(dir, layer);
            
            r->startRotation(currDegree, ANGLE_SPEED);
            rotations.push_back(r);
        }

        void rotate_right_wall(){
            rotate_wall(LEFT_TO_RIGHT, CUBE_DIM - 1);
        }

        void rotate_left_wall() {
            rotate_wall(LEFT_TO_RIGHT, 0);
        }

        void rotate_top_wall() {
            rotate_wall(BOTTOM_TO_TOP, CUBE_DIM - 1); 
        }

        void rotate_bottom_wall() {
            rotate_wall(BOTTOM_TO_TOP, 0);
        }

        void flipAngle(){
            this->currDegree = -this->currDegree;
        }

        void mulDegree(){
            if(glm::abs(this->currDegree) < M_PI) this->currDegree = 2.0f * this->currDegree; 
        }

        void divDegree(){
            if(glm::abs(this->currDegree) > M_PI/4.0f) this->currDegree = 0.5f * this->currDegree;
        }

        void moveZ(float z_offset){
            this->localAxes.origin.z += z_offset;
        }
};