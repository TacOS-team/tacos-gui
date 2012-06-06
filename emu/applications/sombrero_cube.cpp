#include "sombrero.h"
#include "unistd.h"

class Cube : public has_slots<> {
  public:
    Cube(sombrero::Canvas &c) : canvas(c) {
      this->updateZoom(c.getWidth(), c.getHeight());
      memcpy(this->myPoints, points, sizeof(points));
    }

    void draw() {
      this->canvas.clear();

      for (int i = 0; i < 8; i++) {
        this->rotate_point(this->myPoints[i]);
      }

      for (int ligne = 0; ligne < 12; ligne++) {
        this->drawLine(
            this->myPoints[lignes[ligne][0]][0],
            this->myPoints[lignes[ligne][0]][1],
            this->myPoints[lignes[ligne][1]][0],
            this->myPoints[lignes[ligne][1]][1]
        );
      }

      this->canvas.drawEllipse(50, 50, 20, 10);

      this->canvas.fillEllipse(150, 50, 10, 20);

      this->canvas.draw();
    }

  private:
    static float points[8][3];
    static int lignes[12][2];

    sombrero::Canvas &canvas;
    unsigned short int width;
    unsigned short int xShift;
    unsigned short int yShift;
    float myPoints[8][3];

    void drawLine(float x1, float y1, float x2, float y2) {
      this->canvas.drawLine(
          x1 * this->width + this->xShift,
          y1 * this->width + this->yShift,
          x2 * this->width + this->xShift,
          y2 * this->width + this->yShift
      );
    }

    void updateZoom(unsigned short int width, unsigned short int height) {
      this->width = min(width, height) / 4;
      this->xShift = 2 * this->width + (width  - this->width * 4)/2;
      this->yShift = 2 * this->width + (height - this->width * 4)/2;
    }

    void rotate_point(float point[3]) {
      float temp[3];

      // theta = 4°
      float cos_theta = 0.99984769f;
      float sin_theta = 0.01745240f;

      // phi = 1°
      float cos_phi = 0.99756405f;
      float sin_phi = 0.06975647f;

      temp[0] = point[0];
      temp[1] = point[1] * cos_theta - point[2] * sin_theta;
      temp[2] = point[1] * sin_theta + point[2] * cos_theta;

      point[0] = temp[0] * cos_phi - temp[2] * sin_phi;
      point[1] = temp[1];
      point[2] = temp[0] * sin_phi + temp[2] * cos_phi;
    }
};

float Cube::points[8][3] = {
  {  1.0f,  1.0f, -1.0f },
  {  1.0f, -1.0f, -1.0f },
  { -1.0f, -1.0f, -1.0f },
  { -1.0f,  1.0f, -1.0f },
  {  1.0f,  1.0f,  1.0f },
  {  1.0f, -1.0f,  1.0f },
  { -1.0f, -1.0f,  1.0f },
  { -1.0f,  1.0f,  1.0f }
};

int Cube::lignes[12][2] = {
  {0, 1}, // face du haut
  {1, 2},
  {2, 3},
  {3, 0},
  {4, 5}, // face du bas
  {5, 6},
  {6, 7},
  {7, 4},
  {0, 4}, // arêtes des cotés
  {1, 5},
  {2, 6},
  {3, 7}
};

int main() {
  sombrero::Window w("Cube", 0, 0, 200, 200);
  printf("window = %p\n", &w);
  sombrero::Canvas c(200, 200);
  w.add(&c);
  printf("canvas = %p\n", &c);
  Cube cube(c);
  printf("cube = %p\n", &cube);
  Timer t(50);
  t.signal.connect(&cube, &Cube::draw);
  t.start();
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
