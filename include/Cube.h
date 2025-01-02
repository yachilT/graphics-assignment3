#include <glm/glm.hpp>
#include <vector>
using glm::vec3;
using glm::ivec3;
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
        vec3 vertices[8] = { vec3(-1,-1,-1), vec3(1,-1,-1), vec3(1,1,-1), vec3(-1,1,-1),
                                      vec3(-1,-1,1), vec3(1,-1,1), vec3(1,1,1), vec3(-1,1,1) };
        ivec3 indices[12] = {ivec3(0,1,2), ivec3(2,3,0), 
                                     ivec3(1, 5, 6), ivec3(6, 2 ,1),
                                     ivec3(5, 4, 7), ivec3(7, 6 ,5),
                                     ivec3(4, 0, 3), ivec3(3, 7 ,4),
                                     ivec3(3, 2, 6), ivec3(6, 7 ,3),
                                     ivec3(1, 0, 4), ivec3(4, 5 ,1)};
        vec3 color;
    public:
        Cube() : color(vec3(0)){};
        Cube(vec3 color) : color(color) {};
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
                vb.push_back((vertex.x+1)/2);
                vb.push_back((vertex.y+1)/2);
            }
            return vb;
        };

        vector<int> getIndices() {
            vector<int> indices;
            for(vec3 tri : this->indices){
                //vertex pos
                indices.push_back(tri.x);
                indices.push_back(tri.y);
                indices.push_back(tri.z);
            }
            return indices;
        }

        void setColor(const vec3 &color) {
            this->color = color;
        }
};