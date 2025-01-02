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
   by rows, left to right, top to buttom
*/

class RubiksCube{
    protected:
        Cube cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
        RubiksCube(vec3 pos) : localAxes(Axes(pos)){
            for(Cube& cube : this->cubes){
                cube.setColor(vec3(1, 0 ,0));
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
        int indexFlatten(int row, int columns, int layer){
            return row * CUBE_DIM + columns, layer * pow(CUBE_DIM, 2);
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
};