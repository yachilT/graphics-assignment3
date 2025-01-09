#include <glm/glm.hpp>
#include "Cube.h"
#include <Axes.h>
#include <math.h>

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
        Cube cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
        RubiksCube(vec3 pos) : localAxes(Axes(pos)) {

            for (int row = 0; row < CUBE_DIM; row++) {
                for (int col = 0; col < CUBE_DIM; col++) {
                    for (int layer; layer < CUBE_DIM; layer++) {
                        cubes[indexFlatten(row, col, layer)].setAxes(Axes(vec3(row - CUBE_DIM / 2, col - CUBE_DIM / 2, layer - CUBE_DIM / 2)));
                    }
                }
            }
        };

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
        RubiksCube(): localAxes(vec3(0)) {}
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
};