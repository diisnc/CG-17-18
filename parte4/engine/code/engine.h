extern float dx;
extern float dy;
extern float dz;
extern float alfa, beta, radius;
extern float camX, camY, camZ;
extern float rodaI;
extern float time;
extern float p;

extern float up[3];
extern float leftCometa[3];

void spherical2Cartesian();
void renderScene(void);
void changeSize(int w, int h);
