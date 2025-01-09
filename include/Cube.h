#include <glm/glm.hpp>
#include <vector>
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

class Cube{
    protected:
        vec3 vertices[8] = { vec3(-1,-1,-1), vec3(1,-1,-1), vec3(1,1,-1), vec3(-1,1,-1),
                                      vec3(-1,-1,1), vec3(1,-1,1), vec3(1,1,1), vec3(-1,1,1) };
        // ivec3 indices[12] = {ivec3(0,1,2), ivec3(2,3,0), //front
        //                              ivec3(1, 5, 6), ivec3(6, 2 ,1), //right
        //                              ivec3(5, 4, 7), ivec3(7, 6 ,5), //back
        //                              ivec3(4, 0, 3), ivec3(3, 7 ,4), //left
        //                              ivec3(3, 2, 6), ivec3(6, 7 ,3), //top
        //                              ivec3(1, 0, 4), ivec3(4, 5 ,1)}; //bottom
        vec3 color;
        //Front side
        vector<int> getFrontVex(){
            vector<int> vex;
            vex.push_back(0);
            vex.push_back(1);
            vex.push_back(2);
            vex.push_back(3);
            return vex;
        }

        //Right side
        vector<int> getRightVex(){
            vector<int> vex;
            vex.push_back(1);
            vex.push_back(5);
            vex.push_back(6);
            vex.push_back(2);
            return vex;
        }
        

        //Back side
        vector<int> getBackVex(){
            vector<int> vex;
            vex.push_back(5);
            vex.push_back(4);
            vex.push_back(7);
            vex.push_back(6);
            return vex;
        }

        //Top side
        vector<int> getTopVex(){
            vector<int> vex;
            vex.push_back(3);
            vex.push_back(2);
            vex.push_back(6);
            vex.push_back(7);
            return vex;
        }

        // left side
        vector<int> getLeftVex() {
            vector<int> vex;
            vex.push_back(4);
            vex.push_back(0);
            vex.push_back(3);
            vex.push_back(7);
            return vex;
        }

        vector<int> getBottomVex() {
            vector<int> vex;
            vex.push_back(4);
            vex.push_back(0);
            vex.push_back(1);
            vex.push_back(5);
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
        Cube() : color(vec3(0)){};
        Cube(vec3 color) : color(color) {};
        vec3 getColor() {  return this->color; };
        
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
                    vb.push_back(this->color.r);
                    vb.push_back(this->color.g);
                    vb.push_back(this->color.b);

                    
                    // vertex texture coords

                    switch(j) {
                        case 0:
                            vb.push_back(0.0f);
                            vb.push_back(0.0f);
                            break;
                        case 1:
                            vb.push_back(0.0f);
                            vb.push_back(1.0f);
                            break;
                        case 2:
                            vb.push_back(1.0f);
                            vb.push_back(1.0f);
                            break;
                        case 3:
                            vb.push_back(1.0f);
                            vb.push_back(0.0f); 
                            break;                                                                                 
                    }
                }
            }
            
            return vb;
        };

        vector<int> getIndices() {
            vector<int> indices;
            vector<int> currSide;
            for(int i = 0; i < NUM_FACES; i++){
                //vertex pos
                currSide = getFace(i+1);
                indices.push_back(0 + i * currSide.size());
                indices.push_back(1 + i * currSide.size());
                indices.push_back(2 + i * currSide.size());

                indices.push_back(2 + i * currSide.size());
                indices.push_back(3 + i * currSide.size());
                indices.push_back(0 + i * currSide.size());
            }
            return indices;
        }

        void setColor(const vec3 &color) {
            this->color = color;
        }
};