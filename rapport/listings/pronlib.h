Display* pronConnect();

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height);

Window pronCreateWindowExtended(Display *d, Window parent, const PronWindowAttributes &attr, uint32_t mask);

void pronMapWindow(Display *d, Window w);

void pronUnmapWindow(Display *d, Window w);

void pronSelectInput(Display *d, Window w, uint32_t eventMask);

void pronDontPropagateEvent(Display *d, Window w, uint32_t eventMask);

void pronRaiseWindow(Display *d, Window w);

void pronClearWindow(Display *d, Window w);

void pronMoveWindow(Display *d, Window w, int x, int y);

void pronMoveWindowTo(Display *d, Window w, int x, int y);

void pronResizeWindow(Display *d, Window w, int width, int height);

void pronReparentWindow(Display *d, Window w, Window newParent);  

void pronGetWindowAttributes(Display *d, Window w, PronWindowAttributes *attr);

void pronSetWindowAttributes(Display *d, Window w, const PronWindowAttributes &newAttr, uint32_t mask); 

void pronSetCursor(Display *d, Window w, Pixmap p);

void pronDestroyWindow(Display *d, Window w);

Pixmap pronCreatePixmap(Display *d, int width, int height, int depth);

void pronFreePixmap(Display *d, Pixmap p);

GC pronCreateGC(Display *d, const PronGCValues &values, uint32_t mask);

void pronChangeGC(Display *d, GC gc, const PronGCValues &values, uint32_t mask);

void pronGetGCValues(Display *d, GC gc, PronGCValues *values);

void pronFreeGC(Display *d, GC gc);

void pronDrawPoint(Display *d, Drawable dr, GC gc, int x, int y);

void pronDrawLine(Display *d, Drawable dr, GC gc, int x1, int y1, int x2, int y2);

void pronDrawRect(Display *d, Drawable dr, GC gc, int x, int y, int width, int height);

void pronFillRectangle(Display *d, Drawable dr, GC gc, int x, int y, int width, int height);

void pronDrawEllipse(Display *d, Drawable dr, GC gc, int x0, int y0, int a, int b);

void pronFillEllipse(Display *d, Drawable dr, GC gc, int x0, int y0, int a, int b);

void pronCopyArea(Display *d, Drawable src, Drawable dst, GC gc, int srcX, int srcY, int width, int height, int destX, int destY);

void pronPutImage(Display *d, Drawable dr, GC gc, PronImage *image, int srcX, int srcY, int width, int height, int destX, int destY);  

void pronDrawText(Display *d, Drawable dr, GC gc, int x, int y, const char *text, int length, HPosition hpos = LEFT, VPosition vpos = TOP);

void pronTextSize(Display *d, GC gc, const char *text, int length, int *width, int *height);

PronEvent* getPronEvent();

int pronNextEvent(Display *d, PronEvent *e, bool nonBlocking = false);

void pronDisconnect(Display *d);
