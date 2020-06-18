#ifndef QUICK_PATTERNS_LIST_H
#define QUICK_PATTERNS_LIST_H

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

    void increaseByOneElement() {

        if(! this->firstElement) {
          this->firstElement = this->lastElement = new qpListNode<T>();
          this->currentElement = this->firstElement;
        } else {
          this->lastElement->next = new qpListNode<T>();
          this->lastElement = this->lastElement->next;
        }

        this->numElements++;

    }


public:

    int numElements = 0;

    //Get a specific item from list
    T *getItem(int index) {

      qpListNode <T> *tmp = this->firstElement;

      int i = 0;
      while(tmp) {
        if(i == index)
          return tmp->item;
        tmp = tmp->next;
        i++;
      }

      return nullptr;
    }


    //Get reference to item in last node of list
    T *getLast() {
      return this->lastElement->item;
    }


    //Add an item to the end of the list
    void append(T *item) {

      this->increaseByOneElement();

      this->lastElement->item = item;
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
