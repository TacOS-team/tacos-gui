#include "textbox.h"
#include "application.h"
#include "pronlib.h"

namespace sombrero {

  Textbox::Textbox(Container *parent)
    : Widget(parent) {
      this->cursor = 0;
      this->display = 0;
      pron::pronSelectInput(Application::getInstance()->d, this->pronWindow, PRON_EVENTMASK(pron::EV_EXPOSE) | PRON_EVENTMASK(pron::EV_KEY_PRESSED));
    }

  Textbox::~Textbox() {

  }

  void Textbox::handleEventKeyPressed(pron::PronEvent *e) {
    printf("KEY_PRESSED: draw\n");
    pron::EventKeyPressed *keyPressed = (pron::EventKeyPressed*) e;
    this->text.insert(this->cursor, 1, keyPressed->keysym);
    this->cursor++;
    this->draw();
  }

  void Textbox::handleEventExpose() {
    this->draw();
  }

  void Textbox::draw() {
    pronClearWindow(Application::getInstance()->d, this->pronWindow);
    pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0, this->getWidth(), this->getHeight());
    int lines = 0;
    int begin = display;

    string cur;
    int textWidth, textHeight;
    for (unsigned int i = begin; i < cursor; i++) {
      cur = this->text.substr(begin, i - begin + 1);

      pronTextSize(Application::getInstance()->d, Application::getInstance()->d->defaultGC, cur.c_str(), cur.size(), &textWidth, &textHeight);

      if (this->text[i] == '\r') {
	pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, MARGIN_LEFT, MARGIN_TOP + (lines + 1) * textHeight, cur.c_str(), cur.size());
	lines++;
	begin = i + 1;
      } else if (textWidth >= this->getWidth() - MARGIN_RIGHT) {
	pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, MARGIN_LEFT, MARGIN_TOP + (lines + 1) * textHeight, cur.c_str(), cur.size() - 1);
	lines++;
	begin = i;
      }
    }
    cur = this->text.substr(begin, cursor - begin + 1);
    pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, MARGIN_LEFT, MARGIN_TOP + (lines + 1) * textHeight, cur.c_str(), cur.size());
  }

}
