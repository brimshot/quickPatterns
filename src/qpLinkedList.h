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

    ~qpLinkedList() {
      qpListNode <T> *cur = firstElement;
      while(cur) {
        qpListNode <T> *next = cur->next;
        delete cur->item;
        delete cur;
        cur = next;
      }
    }

    uint8_t numElements = 0;

    T *getItem(int index) {

      qpListNode <T> *tmp = this->firstElement;

      for(uint8_t i = 0; i < index; i++)
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

    // Removes an item from the linked list.
    bool remove(T *itemToRemove) {
      qpListNode <T> *current = this->firstElement;
      qpListNode <T> *previous = nullptr;

      while(current) {

        if(current->item == itemToRemove) {
          // If this node is currently marked as the "current node", update it.
          if(this->currentElement == current) {
            this->currentElement = current->next;
          }

          // update the previous node to point to the new item
          if(previous) {
            previous->next = current->next;
          }

          // Update the first/last items
          if(current == this->firstElement) {
            this->firstElement = previous;
          }
          if(current == this->lastElement) {
            this->lastElement = current->next;
          }

          delete current->item;
          delete current;
          return true;
        }

        previous = current;
        current = current->next;
      }

      return false;
    }

    /*
    bool removeAtIndex(int index) {

      qpListNode <T> *tmp = this->firstElement;

      for(uint8_t i = 0; i < index-1; i++)
        tmp = tmp->next;

      qpListNode <T> *tmp = this->firstElement;

      return tmp->item;

    }
    */

};

#endif
