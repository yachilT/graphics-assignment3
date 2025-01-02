#include <glm/glm.hpp>
#include <vector>
using glm::vec3;
using std::vector;

/*Cube orientation:
  Vertecies: front than back, anti-clockwise from downleft to upleft ->
             front (negative z): 0:(-1,-1), 1:(1,-1), 2:(1,1), 3:(-1,1)
             back (positive z): 4:(-1,-1), 5:(1,-1), 6:(1,1), 7:(-1,1)
  Indices:  front -> right -> back -> left -> top -> bottom
            for each side, order of triangles (example by front): (0,1,2), (2,3,0)
*/

class Cube{
    protected:
        vec3 vertices[8];
        vec3 indices[12];
        vec3 color;
    public:
        Cube(vec3 color) : color(color), 
                            vertices({vec3(-1,-1,-1), vec3(1,-1,-1), vec3(1,1,-1), vec3(-1,1,-1),
                                      vec3(-1,-1,-1), vec3(1,-1,-1), vec3(1,1,-1), vec3(-1,1,-1)}),
                            indices({vec3(0,1,2), vec3(2,3,0), 
                                     vec3(1, 5, 6), vec3(6, 2 ,1),
                                     vec3(5, 4, 7), vec3(7, 6 ,5),
                                     vec3(4, 0, 3), vec3(3, 7 ,4),
                                     vec3(3, 2, 6), vec3(6, 7 ,3),
                                     vec3(1, 0, 4), vec3(4, 5 ,1)}){};
        vec3 getColor() {  return this->color; };
        
        vector<float> getVB() {
            vector<float> vb;
            for(vec3 vertex : this->vertices){
                //vertex pos
                vb.push_back(vertex.x);
                vb.push_back(vertex.y);
                vb.push_back(vertex.z);
                //vertex color
                vb.push_back(this->color.r);
                vb.push_back(this->color.g);
                vb.push_back(this->color.b);
                //vertex texcoords
                vb.push_back(0.0);
                vb.push_back(0.0);
            }
            return vb;
        };

        vector<float> getIndices() {
            vector<float> indices;
            for(vec3 tri : this->indices){
                //vertex pos
                indices.push_back(tri.x);
                indices.push_back(tri.y);
                indices.push_back(tri.z);
            }
            return indices;
        }
};