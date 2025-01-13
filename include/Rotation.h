#define FORWARD_TO_INWARDS 0
#define LEFT_TO_RIGHT 1
#define BOTTOM_TO_TOP 2

class Rotation{
    protected:
        float target_angle;
        float curr_degree;
        float angle_speed;
        int dir;
        int dir_id;
        bool finish;


    public:
        Rotation(int dir, int dir_id) : dir(dir), dir_id(dir_id),
         target_angle(0), curr_degree(0), angle_speed(0), finish(true) {}

        void startRotation(float target_angle, float speed){
            this->angle_speed = target_angle > 0 ? speed : -speed;
            this->target_angle = glm::abs(target_angle);
            this->curr_degree = 0;
            this->finish = false;
        }

        float update(float dt){
            float da = this->angle_speed * dt;

            da = glm::min(glm::abs(da), target_angle - glm::abs(curr_degree)) * glm::sign(da);
            
            this->curr_degree += da;
            if (glm::abs(this->curr_degree) >= this->target_angle) {
                    this->curr_degree = 0;
                    this->target_angle = 0;
                    this->finish = true;
            }
            return da;
        }

        int getDir() {
            return dir;
        }

        int getDirID() {
            return dir_id;
        }

        bool shouldTerminate() {
            return finish;
        }
};