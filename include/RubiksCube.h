#pragma once
#include <glm/glm.hpp>
#include <math.h>
#include "Rotation.h"
#include "Cube.h"
#include <Axes.h>
#include <deque>
#define M_PI 3.14159265358979323846f
#define ERROR 0.001f

#define CUBE_DIM 3
#define DT 1.0f
#define ANGLE_SPEED 0.07f

using std::pow;
using std::deque;
using glm::ivec3;

/* Cube Topology to indexes:
   going by layers, from front to back,
   by cols, left to right, 
   by row bottom to top,
*/

class RubiksCube{
    protected:
        // counter for num of wwalls in rotation for each axis
        int num_walls_unaligned[3];
        Cube* cubes[CUBE_DIM * CUBE_DIM * CUBE_DIM];
        Axes localAxes;
        deque<Rotation*> rotations;
        int rotation_offset[3];

        // array for each rotation axis as defined in rotation
        float absolute_rotations[3][CUBE_DIM];

        // total angle of rotation for one press (90 or 45 degrees)
        float currDegree;

        //number of rotation left to do after mix
        int num_mix; 

        vector<ivec3> getIndices(int fixed, int dir);


        int indexFlatten(int row, int columns, int layer) const;
        int indexFlatten(ivec3 coords) const;

        vector<ivec3> getLayerInwards(int layer);
        vector<ivec3> getLayerLeftToRight(int column);
        vector<ivec3> getLayerTopToButtom(int row); 
        
        void rotate_wall(int dir, int layer);
        void rotateIndices(vector<ivec3> indices, int axis, int sign);

    public:
        RubiksCube(): RubiksCube(vec3(0)) {}

        RubiksCube(vec3 pos);

        void update();

        const Cube& getCube(int row, int col, int layer) const;
        
        vector<float> getVBCube(int i);
        vector<float> getVBcubeColorPick(int i);

        vector<int> getIndicesCube(int i);

        glm::mat4 getModelMat(int i);


        // rotate around Axis origin  (origin remains the same)
        void localRotate(float angle, vec3 axis);

        //rotate around (0,0,0) (origin moves too)
        void originRotate(float angle, vec3 axis);

        void translate(vec3 translation);

        void scale(float s);

        void rotate_right_wall();

        void rotate_left_wall();

        void rotate_top_wall();

        void rotate_bottom_wall();

        void rotate_back_wall();

        void rotate_front_wall();
        
        void flipAngle();

        void mulDegree();

        void divDegree();

        void moveCenterX(int x_offset);

        void moveCenterY(int y_offset);

        void moveCenterZ(int z_offset);


        void addMix(int m);

        int getCubeId(int i) {
            return this->cubes[i]->get_id();
        }

        Cube* pickCube(unsigned char * color_picked);
    
        vec3 worldToLocalDir(vec3 dir);
};