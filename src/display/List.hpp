#include "ListLine.hpp"

#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H

#define DISPLAY_LIST_LINES 5
#define DISPLAY_LIST_LINE_HEIGHT 10
#define MAX_LIST_CAPACITY 30

class List {
  public:
    void init();
    void addListLine(ListLine *line);
  protected:
    ListLine lines[MAX_LIST_CAPACITY];
};

#endif
