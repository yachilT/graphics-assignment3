#include <glm/glm.hpp>
#include "Cube.h"
#include <Axes.h>

#define CUBE_DIM 3

/* Cube Topology to indexes:
   going by layers, from front to back,
   by rows, from top to buttom, left to right
*/

class RubiksCube{
    protected:
        Cube cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
    public:
        vector<int> getFrontWall(){
            vector<int> cubes;

            cubes.push_back(0);
            cubes.push_back(1);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }

        vector<int> getFrontWall(){
            vector<int> cubes;
            
            cubes.push_back(9);
            cubes.push_back(7);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }

        vector<int> getFrontWall(){
            vector<int> cubes;
            
            cubes.push_back(0);
            cubes.push_back(1);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }

        vector<int> getFrontWall(){
            vector<int> cubes;
            
            cubes.push_back(0);
            cubes.push_back(1);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }

        vector<int> getFrontWall(){
            vector<int> cubes;
            
            cubes.push_back(0);
            cubes.push_back(1);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }
        
        vector<int> getFrontWall(){
            vector<int> cubes;
            
            cubes.push_back(0);
            cubes.push_back(1);
            cubes.push_back(2);

            cubes.push_back(3);
            cubes.push_back(4);
            cubes.push_back(5);
            
            cubes.push_back(6);
            cubes.push_back(7);
            cubes.push_back(8);

            return cubes;
        }
};