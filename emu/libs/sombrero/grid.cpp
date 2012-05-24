#include "grid.h"
#include "application.h"

namespace sombrero {
  
Grid::Grid() {
  this->init();
}

void Grid::init() {
  widgetsTab.resize(1);
  nbColumns = 0;
}

void Grid::add(Widget *widget) {
  widgetWrapper *wrapper = new widgetWrapper(widget);
  wrapper->x = widgetsTab[widgetsTab.size()-1].size();
  wrapper->y = widgetsTab.size()-1;
  widgetsTab[widgetsTab.size()-1].push_back(wrapper);
  nbColumns = std::max(nbColumns, widgetsTab[widgetsTab.size()-1].size());
  if(this->isPronWindowCreated()) {
    widget->setParent(this);
    this->update();
  }
}

void Grid::remove(__attribute__((unused))Widget *widget) {}

void Grid::newLine() {
  widgetsTab.resize(widgetsTab.size()+1);
}

void Grid::draw() {
  //printf("draw grid (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(), this->getWidth(), this->getHeight());
}

void Grid::update() {
  // TODO meilleure gestion des calculs de position pour éviter les espaces entre widgets
  // Sends the new informations to pron
  if(this->isPronWindowCreated()) {
    this->setVisible(false);
    Container::update();
    // Calculates the new colomns width and line height
    float columnWidth = (float)this->getWidth()/nbColumns;
    float lineHeight  = (float)this->getHeight()/widgetsTab.size();
    for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
      for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
        widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
        // We update the widget just once (the same wrapper can be in sevaral positions)
        if(currentWrapper != NULL && currentWrapper->x == (int)currentX
            && currentWrapper->y == (int)currentY) {
          currentWrapper->widget->setWidth (columnWidth * currentWrapper->width);
          currentWrapper->widget->setHeight(lineHeight  * currentWrapper->height);
          currentWrapper->widget->setX (this->getX()+columnWidth*currentWrapper->x);
          currentWrapper->widget->setY (this->getY()+lineHeight*currentWrapper->y);
          // Sends the new informations to pron
          currentWrapper->widget->update();
        }
      }
    }
    this->setVisible(true);
  }
}

void Grid::insertRow (int position) {
  this->insertRows(position, 1);
}

void Grid::insertRows(int position, size_t nb) {
  // TODO voir ce qu'on fait quand la ligne coupe un span
  // Reverse order to avoid to update the same widget twice
  for(int currentY = widgetsTab.size() - 1; currentY >= 0 ; --currentY) {
    for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
        currentWrapper->y += nb;
      }
    }
  }
  this->widgetsTab.insert(this->widgetsTab.begin()+position, nb, line_t());
  this->update();
}

void Grid::insertColumn (int position) {
  this->insertColumns(position, 1);
}

void Grid::insertColumns(int position, size_t nb) {
  // TODO voir ce qu'on fait quand la colonne coupe un span
  // Reverse order to avoid to update the same widget twice
  for(int currentY = widgetsTab.size() - 1; currentY >= 0 ; --currentY) {
    for(int currentX = widgetsTab[currentY].size() - 1; currentX >= 0; --currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL && currentWrapper->x == (int)currentX && currentWrapper->y == (int)currentY) {
        currentWrapper->x += nb;
      }
    }
  }
  for(size_t currentY = 0; currentY < this->widgetsTab.size(); ++currentY) {
    this->widgetsTab[currentY].insert(this->widgetsTab[currentY].begin()+position,nb,NULL);
  }
  nbColumns += nb;
  this->update();
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

void Grid::attach(widgetWrapper *wrapper) {
  if(this->widgetsTab.size() < (size_t)wrapper->y + wrapper->height) {
    this->widgetsTab.resize(wrapper->y + wrapper->height);
  }
  // Adds the wrapper in all the places it spans
  for(size_t currentY = wrapper->y; currentY < (size_t)wrapper->y + wrapper->height; ++currentY) {
    if(this->widgetsTab[currentY].size() < (size_t)wrapper->x + wrapper->width) {
      this->widgetsTab[currentY].resize(wrapper->x + wrapper->width);
    }
    for(size_t currentX = wrapper->x; currentX < (size_t)wrapper->x + wrapper->width; ++currentX) {
      this->widgetsTab[currentY][currentX] = wrapper;
      // TODO if this->widgetsTab[currentY][currentX] != NULL ?????
    }
  }
  nbColumns = max(nbColumns, this->widgetsTab[wrapper->y].size());
  wrapper->widget->setParent(this);
  this->update();
}

void Grid::attach(Widget *child, int x, int y, int width, int height) {
  // Initialises the new wrapper
  widgetWrapper *wrapper = new widgetWrapper(child);
  wrapper->width  = width;
  wrapper->height = height;
  wrapper->x = x;
  wrapper->y = y;
  this->attach(wrapper);
}

void Grid::attachNextTo(Widget *child, Widget *sibling, PositionType side, int width, int height) {
  size_t x, y;
  // If sibling is NULL, it adds at the begining
  if(sibling == NULL) {
    x = 0;
    y = 0;
  }
  if(sibling == NULL || this->find(sibling, x, y)) {
    widgetWrapper *wrapper = new widgetWrapper(child);
    wrapper->width  = width;
    wrapper->height = height;

    // Initialises the wrapper depending to the side
    switch(side) {
      case POS_RIGHT: {
                        wrapper->x = x+this->widgetsTab[y][x]->width;
                        wrapper->y = y;
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
                        this->insertRows(0, -1*wrapper->y);
                        wrapper->y = 0;
                      }
                      break;
                    }
      case POS_LEFT: {
                       wrapper->x = x-width;
                       wrapper->y = y;
                       if(wrapper->x < 0) {
                         this->insertColumns(0, -1*wrapper->x);
                         wrapper->x = 0;
                       }
                       break;
                     }
    }
    this->attach(wrapper);
  }// TODO else exception
}

void Grid::setParent(Widget *parent) {
  printf("Grid::setParent\n");
  Container::setParent(parent);
  for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
    for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL && currentWrapper->x == (int)currentX
          && currentWrapper->y == (int)currentY) {
        currentWrapper->widget->setParent(this);
      }
    }
  }
  this->update();
}

Grid::widgetWrapper::widgetWrapper(Widget* w) {
  widget = w;
  width  = 1;
  height = 1;
  x = 1;
  y = 1;
}

} // namespace sombrero
