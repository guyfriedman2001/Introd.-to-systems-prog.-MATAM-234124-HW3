#pragma once

#include <iostream>
#include <stdexcept>
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
        // class ConstIterator;
        class SortedListNode {
        //TODO maybe delete these comment lines
            //the members of this class are only accessible by SortedList class
        private:
            friend class SortedList<T>;
            SortedListNode* prev;
            SortedListNode* next;
            T* data;

            SortedListNode(): prev(nullptr), next(nullptr), data(nullptr) {}

            SortedListNode(T data): SortedListNode() {
                try{
                    this->data = new T(data);
                } catch (std::bad_alloc& e) {
                    delete this;
                    throw;
                }
            }

            //FIXME might be redundant
            SortedListNode(SortedListNode* other) : SortedListNode() {
                try{
                    this->data = new T(*(other->data));
                } catch (std::bad_alloc& e) {
                    delete this;
                    throw;
                }
            }


            void deleteNode() {
                this->prev->next = this->next;
                this->next->prev = this->prev;
                this->next = nullptr;
                this->prev = nullptr;
                delete this;
            }

            void addImmediate(SortedListNode* newNode) {
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }

            void add(SortedListNode* newNode) {
                //if we arrived at the end of the chain
                if (!this->hasNext()) {
                    this->addImmediate(newNode);
                    return;
                }
                //if we arrived at the orderly place of the new node
                if (*(newNode->data) > *(this->next->data)) { //fixme
                    this->addImmediate(newNode);
                    return;
                }
                //if we are yet to find the place for the new node nor did we arrive to the end of the chain
                this->next->add(newNode);
            }

        public:

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

            void insert(const T& data) {
                // if (data == nullptr) {
                //     //TODO invalid argument
                // }
                try{
                     SortedListNode* newNode = new SortedListNode(data);
                    this->add(newNode);
                }
                catch (std::bad_alloc& e) {
                    throw;
                }
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

            friend bool operator>(const SortedListNode& lhs, const SortedListNode& rhs) {
                if (rhs.isTail()) {
                    return true;
                }
                return (lhs.data > rhs.data);
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

        void deleteNode(SortedListNode* node) {
            if (node == nullptr) {
                return;
            }
            node->deleteNode();
            --(this->listLength);
        }

        public:
        //class NodeIterator;
        class ConstIterator;

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
                throw;
            }
            this->head->next = this->tail;
            this->tail->prev = this->head;
        }

        SortedList(const SortedList<T>& other): SortedList<T>(){
            /*
             try {
                this->head = new SortedListNode();
                this->tail = new SortedListNode();
            } catch (std::bad_alloc& e) {
                delete this->head;
                delete this->tail;
            }
            this->head->next = this->tail;
            this->tail->prev = this->head;
            this->listLength = 0;
            */

            for(const T& currentData : other) {
                try{
                    this->insert(currentData);
                } catch (std::bad_alloc& e) {
                    throw;
                }
            }
        }

        SortedList<T>& operator=(const SortedList<T>& other) {
            if(this == &other) {
                return *this;
            }
            
            // for(ConstIterator iter : *this) {
            //     this->remove(*iter);
            // }
             SortedListNode* oldHead = this->head;
             SortedListNode* oldTail = this->tail;
             int oldLength = this->listLength;
             try{
                SortedList* tempOther = new SortedList(other);
                this->head = tempOther->head;
                this->tail = tempOther->tail;
                this->listLength = tempOther->listLength;
                tempOther->head = nullptr;
                tempOther->tail = nullptr;
                delete tempOther;
            } 
            catch (std::bad_alloc& e) {
                this->head = oldHead;
                this->tail = oldTail;
                this->listLength = oldLength;
                throw;
            }
            while(oldHead != nullptr) {
                SortedListNode* nextNode = oldHead->next;
                delete oldHead;
                oldHead = nextNode;
            }
            return *this;
            // for(ConstIterator iter : other) {
            //     this->insert(*iter);
            // }
            // ConstIterator otherCurrent = other.begin();
            // while(otherCurrent != other.end()) {
            //     this->insert(*otherCurrent);
            //     ++otherCurrent;
            // }            
        }

        ~SortedList() {
            SortedListNode* current = this->head;
            while(current != nullptr) {
                SortedListNode* next = current->next;
                delete current;
                current = next;
            }
        }

        void insert(const T& newElement) {
            try{
                this->head->insert(newElement);
                ++(this->listLength);
            }
            catch (std::bad_alloc& e) {
                throw;
            }
        }

        int length() const {
            return this->listLength;
        }

        void remove(const ConstIterator& iter){
            ConstIterator last = this->end();
            if(!(iter != last)) {
                return;
            }
            SortedListNode* current = this->head->next;
            ConstIterator curr = this->begin();
            while(curr != last) {
                if(!(curr != iter)) {
                    current->deleteNode();
                    --(this->listLength);
                    return;
                }
                ++curr;
                current = current->next;
            }
        }
        
        template <class Condition>
        SortedList<T> filter(Condition condition) const{
            SortedList<T> filteredList;
            for(const T& val : *this){
                if(condition(val)) {
                    try{
                        filteredList.insert(val);
                    } 
                    catch (std::bad_alloc& e) {
                        throw;
                    }
                }
            }
            return filteredList;
        }

        template <class Function>
        SortedList<T> apply(Function function) const{
            SortedList<T> appliedList;
            
            for(const T& val : *this){
                try{
                    appliedList.insert(function(val));
                } 
                catch (std::bad_alloc& e) {
                    throw;
                }
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
            const SortedListNode* current;
            ConstIterator(const SortedListNode* current) : current(current) {}
        public:
            ConstIterator(const ConstIterator& other) {
                this->current = other.current;
            }
            ConstIterator& operator=(const ConstIterator& other) = default;
            ~ConstIterator() = default;
            T& operator*() const{
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


};