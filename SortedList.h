#pragma once

#include <iostream>
#include <stdexcept>

namespace mtm {

    template <typename T>
    class SortedList {
        template <typename T>
        class SortedListNode {
            private:
            friend class SortedList<T>;
            SortedListNode<T> * prev;
            SortedListNode<T> * next;
            T* data;
            void add(SortedListNode<T> * newNode) {
                if (newNode <= this->next){ //TODO create > operator, make null node smallest;
                    this->next->add(newNode);
                    return;
                }
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }
            SortedListNode() : prev(nullptr), next(nullptr), data(nullptr) {}
            public:
            void add(T const& const data) {
                if (data == nullptr) {
                    //TODO invalid argument
                }
                SortedListNode<T> * newNode = new SortedListNode<T>(data);
                this->add(newNode);
            }
            ~SortedListNode() {
                this->prev = nullptr;
                this->next = nullptr;
                delete this->data;
            }
        };
        private:
        int length;
        SortedListNode<T>* head;
        SortedListNode<T>* tail;
        class SortedListNode { 
        private:
            T* data;
            SortedListNode<T>* next;
            SortedListNode<T>* previous;
        public:
            SortedListNode(T* data = nullptr, SortedListNode<T>* next = nullptr, SortedListNode<T>* previous = nullptr) : data(data), next(next), previous(previous) {}
            ~SortedListNode() {
                delete data;
            }
        };


    public:
        SortedList() : length(0), head(nullptr), tail(nullptr) {}

        SortedList(const SortedList& other) : length(other.length){
            SortedListNode<T>* currentOther = other.head;
            SortedListNode<T>* currentNode = this->head;
             SortedListNode<T>* previousNode = nullptr;
            while(currentOther != nullptr) {
                currentNode = new SortedListNode<T>(currentOther->data, nullptr, previousNode);
                if(previousNode == nullptr) {
                    this->head = currentNode;
                }
                if(previousNode != nullptr) {
                    previousNode->next = currentNode;
                }
                this->tail = currentNode;
                previousNode = currentNode;
                currentOther = currentOther->next;
        }
        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list.
         * 2. copy constructor
         * 3. operator= - assignment operator
         * 4. ~SortedList() - destructor
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method
         * 7. end method
         *
         * functions:
         * 8. insert - inserts a new element to the list
         * 9. remove - removes an element from the list
         * 10. length - returns the number of elements in the list
         * 11. filter - returns a new list with elements that satisfy a given condition
         * 12. apply - returns a new list with elements that were modified by an operation
         */

    };

    template <class T>
    class SortedList<T>::ConstIterator {
    /**
     * the class should support the following public interface:
     * if needed, use =defualt / =delete
     *
     * constructors and destructor:
     * 1. a ctor(or ctors) your implementation needs
     * 2. copy constructor
     * 3. operator= - assignment operator
     * 4. ~ConstIterator() - destructor
     *
     * operators:
     * 5. operator* - returns the element the iterator points to
     * 6. operator++ - advances the iterator to the next element
     * 7. operator!= - returns true if the iterator points to a different element
     *
     */
    };
}

