#pragma once
#include <glm/glm.hpp>
#include "Cube.h"
#include <Axes.h>
#include <math.h>
#define M_PI 3.14159265358979323846f

#define CUBE_DIM 3
#define FORWARD_TO_INWARDS 0
#define LEFT_TO_RIGHT 1
#define TOP_TO_BOTTOM 2

using std::pow;

/* Cube Topology to indexes:
   going by layers, from front to back,
   by cols, left to right, 
   by row bottom to top,
*/

class RubiksCube{
    protected:

        bool rotating;
        float acc_angle;
        float angle_dx;
        Cube cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
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
                    case TOP_TO_BOTTOM:
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
            this->currDegree = M_PI/2.0;
            acc_angle = 0;
            angle_dx = 0.2f;
            rotating = false;
            for (int row = 0; row < CUBE_DIM; row++) {
                for (int col = 0; col < CUBE_DIM; col++) {
                    for (int layer = 0; layer < CUBE_DIM; layer++) {
                        Axes axes(vec3((col - CUBE_DIM / 2), (row - CUBE_DIM / 2), (layer - CUBE_DIM / 2)));
                        axes.scale(.5f);
                        cubes[indexFlatten(row, col, layer)].setAxes(axes);
                    }
                }
            }
        };
        int indexFlatten(int row, int columns, int layer){
            return layer * pow(CUBE_DIM, 2) + row * CUBE_DIM + columns;
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
            return getIndices(row, TOP_TO_BOTTOM);
        }

        
        vector<float> getVBCube(int i) {
            return cubes[i].getVB();
        }

        vector<int> getIndicesCube(int i) {
            return cubes[i].getIndices();
        }

        glm::mat4 getModelMat(int i) {
            return this->localAxes.localToGlobal() * cubes[i].getAxes().localToGlobal();
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
            if (rotating) {
                vector<int> indices = getLayerLeftToRight(CUBE_DIM - 1);
                vec3 rot_axis = vec3(1,0,0);
                for(int i: indices){
                    this->cubes[i].originRotate(glm::min(this->angle_dx, currDegree - acc_angle), rot_axis);
                }
                acc_angle += angle_dx;
                if (acc_angle >= currDegree) {
                    rotating = false;
                    acc_angle = 0;
                }
            }
        }

        void rotate_right_wall(){
            if (!rotating) {
                rotating = true;
            }
        }
};