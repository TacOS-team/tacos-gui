#include "grid.h"
#include "application.h"

namespace sombrero {
  
Grid::Grid() {
  this->init();
}

void Grid::init() {
  widgetsTab.resize(1);
  this->nbColumns = 0;
}

void Grid::add(Widget *widget) {
  widgetWrapper *wrapper = new widgetWrapper(widget);
  wrapper->x = widgetsTab[widgetsTab.size()-1].size();
  wrapper->y = widgetsTab.size()-1;
  widgetsTab[widgetsTab.size()-1].push_back(wrapper);
  this->nbColumns = std::max(nbColumns, widgetsTab[widgetsTab.size()-1].size());
  widget->setParent(this);
  this->update();
}

void Grid::removeRows(int position, int nb) {
  // deletes the wrappers
  line_t wrappers = this->getWrappers();
  for(line_t::iterator it = wrappers.begin(); it < wrappers.end(); ++it) {
    widgetWrapper *currentWrapper = *it;
    if(currentWrapper->y >= position) {
      if(currentWrapper->y < position+nb) {
        currentWrapper->height -= position+nb - currentWrapper->y;
        currentWrapper->y      -= currentWrapper->y - position;
        if(currentWrapper->height < 1) {
          delete currentWrapper;
        }
      } else {
        currentWrapper->y -= nb;
      }
    } else if(currentWrapper->y + currentWrapper->height >= position) {
      currentWrapper->height -= std::min(currentWrapper->y + currentWrapper->height - position,
                                         nb);
    }
  }
  widgetsTab.erase(widgetsTab.begin()+position, widgetsTab.begin()+position+nb);
  this->update();
}

void Grid::removeColumns(int position, int nb) {
  // deletes the wrappers
  line_t wrappers = this->getWrappers();
  for(line_t::iterator it = wrappers.begin(); it < wrappers.end(); ++it) {
    widgetWrapper *currentWrapper = *it;
    if(currentWrapper->x >= position) {
      if(currentWrapper->x < position+nb) {
        currentWrapper->width -= position+nb - currentWrapper->x;
        currentWrapper->x      -= currentWrapper->x - position;
        if(currentWrapper->width < 1) {
          delete currentWrapper;
        }
      } else {
        currentWrapper->x -= nb;
      }
    } else if(currentWrapper->x + currentWrapper->width >= position) {
      currentWrapper->width -= std::min(currentWrapper->x + currentWrapper->width - position,
                                         nb);
    }
  }
  this->nbColumns = 0;
  for(column_t::iterator it = widgetsTab.begin(); it != widgetsTab.end(); ++it) {
    it->erase(it->begin()+position, it->begin()+position+nb);
    this->nbColumns = std::max(this->nbColumns, it->size());
  }
  this->update();
}

void Grid::cleanTab() {
  // looks for empty rows
  bool empty;
  for(size_t currentY = 0; currentY < widgetsTab.size(); ) {
    empty = true;
    for(line_t::iterator it = widgetsTab[currentY].begin(); empty && it != widgetsTab[currentY].end(); ++it) {
      if(*it != NULL) {
        empty = false;
      }
    }
    if(empty) {
      this->removeRows(currentY,1);
    } else {
      ++currentY;
    }
  }
  for(size_t currentX = 0; currentX < this->nbColumns; ) {
    empty = true;
    for(size_t currentY = 0; empty && currentY < widgetsTab.size(); ++currentY) {
      if(widgetsTab[currentY].size() > currentX && widgetsTab[currentY][currentX] != NULL) {
        empty = false;
      }
    }
    if(empty) {
      this->removeColumns(currentX,1);
    } else {
      ++currentX;
    }
  }
}

void Grid::remove(__attribute__((unused))Widget *widget) {
  // Deletes the wrappers
  bool deleteEnd;
  widgetWrapper *wrapperToDelete = NULL;
  for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
    for(int currentX = widgetsTab[currentY].size()-1; currentX >= 0; --currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL) {
        if(currentWrapper->widget == widget) {
          widgetsTab[currentY][currentX] = NULL;
          wrapperToDelete = currentWrapper;
        } else {
          deleteEnd = false;
        }
      }
      // If the widget (or NULL) was at the end of the vector, we resize the line vector
      if(deleteEnd) {
        widgetsTab[currentY].pop_back();
      }
    }
  }
  if(wrapperToDelete != NULL) {
    delete wrapperToDelete;
    this->nbColumns = 0;
    // Update the nbColumns var
    for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
      this->nbColumns = std::max(this->nbColumns, widgetsTab[currentY].size());
    }
  }// @todo else exception
  this->cleanTab();
  this->update();
}

void Grid::newLine() {
  widgetsTab.resize(widgetsTab.size()+1);
}

void Grid::draw() {
  //printf("draw grid (x, y, w, h) : %d, %d, %d, %d, %p\n", this->getX(), this->getY(), this->getWidth(), this->getHeight(), this);
}

void Grid::execUpdate() {
  // TODO meilleure gestion des calculs de position pour éviter les espaces entre widgets
  // Sends the new informations to pron
  Container::execUpdate();
  // Calculates the new colomns width and line height
  float columnWidth = (float)this->getWidth()/nbColumns;
  float lineHeight  = (float)this->getHeight()/widgetsTab.size();
  std::vector<widgetWrapper*> wrappers = this->getWrappers();
  for(std::vector<widgetWrapper*>::iterator it = wrappers.begin(); it != wrappers.end(); ++it) {
    (*it)->widget->setWidth (columnWidth * (*it)->width);
    (*it)->widget->setHeight(lineHeight  * (*it)->height);
    (*it)->widget->setX (columnWidth*(*it)->x);
    (*it)->widget->setY (lineHeight*(*it)->y);
    // Sends the new informations to pron
    (*it)->widget->update();
  }
}

void Grid::insertRow (int position) {
  this->insertRows(position, 1);
}

void Grid::insertRows(int position, size_t nb) {
  // TODO voir ce qu'on fait quand la ligne coupe un span
  // Reverse order to avoid to update the same widget twice
  for(int currentY = widgetsTab.size() - 1; currentY >= position ; --currentY) {
    for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL && currentWrapper->x == (int)currentX
                                && currentWrapper->y == (int)currentY) {
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
    for(int currentX = widgetsTab[currentY].size() - 1; currentX >= position; --currentX) {
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
  //printf("Grid::setParent\n");
  Container::setParent(parent);
  this->update();
}

Grid::line_t Grid::getWrappers() {
  line_t res;
  for(size_t currentY = 0; currentY < widgetsTab.size(); ++currentY) {
    for(size_t currentX = 0; currentX < widgetsTab[currentY].size(); ++currentX) {
      widgetWrapper *currentWrapper = widgetsTab[currentY][currentX];
      // We update the widget just once (the same wrapper can be in sevaral positions)
      if(currentWrapper != NULL && currentWrapper->x == (int)currentX
          && currentWrapper->y == (int)currentY) {
        res.push_back(currentWrapper);
      }
    }
  }
  return res;
}

std::vector<Widget*> Grid::getChildren() {
  std::vector<Widget*> res;
  line_t wrappers = this->getWrappers();
  for(line_t::iterator it = wrappers.begin(); it != wrappers.end(); ++it) {
    res.push_back((*it)->widget);
  }
  return res;
}

Grid::widgetWrapper::widgetWrapper(Widget* w) {
  widget = w;
  width  = 1;
  height = 1;
  x = 1;
  y = 1;
}

} // namespace sombrero
