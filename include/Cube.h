#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Axes.h"
# include <iostream>
#include <glm/gtx/string_cast.hpp>
#define GLM_ENABLE_EXPERIMENTAL
using glm::vec3;
using glm::ivec3;
using std::vector;

/*Cube orientation:
  Verticies: front than back, anti-clockwise from downleft to upleft ->
             front (negative z): 0:(-1,-1), 1:(1,-1), 2:(1,1), 3:(-1,1)
             back (positive z): 4:(-1,-1), 5:(1,-1), 6:(1,1), 7:(-1,1)
  Indices:  front -> right -> back -> left -> top -> bottom
            for each side, order of triangles (example by front): (0,1,2), (2,3,0)
*/

#define NUM_FACES 6
#define VERTECIES_PER_FACE 4

class Cube{
    protected:
        vec3 vertices[8] = { vec3(-1,-1,-1), vec3(1,-1,-1), vec3(1,1,-1), vec3(-1,1,-1),
                                      vec3(1,-1,1), vec3(-1,-1,1), vec3(-1,1,1), vec3(1,1,1) };
        // ivec3 indices[12] = {ivec3(0,1,2), ivec3(2,3,0), //front
        //                              ivec3(1, 5, 6), ivec3(6, 2 ,1), //right
        //                              ivec3(5, 4, 7), ivec3(7, 6 ,5), //back
        //                              ivec3(4, 0, 3), ivec3(3, 7 ,4), //left
        //                              ivec3(3, 2, 6), ivec3(6, 7 ,3), //top
        //                              ivec3(1, 0, 4), ivec3(4, 5 ,1)}; //bottom
        vec3 color[NUM_FACES + 2]; // remove
        Axes axes;
        int id;


        // returns indices of vertices according to position in vertices array of cube
        vector<int> getFrontVex(){
            vector<int> vex;
            vex.push_back(0);
            vex.push_back(1);
            vex.push_back(2);
            vex.push_back(3);
            return vex;
        }

        vector<int> getRightVex(){
            vector<int> vex;
            vex.push_back(1);
            vex.push_back(4);
            vex.push_back(7);
            vex.push_back(2);
            return vex;
        }
        

        vector<int> getBackVex(){
            vector<int> vex;
            vex.push_back(4);
            vex.push_back(5);
            vex.push_back(6);
            vex.push_back(7);
            return vex;
        }

        vector<int> getTopVex(){
            vector<int> vex;
            vex.push_back(3);
            vex.push_back(2);
            vex.push_back(7);
            vex.push_back(6);
            return vex;
        }

        vector<int> getLeftVex() {
            vector<int> vex;
            vex.push_back(5);
            vex.push_back(0);
            vex.push_back(3);
            vex.push_back(6);
            return vex;
        }

        vector<int> getBottomVex() {
            vector<int> vex;
            vex.push_back(5);
            vex.push_back(0);
            vex.push_back(1);
            vex.push_back(4);
            return vex;
        }

        vector<int> getFace(int i) {
            switch (i)
            {
            case 1:
                return getFrontVex();
            case 2:
                return getRightVex();
            case 3:
                return getBackVex();
            case 4:
                return getLeftVex();
            case 5:
                return getTopVex();
            case 6:
                return getBottomVex();
            default:
                return vector<int>();
            }
        }
    public:
        bool marked = false;
        Cube() : Cube(Axes(vec3(0, 0, 0))){}

        Cube(Axes axes) : axes(axes) {
            this->setDefaultColors();

            // for (int i = 0; i < 8; i++) {
            //     std::cout << "vertex " << i << " " << glm::to_string(vertices[i]) << ": " << glm::to_string(color[i]) << std::endl;
            // }
        }

        Axes &getAxes() {return axes; };
        
        void setAxes(const Axes &axes) { 
            this->axes = axes;

            // std::cout << "o: " << glm::to_string(axes.origin) << std::endl;
        }
        
        void setColor(vec3 c){
            this->color[0] = c;
            this->color[1] = c;
            this->color[2] = c;
            this->color[3] = c;
            this->color[4] = c;
            this->color[5] = c;
        }

        vector<float> getVB() {
            vector<float> vb;
            for (int i = 0; i < NUM_FACES; i++) {
                vector<int> vexIndices = getFace(i+1);
                for (int j = 0; j < vexIndices.size(); j++) {
                    //vertex pos
                    vb.push_back(this->vertices[vexIndices[j]].x);
                    vb.push_back(this->vertices[vexIndices[j]].y);
                    vb.push_back(this->vertices[vexIndices[j]].z);

                    //vertex color
                    vb.push_back(this->color[i].r);
                    vb.push_back(this->color[i].g);
                    vb.push_back(this->color[i].b); 
                    

                    // vb.push_back(this->color[vexIndices[j]].r);
                    // vb.push_back(this->color[vexIndices[j]].g);
                    // vb.push_back(this->color[vexIndices[j]].b);

                    //std::cout << i << ": " << "(" << this->color[i].r << ", " << this->color[i].g << ", " << this->color[i].b << ")" << std::endl;

                    
                    // vertex texture coords
                    switch(j) {
                        case 0:
                            vb.push_back(0.0f);
                            vb.push_back(0.0f);
                            break;
                        case 1:
                            vb.push_back(1.0f);
                            vb.push_back(0.0f);
                            break;
                        case 2:
                            vb.push_back(1.0f);
                            vb.push_back(1.0f);
                            break;
                        case 3:
                            vb.push_back(0.0f);
                            vb.push_back(1.0f); 
                            break;                                                                                 
                    }
                }
            }
            
            return vb;
        };

        vector<float> getColorPickVB() {
            vector<float> vb;

            for (int i = 0; i < NUM_FACES; i++) {
                vector<int> vexIndices = getFace(i+1);
                for (int j = 0; j < vexIndices.size(); j++) {
                    //vertex pos
                    vb.push_back(this->vertices[vexIndices[j]].x);
                    vb.push_back(this->vertices[vexIndices[j]].y);
                    vb.push_back(this->vertices[vexIndices[j]].z);

                    //vertex color
                    vb.push_back(1.0f);
                    vb.push_back(1.0f);
                    vb.push_back(1.0f);

                    
                    // vertex texture coords
                    switch(j) {
                        case 0:
                            vb.push_back(0.0f);
                            vb.push_back(0.0f);
                            break;
                        case 1:
                            vb.push_back(1.0f);
                            vb.push_back(0.0f);
                            break;
                        case 2:
                            vb.push_back(1.0f);
                            vb.push_back(1.0f);
                            break;
                        case 3:
                            vb.push_back(0.0f);
                            vb.push_back(1.0f); 
                            break;                                                                                 
                    }
                }
            }
            
            return vb;
        };

        vector<int> getIndices() {
            vector<int> indices;
            for(int i = 0; i < NUM_FACES; i++){
                //vertex pos
                indices.push_back(0 + i * VERTECIES_PER_FACE);
                indices.push_back(1 + i * VERTECIES_PER_FACE);
                indices.push_back(2 + i * VERTECIES_PER_FACE);

                indices.push_back(2 + i * VERTECIES_PER_FACE);
                indices.push_back(3 + i * VERTECIES_PER_FACE);
                indices.push_back(0 + i * VERTECIES_PER_FACE);
            }
            return indices;
        }

        void setDefaultColors(){
            this->color[0] = vec3(0x00/255.0, 0x9b/255.0, 0x48/255.0);
            this->color[1] = vec3(0xb7/255.0, 0x12/255.0, 0x34/255.0);
            this->color[2] = vec3(0x00/255.0, 0x46/255.0, 0xad/255.0);
            this->color[3] = vec3(0xff/255.0, 0x58/255.0, 0x00/255.0);
            this->color[4] = vec3(0xff/255.0, 0xff/255.0, 0xff/255.0);
            this->color[5] = vec3(0xff/255.0, 0xd5/255.0, 0x00/255.0);
            this->color[6] = vec3(168/255.0, 50/255.0, 127/255.0);
            this->color[7] = vec3(0/255.0, 0/255.0, 0/255.0);
        }

        // rotate around Axis origin  (origin remains the same)
        void localRotate(float angle, vec3 axis) {
            this->axes.localRotate(angle, axis);
        }

        //rotate around (0,0,0) (origin moves too)
        void originRotate(float angle, vec3 axis) {
            this->axes.originRotate(angle, axis);
        }

        void set_id(int id) {
            this->id = id;
        }

        int get_id() {
            return this->id;
        }
};