#ifndef _GC_H_
#define _GC_H_

#include <pronlib_structs.h>
#include <vector>

class GC {
public:
  static std::vector<GC*> gcs;
  unsigned int id;
  color_t bg;
  color_t fg;

  static GC* getGC(unsigned int id);

  GC();

  GC(unsigned int id, const PronGCValues &values, unsigned int mask);

  PronGCValues getValues();

  void setValues(const PronGCValues &values, unsigned int mask);

  void destroy();
};

#endif