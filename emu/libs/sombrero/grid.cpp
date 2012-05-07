#include "grid.h"
#include "application.h"

namespace sombrero {
  Grid::Grid() {
    this->init();
  }

  Grid::Grid(Window *parent) {
    this->init();
    parent->setLayout(this);
  }

  void Grid::init() {
    widgetsTab.resize(1);
    nbColumns = 0;
  }

  void Grid::add(Widget* widget) {
    widgetWrapper *wrapper = new widgetWrapper(widget);
    wrapper->x = widgetsTab[widgetsTab.size()-1].size();
    wrapper->y = widgetsTab.size()-1;
    widgetsTab[widgetsTab.size()-1].push_back(wrapper);
    nbColumns = std::max(nbColumns, widgetsTab[widgetsTab.size()-1].size());
    widget->setParent(this);
    this->update();
  }

  void Grid::newLine() {
    widgetsTab.resize(widgetsTab.size()+1);
  }

  void Grid::draw() {
    //printf("draw grid (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(), this->getWidth(), this->getHeight());
    
  }

  void Grid::update() {
    this->updatePronSize();
    this->updatePronPosition();
    float columnWidth = (float)this->getWidth()/nbColumns;
    float lineHeight  = (float)this->getHeight()/widgetsTab.size();
    for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
      for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
        widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
        if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
          currentWrapper->widget->setWidth (columnWidth * currentWrapper->width);
          currentWrapper->widget->setHeight(lineHeight  * currentWrapper->height);
          currentWrapper->widget->setX (this->getX()+columnWidth*currentWrapper->x);
          currentWrapper->widget->setY (this->getY()+lineHeight*currentWrapper->y);
          currentWrapper->widget->updatePronSize();
          currentWrapper->widget->updatePronPosition();
        }
      }
    }
  }

  bool Grid::find(Widget * w, size_t &x, size_t &y) {
    for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
      for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
        if(widgetsTab[currentY][currentX]->widget == w) {
          x = currentX;
          y = currentY;
          return true;
        }
      }
    }
    return false;
  }

  void Grid::attachNextTo (Widget *child, Widget *sibling, PositionType side, int width, int height) {
    size_t x, y;
    if(this->find(sibling, x, y)) {
      widgetWrapper *wrapper = new widgetWrapper(child);
      wrapper->width  = width;
      wrapper->height = height;

      switch(side) {
       case POS_RIGHT: {
        wrapper->x = x+this->widgetsTab[y][x]->width;
        wrapper->y = y;
        printf("%d, %d\n", wrapper->x, wrapper->y);
        break;
       }
       case POS_BOTTOM: {
        wrapper->x = x;
        wrapper->y = y+this->widgetsTab[y][x]->height;
        break;
       }
       case POS_TOP: {
        wrapper->x = x;
        wrapper->y = y-height;
        if(wrapper->y < 0) {
          size_t yAbs = abs(wrapper->y);
          for(int currentY = widgetsTab.size() - 1; currentY >= 0 ; --currentY) {
            for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
              widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
              if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
                currentWrapper->y += yAbs;
              }
            }
          }
          this->widgetsTab.insert(this->widgetsTab.begin(),yAbs,line_t());
          wrapper->y = 0;
        }
        break;
       }
       case POS_LEFT: {
        wrapper->x = x-width;
        wrapper->y = y;
        if(wrapper->x < 0) {
          size_t xAbs = abs(wrapper->x);
          for(int currentY = widgetsTab.size() - 1; currentY >= 0 ; --currentY) {
            for(int currentX = widgetsTab[currentY].size()-1; currentX >= 0; --currentX) {
              widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
              if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
                currentWrapper->x += xAbs;
              }
            }
          }
          for(size_t currentY = 0; currentY < this->widgetsTab.size(); ++currentY) {
            this->widgetsTab[currentY].insert(this->widgetsTab[currentY].begin(),xAbs,NULL);
          }
          wrapper->x = 0;
        }
        break;
       }
      }
      if(this->widgetsTab.size() < (size_t)wrapper->y + height) {
        this->widgetsTab.resize(wrapper->y + height);
      }
          printf("\n\n");
      for(size_t currentY = wrapper->y; currentY < (size_t)wrapper->y + height; ++currentY) {
          printf("ok1\n");
        if(this->widgetsTab[currentY].size() < (size_t)wrapper->x + width) {
          this->widgetsTab[currentY].resize(wrapper->x + width);
        }
        for(size_t currentX = wrapper->x; currentX < (size_t)wrapper->x + width; ++currentX) {
          printf("ok2\n");
          this->widgetsTab[currentY][currentX] = wrapper;
          // TODO if this->widgetsTab[currentY][currentX] != NULL ?????
        }
          printf("ok3\n");
      }
      nbColumns = max(nbColumns, this->widgetsTab[wrapper->y].size());
      child->setParent(this);
      this->update();
    }// else exception
  }

  Grid::widgetWrapper::widgetWrapper(Widget* w) {
    widget = w;
    width  = 1;
    height = 1;
    x = 1;
    y = 1;
  }
}