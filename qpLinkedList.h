#ifndef QUICK_PATTERNS_LIST_H
#define QUICK_PATTERNS_LIST_H

#include <Arduino.h>

/** A minimalist linked list implementation */

template <typename T>
struct qpListNode {
  T *item;
  qpListNode *next;
};

template <typename T>
class qpLinkedList {

private:

    qpListNode <T> *firstElement = nullptr;
    qpListNode <T> *lastElement = nullptr;

    qpListNode <T> *currentElement = nullptr;

public:

    byte numElements = 0;

    T *getItem(int index) {

      qpListNode <T> *tmp = this->firstElement;

      for(byte i = 0; i < index; i++)
        tmp = tmp->next;

      return tmp->item;
    }


    T *getLast() {
      return this->lastElement->item;
    }


    T *append(T *item) {
      
      if(! this->firstElement) {
        this->firstElement = this->lastElement = new qpListNode<T>();
        this->currentElement = this->firstElement;
      } else {
        this->lastElement->next = new qpListNode<T>();
        this->lastElement = this->lastElement->next;
      }

      this->numElements++;

      this->lastElement->item = item;

      return item;
    }


    // Returns next item on each successive call, nullptr when no items left - resets for next iteration automatically
    T *fetch() {

      T *itemToReturn = nullptr;

      if(this->currentElement) {
        itemToReturn = this->currentElement->item;
        this->currentElement = currentElement->next;
      } else //reset for next iteration
        this->currentElement = this->firstElement;

      return itemToReturn;

    }

};

#endif
