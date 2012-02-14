#include <window.h>
#include <screen.h>
#include <cstdlib>
#include <string.h>
#include <pronlib_enums.h>

Window::Window(Screen *screen, int id, Window *parent, int x, int y, int width, int height) {
    this->screen = screen;

    this->id = id;

    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    // Père
    this->parent = parent;
    // Fils
    this->firstChild = this->lastChild = NULL;
    // Frères
    if (this->parent != NULL) {
	this->prevSibling = parent->lastChild;
	this->nextSibling = NULL;
    } else {
	this->prevSibling = this->nextSibling = NULL;
    }
    // Maj du père
    if (this->parent != NULL) {
	if (this->parent->lastChild == NULL) {
	    this->parent->firstChild = this->parent->lastChild = this;
	} else {
	    this->parent->lastChild->nextSibling = this;
	    this->parent->lastChild = this;
	}
    }

}

void Window::drawPoint(int x, int y) {
    this->screen->drawPoint(this->x + x, this->y + y);
}

void Window::drawLine(int x1, int y1, int x2, int y2) {
    this->screen->drawLine(this->x + x1, this->y + y1, this->x + x2, this->y + y2);
}

void Window::fillRectangle(int x1, int y1, int x2, int y2) {
    this->screen->fillRectangle(this->x + x1, this->y + y1, this->x + x2, this->y + y2);
}

void Window::clear() {
    for (int h = 0; h < this->height; h++) {
	memset(this->screen->videoBuffer + 3 * ((this->y + h) * this->screen->width + this->x), (this->id >> 16) << 3, this->width * 3);
    }
}

void Window::flush() {
    this->screen->flush();
}

PronWindowAttributes * Window::getAttributes() {
    PronWindowAttributes * attr = new PronWindowAttributes();
    attr->x = this->x;
    attr->y = this->y;
    attr->width = this->width;
    attr->height = this->height;

    return attr;
}

void Window::setAttributes(PronWindowAttributes * newAttr, int mask) {
    if (mask & WIN_ATTR_X) {
	this->x = newAttr->x;
    }
    if (mask & WIN_ATTR_Y) {
	this->y = newAttr->y;
    }
    if (mask & WIN_ATTR_WIDTH) {
	this->width = newAttr->width;
    }
    if (mask & WIN_ATTR_HEIGHT) {
	this->height = newAttr->height;
    }
}
