#pragma once

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <functional>
#include <new>

namespace mtm {
    template <typename T>
    class SortedList {
        //class SortedListNode; //TODO fix dependencies
        //TODO maybe delete these comment lines
        //provides the de-facto interface for interacting with SortedListNode,
        //wrapping function provides flexibility with static like parameters such as
        //head and tail, without forcing a single instance of the list class.
        class ConstIterator;
        class SortedListNode {
            //TODO maybe delete these comment lines
            //the members of this class are only accessible by SortedList class
        private:
            friend class SortedList<T>;
            SortedListNode* prev;
            SortedListNode* next;
            T* data;

            SortedListNode(): prev(nullptr), next(nullptr), data(nullptr) {}

            SortedListNode(T data): prev(nullptr), next(nullptr) {
                this->data = new T(data);
            }

            //FIXME might be redundant
            SortedListNode(SortedListNode* other) {
                this();
                this->data = new T(other->data);
            }

            ~SortedListNode() {
                this->prev = nullptr;
                this->next = nullptr;
                delete this->data;
            }

            void deleteNode() {
                this->prev->next = this->next;
                this->next->prev = this->prev;
                this->next = nullptr;
                this->prev = nullptr;
                delete this;
            }

            void addImmediate(SortedListNode* newNode) {
                assert(!this->isTail());
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }

            void add(SortedListNode* newNode) {
                //if we arrived at the end of the chain
                if (!this->hasNext()) {
                    assert(this->next->isTail());
                    this->addImmediate(newNode);
                    assert(this->isSorted());
                    return;
                }
                //if we arrived at the orderly place of the new node
                if (newNode < this->next) {
                    assert(!this->next->isTail());
                    this->addImmediate(newNode);
                    assert(this->isSorted());
                    return;
                }
                //if we are yet to find the place for the new node nor did we arrive to the end of the chain
                this->next->add(newNode);
                assert(this->isSorted());
            }

            // SortedListNode(SortedListNode* prev = nullptr, SortedListNode* next = nullptr, T* data = nullptr) :
            // prev(prev), next(next), data(data) {}

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

            inline bool isTail() const {
                return (this->data == nullptr)&&(this->prev != nullptr)&&(this->next == nullptr);
            }

            inline bool isHead() const {
                return (this->data == nullptr)&&(this->next != nullptr)&&(this->prev == nullptr);
            }

            inline bool hasNext()   const {
                return !(this->next->isTail());
            }

            void insert(const T& data) {
                // if (data == nullptr) {
                //     //TODO invalid argument
                // }
                SortedListNode* newNode = new SortedListNode(data);
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

            //FIXME CORRECT FOR SYNTAX
            bool operator==(SortedListNode* other) const {
                return (this->data == other->data);
            }

            bool operator!=(SortedListNode* other) const {
                return !(*this == *other);
            }

            bool operator>(SortedListNode* other) const {
                return (this->data > other->data);
            }

            bool operator<(SortedListNode* other) const {
                bool equal = *this == *other;
                bool greater = *this > *other;
                return (!equal) && (!greater);
            }

            bool operator<=(SortedListNode* other) const {
                return !(*this > *other);
            }

            bool operator>=(const SortedListNode* other) const {
                return !(*this < *other);
            }
        };
        private:
        int listLength;
        SortedListNode* head;
        SortedListNode* tail;

        bool isEmpty() const{
            return (this->listLength == 0);
        }

        bool isSorted() const {
            if ((this->isEmpty())){
                return true;
            }
            return head->isSorted();
        }

        bool verifyLength() const {
            int actualLength = 0;
            for(const T& data : *this) {
                ++actualLength;
            }
            return actualLength == this->listLength;
        }

        bool verifyList() const {
            return (this->isSorted())&&(this->verifyLength());
        }


        void deleteNode(SortedListNode* node) {
            if (node == nullptr) {
                return;
            }
            node->deleteNode();
            --(this->listLength);
            assert(this->verifyList());
        }

        public:
        //class NodeIterator;
        //class ConstIterator;

        ConstIterator begin() const {
            return ConstIterator(this->head->next);
        }

        ConstIterator end() const {
            return ConstIterator(this->tail);
        }

        SortedList() : listLength(0){
            try {
                this->head = new SortedListNode();
                this->tail = new SortedListNode();
            } catch (std::bad_alloc& e) {
                delete this->head;
                delete this->tail;
            }
            this->head->next = this->tail;
            this->tail->prev = this->head;
        }

        SortedList(const SortedList<T>& other): SortedList<T>(){
            for(const T& currentData : other) {
                this->insert(currentData);
            }
            assert(this->verifyList());
        }

        SortedList<T>& operator=(const SortedList<T>& other) {
            if(this == &other) {
                return *this;
            }
            
            // for(ConstIterator iter : *this) {
            //     this->remove(*iter);
            // }
            SortedListNode* current = this->head->next;
            while(current != this->tail) {
                SortedListNode* next = current->next;
                delete current;
                current = next;
            }
           this->listLength = 0;
            if(other.head == nullptr) {
                return *this;
            }
            // for(ConstIterator iter : other) {
            //     this->insert(*iter);
            // }
            // ConstIterator otherCurrent = other.begin();
            // while(otherCurrent != other.end()) {
            //     this->insert(*otherCurrent);
            //     ++otherCurrent;
            // }
            for(const T& currentData : other) { //TODO create iterator for SortedListNodes
                this->insert(currentData);
            }
            return *this;
        }

        ~SortedList() {
            SortedListNode* current = this->head;
            while(current != nullptr) {
                SortedListNode* next = current->next;
                delete current;
                current = next;
            }
        }

        // void insert(const T& newElement) {
        //     ConstIterator iter = this->begin();
        //     while( newElement < *iter && iter != this->end()) {
        //         ++iter;
        //     }
        //     if(iter == this->end()) {
        //         SortedListNode* newNode = new SortedListNode(newElement);
        //         this->tail->prev = newNode;
        //         newNode->next = this->tail;
        //         ++(this->listLength);
        //         return;
        //     }
        //     SortedListNode* newNode = new SortedListNode(newElement);
        //     newNode->prev = iter.current;
        //     newNode->next = iter.current->next;
        //     iter.current->next->prev = newNode;
        //     iter.current->next = newNode;
        //     ++(this->listLength);
        // }

        void insert(const T& newElement) {
            this->head->insert(newElement);
            ++(this->listLength);
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
            return this->listLength;
        }

        //SortedList<T> filter(std::function<bool(T)> filterFunc) const {}
        
        void remove(ConstIterator iter){
            if(iter == this->end()) {
                return;
            }
            if(iter == this->begin()) {
                this->head = this->head->next;
            }
            else{
                SortedListNode* nextNode = iter.current->next;
                SortedListNode* prevNode = iter.current->prev;
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
            }
            //delete iter;
            --(this->listLength);
        }
        
        template <class Condition>
        SortedList<T> filter(Condition condition) const{
            SortedList<T> filteredList;
            for(const T& val : *this){
                if(condition(val)) {
                    filteredList.insert(val);
                }
            }
            return filteredList;
        }

        template <class Function>
        SortedList<T> apply(Function function) const{
            SortedList<T> appliedList;
            for(const T& val : *this){
                appliedList.insert(function(val));
            }
            return appliedList;
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
        private:
            friend class SortedList<T>;
            SortedListNode* current;
            ConstIterator(SortedListNode* current) : current(current) {}
        public:
            ConstIterator(ConstIterator& other) = default;
            ConstIterator& operator=(const ConstIterator& other) = default;
            ~ConstIterator() = default;
            const T& operator*() const{
                assert(!current->isHead());
                if(current->isTail()) {
                    throw std::out_of_range("Iterator is out of range");
                }
                return *(current->data);
            }
            ConstIterator& operator++(){
                if(current->isTail()){
                    throw std::out_of_range("Iterator is out of range");                
                }
                current = current->next;
                return *this;
            }
            bool operator!=(const ConstIterator& other) const{
                return current != other.current;
            }
            bool operator>(const ConstIterator& other) const{
                return current->data > other.current->data;
            }
            bool operator==(const ConstIterator& other) const{
                return current == other.current;
            }
            ConstIterator operator++(int){
                ConstIterator temp = *this;
                ++(*this);
                return temp;
            }

    /**
     * the class should support the following public interface:
     * if needed, use =defualt / =delete
     *
     * constructors and destructor:
     * 1. a ctor(or ctors) your implementation needs V?
     * 2. copy constructor V?
     * 3. operator= - assignment operator V?
     * 4. ~ConstIterator() - destructor V?
     *
     * operators:
     * 5. operator* - returns the element the iterator points to V?
     * 6. operator++ - advances the iterator to the next element V?
     * 7. operator!= - returns true if the iterator points to a different element V?
     *
     */
    };


    // template <class T>
    // class SortedList<T>::NodeIterator {
    //     /**
    //      * the class should support the following public interface:
    //      * if needed, use =defualt / =delete
    //      *
    //      * constructors and destructor:
    //      * 1. a ctor(or ctors) your implementation needs
    //      * 2. copy constructor
    //      * 3. operator= - assignment operator
    //      * 4. ~ConstIterator() - destructor
    //      *
    //      * operators:
    //      * 5. operator* - returns the element the iterator points to
    //      * 6. operator++ - advances the iterator to the next element
    //      * 7. operator!= - returns true if the iterator points to a different element
    //      *
    //      */
    //     private:
    //     SortedListNode* current;
    //     int index;
    //     public:

    //     NodeIterator() = delete;
    //     NodeIterator(SortedListNode* current) : current(current) {}

    //     bool operator==(const NodeIterator& other) const {
    //         return this->current == other.current;
    //     }

    //     bool operator!=(const NodeIterator& other) const {
    //         return !(this == other);
    //     }

    //     bool operator<(const NodeIterator& other) const {
    //         return this->index < other.index;
    //     }

    //     bool operator>(const NodeIterator& other) const {
    //         return (this != other) && (!(this < other));
    //     }

    //     bool operator<=(const NodeIterator& other) const {
    //         return !(*this > other);
    //     }

    //     bool operator>=(const NodeIterator& other) const {
    //         return !(*this < other);
    //     }

    //     T operator*() const {
    //         return this->current;
    //     }

    // };
};