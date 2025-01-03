#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>

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
                if (newNode > this->next){ //TODO create > operator, make null node smallest;
                    this->next->add(newNode);
                    return;
                }
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }
            SortedListNode(SortedListNode<T>* prev = nullptr, SortedListNode<T>* next = nullptr, T* data = nullptr) : prev(prev), next(next), data(data) {}
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

        void addInitial(SortedListNode<T>* addThis){
            assert(this->isEmpty());
            this->head->add(addThis);
        }

        void add(SortedListNode<T>* addThis){
            if(this->isEmpty()){
                this->addInitial(addThis);
            }
            for (SortedListNode<T>* node : this){ //FIXME adjust to work correctly with the iterator
                if (addThis > node){
                    node->addPrevious(addThis); //TODO add addprevious method to nodes
                    break;
                }
            }
        }

    public:
        SortedList() : length(0), head(nullptr), tail(nullptr) {}
        SortedList(const SortedList& other) : length(0){
            if(other.head == nullptr) {
                this->head = nullptr;
                this->tail = nullptr;
                return;
            }
            for(SortedListNode<T> currentOther : other) { //TODO create iterator for SortedListNodes 
                this->insert(currentOther.data);
            }
            
            // SortedListNode<T>* currentOther = other.head;
            // SortedListNode<T>* currentNode = this->head;
            //  SortedListNode<T>* previousNode = nullptr;
            // while(currentOther != nullptr) {
            //     currentNode = new SortedListNode<T>(previousNode, nullptr, currentOther->data);
            //     if(previousNode == nullptr) {
            //         this->head = currentNode;
            //     }
            //     if(previousNode != nullptr) {
            //         previousNode->next = currentNode;
            //     }
            //     this->tail = currentNode;
            //     previousNode = currentNode;
            //     currentOther = currentOther->next;
        }
        
        inline bool isEmpty(){
            return this->length == 0;
        }

        SortedList<T>& operator=(const SortedList& other) {
            if(this == &other) {
                return *this;
            }
            
            SortedListNode<T>* current = this->head;
            while(current != nullptr) {
                SortedListNode<T>* next = current->next;
                delete current;
                current = next;
            }
            this->head = nullptr;
            this->tail = nullptr;
            this->length = 0;
            if(other.head == nullptr) {
                return *this;
            }
            for(SortedListNode<T> currentOther : other) { //TODO create iterator for SortedListNodes 
                this->insert(currentOther.data);
            }
            // SortedListNode<T>* currentOther = other.head;
            // SortedListNode<T>* currentNode = this->head;
            // SortedListNode<T>* previousNode = nullptr;
            // while(currentOther != nullptr) {
            //     currentNode = new SortedListNode<T>(previousNode, nullptr, currentOther->data);
            //     if(previousNode == nullptr) {
            //         this->head = currentNode;
            //     }
            //     if(previousNode != nullptr) {
            //         previousNode->next = currentNode;
            //     }
            //     this->tail = currentNode;
            //     previousNode = currentNode;
            //     currentOther = currentOther->next;
            // }
            return *this;
        }
        ~SortedList() {
            SortedListNode<T>* current = this->head;
            while(current != nullptr) {
                SortedListNode<T>* next = current->next;
                delete current;
                current = next;
            }
        }

        void insert(const T& newElement){
            if (newElement == nullptr) {
                    //TODO invalid argument
                }
            if(this->head == nullptr) {
                this->head = new SortedListNode<T>(nullptr, nullptr, newElement);
                this->tail = this->head;
                this->length = 1;
                return;
            }  
            this->head->add(newElement);
            this->length++;
        }

        int length() const {
            return this->length;
        }
        
        void remove(ConstIterator<T>& iter){
            if(iter == this->end()) {
                return;
            }
            if(iter == this->begin()) {
                this->head = this->head->next;
            }
            else{
                SortedListNode<T>* nextNode = iter->next;
                SortedListNode<T>* prevNode = iter->prev;
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
            }
            delete iter;
            this->length--;
        }

        template <class Condition>
        SortedList<T> filter(Condition condition) const{
            SortedList<T> filteredList;
            for(const SortedListNode<T>& current : *this){
                if(condition(current.data)) {
                    filteredList.insert(current.data);
                }
            }
            return filteredList;
        }

        template <class Function>
        SortedList<T> apply(Function function) const{
            SortedList<T> appliedList;
            for(const SortedListNode<T>& current : *this){
                appliedList.insert((function(current)).data);
            }
            return appliedList;
        }

        ConstIterator begin() const {
            return ConstIterator(this->head);
        }

        ConstIterator end() const {
            return ConstIterator(nullptr);
        }
        /**
         *
         * the class should support the following public interface:
         * if needed, use =defualt / =delete
         *
         * constructors and destructor:
         * 1. SortedList() - creates an empty list. V
         * 2. copy constructor V
         * 3. operator= - assignment operator V
         * 4. ~SortedList() - destructor V
         *
         * iterator:
         * 5. class ConstIterator;
         * 6. begin method V?
         * 7. end method V?
         *
         * functions:
         * 8. insert - inserts a new element to the list V
         * 9. remove - removes an element from the list V
         * 10. length - returns the number of elements in the list V
         * 11. filter - returns a new list with elements that satisfy a given condition V?
         * 12. apply - returns a new list with elements that were modified by an operation V?
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
};