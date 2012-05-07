#include "grid.h"
#include "application.h"

namespace sombrero {
  Grid::Grid(Container *parent) : Container(parent) {
    widgetsTab.resize(1);
    nbColumns = 0;
    this->setWidth(parent->getWidth());
    this->setHeight(parent->getHeight());
    this->setX(parent->getX());
    this->setY(parent->getY());
  }

  void Grid::add(Widget* widget) {
    widgetWrapper *wrapper = new widgetWrapper(widget);
    wrapper->x = widgetsTab[widgetsTab.size()-1].size();
    wrapper->y = widgetsTab.size()-1;
    widgetsTab[widgetsTab.size()-1].push_back(wrapper);
    nbColumns = std::max((int)nbColumns, (int)widgetsTab[widgetsTab.size()-1].size());
    this->update();
  }

  void Grid::newLine() {
    widgetsTab.resize(widgetsTab.size()+1);
  }

  void Grid::draw() {
    //printf("draw grid (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(), this->getWidth(), this->getHeight());
    
  }

  void Grid::update() {
    unsigned short columnWidth = this->getWidth()/nbColumns;
    unsigned short lineHeight  = this->getHeight()/widgetsTab.size();
    for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
      for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
        widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
        if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
          currentWrapper->widget->setWidth (columnWidth * currentWrapper->width);
          currentWrapper->widget->setHeight(lineHeight  * currentWrapper->height);
          currentWrapper->widget->setX (this->getX()+columnWidth*currentWrapper->x);
          currentWrapper->widget->setY (this->getY()+lineHeight*currentWrapper->y);
        }
      }
    }
  }

  Grid::widgetWrapper::widgetWrapper(Widget* w) {
    widget = w;
    width  = 1;
    height = 1;
    x = 1;
    y = 1;
  }
}