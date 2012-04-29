#include <algorithm>
#include <gc.h>

std::vector<GC*> GC::gcs;

GC::GC() {
  this->id = 0;

  // Default foreground color
  COLOR(this->fg, 24).r = 255;
  COLOR(this->fg, 24).g = 255;
  COLOR(this->fg, 24).b = 255;

  // Default background color
  COLOR(this->bg, 24).r = 0;
  COLOR(this->bg, 24).g = 0;
  COLOR(this->bg, 24).b = 0;

  // Default font
  this->font_num = 0;
}

GC::GC(unsigned int id, const PronGCValues &values, unsigned int mask) {
  this->id = id;
  this->setValues(values, mask);
  GC::gcs.push_back(this);
}

GC* GC::getGC(unsigned int id) {
  for (unsigned int i = 0; i < GC::gcs.size(); i++) {
    if (GC::gcs[i]->id == id) {
      return GC::gcs[i];
    }
  }
  return NULL;
}

PronGCValues GC::getValues() {
  PronGCValues values;
  values.fg = this->fg;
  values.bg = this->bg;
  values.font_num = this->font_num;

  return values; 
}

void GC::setValues(const PronGCValues &values, unsigned int mask) {
  if (mask & GC_VAL_FG) {
    this->fg = values.fg;
  }
  if (mask & GC_VAL_BG) {
    this->bg = values.bg;
  }
  if (mask & GC_VAL_FONTNUM) {
    this->font_num = values.font_num;
  }
}

void GC::destroy() {
  GC::gcs.erase(std::find(GC::gcs.begin(), GC::gcs.end(), this));
  delete this;
}
