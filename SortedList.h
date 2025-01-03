#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>

namespace mtm {

    template <typename T>
    class SortedList {
        //TODO maybe delete these comment lines
        //provides the de-facto interface for interacting with SortedListNode,
        //wrapping function provides flexibility with static like parameters such as
        //head and tail, without forcing a single instance of the list class.
        template <typename T>
        class SortedListNode {
            //TODO maybe delete these comment lines
            //the members of this class are only accessible by SortedList class
        private:
            friend class SortedList<T>;
            SortedListNode<T> * prev;
            SortedListNode<T> * next;
            T* data;

            SortedListNode(): prev(nullptr), next(nullptr), data(nullptr) {}

            SortedListNode(T data): prev(nullptr), next(nullptr) {
                this->data = new T(data);
            }

            ~SortedListNode() {
                this->prev = nullptr;
                this->next = nullptr;
                delete this->data;
            }

            inline bool isTail() const {
                return (this->data == nullptr)&&(this->prev != nullptr)&&(this->next == nullptr);
            }

            inline bool isHead() const {
                return (this->data == nullptr)&&(this->next != nullptr)&&(this->prev == nullptr);
            }

            inline bool hasNext()   const {
                return !(this->next->isTail());
            }

            void addImmediate(SortedListNode<T>* newNode) {
                assert(!this->isTail());
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }

            void add(SortedListNode<T>* newNode) {
                //if we arrived at the end of the chain
                if (!this->hasNext()) {
                    this->addImmediate(newNode);
                    assert(this->isSorted());
                    return;
                }
                //if we arrived at the orderly place of the new node
                if (newNode > this->next) {
                    this->addImmediate(newNode);
                    assert(this->isSorted());
                    return;
                }
                //if we are yet to find the place for the new node nor did we arrive to the end of the chain
                this->next->add(newNode);
                assert(this->isSorted());
            }

            SortedListNode(SortedListNode<T>* prev = nullptr, SortedListNode<T>* next = nullptr, T* data = nullptr) :
            prev(prev), next(next), data(data) {}

            /**
             * overload of the isSorted function, to differentiate calls between
             * the head and the rest of the chain
             *
             * @return if the list is sorted
             */
            bool isSorted(int) const {
                if (this->isTail()){
                    return true;
                }
                return (this >= this->next) && (this->next->isSorted(0)); //TODO create >= operator or < and use (!(this < this->next))
            }

        public:
            void insert(T const& const data) {
                if (data == nullptr) {
                    //TODO invalid argument
                }
                SortedListNode<T> * newNode = new SortedListNode<T>(data);
                this->add(newNode);
                assert(this->isSorted());
            }

            /**
             *check if the list is sorted
             *
             * @return if the list is sorted
             */
            bool isSorted() const {
                //the head breaks the sorted logic, if it is the head- skip it.
                return (this->isHead())?this->next->isSorted(0):this->isSorted(0);
            }
        };
    private:
        int length;
        SortedListNode<T>* head;
        SortedListNode<T>* tail;

        /**
         * //maybe can delete this function
        void addInitial(SortedListNode<T>* addThis){
            assert(this->isEmpty());
            this->head->add(addThis);
        }
         */


        /** //maybe can delete this function
        void add(SortedListNode<T>* addThis){
            if(this->isEmpty()){
                this->head->addImmediate(addThis);
                return;
            }
            for (SortedListNode<T>* node : this){ //FIXME adjust to work correctly with the iterator
                if (addThis > node){
                    node->addPrevious(addThis); //TODO add addprevious method to nodes
                    return;
                }
            }
        }
         *
         */


        bool isSorted() const {
            if (this->isEmpty()){
                return true;
            }
            return head->isSorted();
        }

        bool verifyLength() const {
            int actualLength = 0;
            for (SortedListNode<T>* node : this) {
                ++actualLength;
            }
            return actualLength == this->length;
        }

        bool verifyList() const {
            return (this->isSorted())&&(this->verifyLength());
        }

        inline bool isEmpty(){
            return this->length == 0;
        }

    public:

        SortedList<T>() : length(0){
            this->head = new SortedListNode<T>();
            this->tail = new SortedListNode<T>();
            this->head->next = this->tail;
            this->tail->prev = this->head;
        }

        SortedList<T>(const SortedList& other) : length(0){
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

        void insert(const T& newElement) {
            this->head->insert(newElement);
            ++this->length;
            assert(this->verifyList());
        }

        /**
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
         */



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