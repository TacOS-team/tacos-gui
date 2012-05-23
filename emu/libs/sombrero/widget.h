#ifndef __WIDGET_H__
#define __WIDGET_H__
/**
 * @file widget.h
 * Widget class definition
 */

#include "pronlib.h"
#include "sigslot.h"

using namespace sigslot;

namespace sombrero {

  class Container;

  /**
   * Widget class definition. A widget is something dealing with 
   * graphic and events
   */
  class Widget {

    private:
      Container *parent; /**< The parent widget */
      int lastX; /**< last x coordinate received/sent from/to pron */
      int lastY; /**< last y coordinate received/sent from/to pron */
      int x; /**< x coordinate */
      int y; /**< y coordinate */
      int lastWidth; /**< last Width received/sent from/to pron */
      int lastHeight; /**< last Height received/sent from/to pron */
      int width; /**< Width */
      int height; /**< Height */
      bool active; /**< Dealing with activable widgets */
      bool visible; /**< The widget has to be shown */
      uint32_t eventMask; /**< The event mask */
      uint32_t dontPropagateEventMask; /**< The event mask to don't propagate */

    protected:
      /**
       * The pronWindow associated with the widget.
       * Has to be reparented when the widget is reparented.
       */
      pron::Window pronWindow;
      /**
       * Widget default constructor that does nothing.
       */
      Widget();
      /**
       * Widget destructor. Destroys the pron top window
       */
      ~Widget();
      /**
       * initialize the widget
       */
      void init();

    public:
      // Signals
      signal0<> resized;
      /**
       * Signal sent when the mouse moves.
       * <xMove, yMove, relativeXPosition, relativeYPosition>
       */
      signal4<int, int, int, int> mouseDrag;
      /**
       * Subscribes to new event
       * @param eventMask The event mask to add (only one event at once)
       */
      void subscribeEvent(uint32_t eventMask);
      /**
       * Unsubscribes to new event
       * @param eventMask The event mask to add (only one event at once)
       */
      void unsubscribeEvent(uint32_t eventMask);
      /**
       * Don't propagate new event
       * @param eventMask The event mask to propagate (only one event at once)
       */
      void dontPropagateEvent(uint32_t eventMask);
      /**
       * Propagates the event
       * @param eventMask The event mask to propagate (only one event at once)
       */
      void propagateEvent(uint32_t eventMask);
      // Getters and setters
      /**
       * Sets the parent container
       * @param Pointer to the parent container
       */
      virtual void setParent(Container *parent);
      /**
       * Gets x
       * @return x
       */
      int getX();
      /**
       * Sets the x
       * @param x
       */
      void setX(int x);
      /**
       * Gets y
       * @return y
       */
      int getY();
      /**
       * Sets y
       * @param y
       */
      void setY(int y);
      /**
       * Sends the new position to pron
       */
      void updatePronPosition();
      /**
       * Gets the width
       * @return width
       */
      int getWidth();
      /**
       * Sets the width
       * @param width
       */
      void setWidth(int width);
      /**
       * Gets the height
       * @return height
       */
      int getHeight();
      /**
       * Sets the height
       * @param height
       */
      void setHeight(int height);
      /**
       * Sends the new size to pron
       */
      void updatePronSize();
      /**
       * Updates the widgets to pron
       */
      virtual void update();
      /**
       * Gets active
       * @return active
       */
      bool isActive();
      /**
       * Sets active
       * @param active
       */
      void setActive(bool active);
      /**
       * Gets visible
       * @return visible
       */
      bool isVisible();
      /**
       * Sets visible
       * @param visible
       */
      void setVisible(bool visible);
      /**
       * Put here widgets drawing stuff
       */
      virtual void draw() = 0;
      /**
       * Clears the widget
       */
      void clear();


      /**
       * Handle for a windowCreated event
       */
      virtual void handleEventWindowCreated() ;
      /**
       * Handle for an expose event
       */
      virtual void handleEventExpose() ;
      /**
       * Handle for a pointerMoved event
       */
      virtual void handleEventPointerMoved(pron::EventPointerMoved * e) ;
      /**
       * Handle for a mouseButton event
       * @param e A pointer to the event that accured
       */
      virtual void handleEventMouseButton(pron::EventMouseButton * e) ;
      /**
       * Handle for a keyPressed event
       * @param e A pointer to the event that occured
       */
      virtual void handleEventKeyPressed(pron::PronEvent * e __attribute__((unused))) ;
      /**
       * Handle for a keyReleased event
       */
      virtual void handleEventKeyReleased() ;
      /**
       * Handle for a destroyWindow event
       */
      virtual void handleEventDestroyWindow() ;
      /**
       * Handle for a resizeWindow event
       */
      virtual void handleEventResizeWindow(int width, int height) ;
      
      /**
       * Gets parent container
       * @return The parent Container
       */
      Container* getParent();

  };

} // namespace sombrero

#endif//__WIDGET_H__
