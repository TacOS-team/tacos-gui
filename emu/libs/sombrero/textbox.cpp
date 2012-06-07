#include "textbox.h"
#include "application.h"
#include "pronlib.h"

namespace sombrero {

  Textbox::Textbox(bool multiLine) {
    this->cursor = 0;
    this->multiLine = multiLine;
    this->subscribeEvent(pron::EV_KEY_PRESSED);
  }

  Textbox::~Textbox() {}

  void Textbox::handleEventKeyPressed(pron::EventKeyPressed *keyPressed) {
    if (keyPressed->ch != 0) {
      if (keyPressed->ch == '\r' && !this->multiLine) {
        this->submitted();
        return;
      }
      
      this->text.insert(this->cursor, 1, keyPressed->ch);
      this->cursor++;
      this->draw();
    }
  }

  void Textbox::handleEventExpose() {
    this->draw();
  }
      
  string Textbox::getText() {
    return this->text;
  }

  void Textbox::draw() {
    // Clear all
    pronClearWindow(Application::getInstance()->d, this->pronWindow);
    pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0, this->getWidth(), this->getHeight());
    
    int textWidth, textHeight;
    int lines = 0;

    int begin = 0;
    string cur;

    for (unsigned int i = begin; i < cursor; i++) {
      // Get substring from last displayed character to current character
      cur = this->text.substr(begin, i - begin + 1);

      // Get size of the text
      pronTextSize(Application::getInstance()->d, Application::getInstance()->d->defaultGC, cur.c_str(), cur.size(), &textWidth, &textHeight);

      if (this->text[i] == '\r' || i == cursor - 1) {
        // Draw line if we have a line feed or we reached the end of the text...
        pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, MARGIN_LEFT, MARGIN_TOP + lines * textHeight, cur.c_str(), cur.size());
        if (this->multiLine) {
          lines++;
          begin = i + 1;
        } else {
          break;
        }
      } else if (textWidth >= this->getWidth() - MARGIN_RIGHT) {
        // ... or if we must break the line
        pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, MARGIN_LEFT, MARGIN_TOP + lines * textHeight, cur.c_str(), cur.size() - 1);
        if (this->multiLine) {
          lines++;
          begin = i;
        } else {
          break;
        }
      }
    }
  }
}
