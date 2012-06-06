#include <vector>
#include "sombrero.h"
#define PI 3.14159f
#define WIDTH 320
#define HEIGHT 320

using namespace std;
using namespace sombrero;

float my_cos(float angle) {
	float angle_radians = angle * (2 * PI)/360;
	float result;
	
	asm("fcos" : "=t" (result) : "0" (angle_radians));
	
	return result;
}

float my_sin(float angle) {
	float angle_radians = angle * (2 * PI)/360;
	float result;
	
	asm("fsin" : "=t" (result) : "0" (angle_radians));
	
	return result;
}

class Clock : public has_slots<> {
 public:
  Clock(Canvas &canvas) : center(WIDTH / 2, HEIGHT / 2), zero(0, -(HEIGHT / 2.2)), canvas(canvas) {
    aiguilleH.push_back(Point(0, HEIGHT / 25.));
    aiguilleH.push_back(Point(-(HEIGHT / 25.), -0));
    aiguilleH.push_back(Point(0, HEIGHT / 25.));
    aiguilleH.push_back(Point((HEIGHT / 25.), 0));
    aiguilleH.push_back(Point(0, -(HEIGHT / 3.3)));
    aiguilleH.push_back(Point(-(HEIGHT / 25.), -0));
    aiguilleH.push_back(Point(0, -(HEIGHT / 3.3)));
    aiguilleH.push_back(Point(HEIGHT / 25., 0));

    aiguilleM.push_back(Point(0, HEIGHT / 20.));
    aiguilleM.push_back(Point(-(HEIGHT / 40.), 0));
    aiguilleM.push_back(Point(0, HEIGHT / 20.));
    aiguilleM.push_back(Point((HEIGHT / 40.), 0));
    aiguilleM.push_back(Point(0, -(HEIGHT / 2.5)));
    aiguilleM.push_back(Point(-(HEIGHT / 40.), 0));
    aiguilleM.push_back(Point(0, -(HEIGHT / 2.5)));
    aiguilleM.push_back(Point(HEIGHT / 40., 0));

    aiguilleS.push_back(Point(0, 0));
    aiguilleS.push_back(Point(0, -(HEIGHT / 2.5)));
  }
  
  void draw() {
    Color c(255, 77, 182);
    Point p = zero;
    struct tm *date;
    time_t timer;
    
    this->canvas.clear();

    for (int minute = 0; minute < 60; minute++) {
      p = this->rotate(p, (float) 360 / 60);
      this->drawPoint(p, c);
    }

    for (int heure = 0; heure < 12; heure++) {
      p = this->rotate(p, (float) 360 / 12);
      this->fillRect(p - Point(1, 1), p + Point(1, 1), c);
    }

    // Extraction heures, minutes, secondes
    timer = time(NULL);
		date = gmtime(&timer);
    int heures = date->tm_hour;
    int minutes = date->tm_min;
    int secondes = date->tm_sec;
    
    // Heures
    float angleHeures = ((heures % 12) + ((float) minutes / 60)) * ((float) 360 / 12);
    this->drawAiguille(aiguilleH, angleHeures);
    
    // Minutes
    float angleMinutes = minutes * ((float) 360 / 60);
    this->drawAiguille(aiguilleM, angleMinutes);
    
    // Secondes
    float angleSecondes = secondes * ((float) 360 / 60);
    this->drawAiguille(aiguilleS, angleSecondes);

    this->canvas.draw();
  }

 private:
  class Point {
   public:
    float x;
    float y;
    
    Point(float x, float y) : x(x), y(y) {};

    Point operator+(const Point &p) const {
      return Point(x + p.x, y + p.y);
    }
    
    Point operator-(const Point &p) const {
      return Point(x - p.x, y - p.y);
    }
  };

  vector<Point> aiguilleH, aiguilleM, aiguilleS;
  Point center, zero;
  Canvas &canvas;

  Point rotate(Point p, float theta) {
    return Point(my_cos(theta) * p.x - my_sin(theta) * p.y,
        my_sin(theta) * p.x + my_cos(theta) * p.y);
  }

  void drawPoint(Point p, Color c) {
    this->canvas.setFGColor(c);
    this->canvas.drawPoint(center.x + p.x, center.y + p.y);
  }

  void drawLine(Point p1, Point p2, Color c) {
    this->canvas.setFGColor(c);
    this->canvas.drawLine(center.x + p1.x, center.y + p1.y, center.x + p2.x, center.y + p2.y);
  }

  void fillRect(Point p1, Point p2, Color c) {
    this->canvas.setFGColor(c);
    this->canvas.fillRectangle(center.x + p1.x, center.y + p1.y, p2.x - p1.x + 1, p2.y - p1.y + 1);
  }

  void drawAiguille(vector<Point> aiguille, float angle) {
    Color c(255, 77, 182);
    vector<Point> tmp = aiguille;

    for (unsigned int point = 0; point < aiguille.size(); point++) {
      aiguille[point] = rotate(aiguille[point], angle);
      if (point % 2 == 1) {
        this->drawLine(aiguille[point - 1], aiguille[point], c);
      }
    }
  }
};

int main() {
  sombrero::Window w("Whoohoo clock trop bien", 0, 0, WIDTH, HEIGHT);
  printf("window = %p\n", &w);
  sombrero::Canvas c(WIDTH, HEIGHT);
  w.add(&c);
  printf("canvas = %p\n", &c);
  Clock clock(c);
  printf("clock = %p\n", &clock);
  Timer t(250);
  t.signal.connect(&clock, &Clock::draw);
  t.start();
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
