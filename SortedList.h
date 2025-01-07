#pragma once

#include <iostream>
#include <stdexcept>
#include <functional>
#include <new>

namespace mtm {
    template <typename T>
    class SortedList {
        class SortedListNode {
            /* Inner class, part of implementation so no documentation is provided. */
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
            /**
            SortedListNode(SortedListNode* other) : SortedListNode() {
                try{
                    this->data = new T(*(other->data));
                } catch (std::bad_alloc& e) {
                    delete this;
                    throw;
                }
            }
            */

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
                if (*(newNode->data) > *(this->next->data)) {
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

        template <typename V> //FIXME if new assignment operator is not used, then no need for this function
        void swap(V address1, V address2) {
            V temp = address1;
            address1 = address2;
            address2 = temp;
        }

        void swapList(SortedList& other) {
            Swap<SortedListNode*>(this->head, other.head);
            Swap<SortedListNode*>(this->tail, other.tail);
            Swap<int>(this->listLength, other.listLength);
        }

        public:
        class ConstIterator;

        /**
         * @brief Retrieves an iterator to the first element of the sorted list.
         *
         * @return ConstIterator, An iterator pointing to the first element.
         */
        ConstIterator begin() const {
            return ConstIterator(this->head->next);
        }

        /**
         * @brief Retrieves an iterator one element after the last.
         *
         * @return ConstIterator, An iterator pointing past the last element.
         */
        ConstIterator end() const {
            return ConstIterator(this->tail);
        }

        /**
         * @brief Constructor for the SortedList class. Initializes an empty sorted list.
         */
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

        /**
         * @brief Copy constructor for the SortedList class.
         *
         * @param other The SortedList instance to copy.
         */
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

        /**
         * @brief Assignment operator for the SortedList class.
         *
         * @param other The SortedList instance to assign.
         * @return SortedList<T>& A reference to the assigned SortedList instance.
         */
        SortedList<T>& operator=(const SortedList<T>& other) {
            if(this == &other) {
                return *this;
            }
            
            // for(ConstIterator iter : *this) {
            //     this->remove(*iter);
            // }
            /**
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
            */
            SortedList<T> temp = SortedList(other);
            this->swapList(temp);
            return *this;
            /**
             *FIXME
             *
             *maybe better implementation for this:
             *
            *
            if(this == &other) {
                return *this;
            }
            sortedList<T> temp = sortedList(other);
            this->swapList(temp);
            delete temp;
            return *this;

             *
             */
        }

        /**
         * @brief Destructor for the SortedList class. Frees all allocated resources.
         */
        ~SortedList() {
            SortedListNode* current = this->head;
            while(current != nullptr) {
                SortedListNode* next = current->next;
                delete current;
                current = next;
            }
        }

        /**
         * @brief Inserts a new element into the sorted list.
         *
         * @param newElement The element to insert.
         */
        void insert(const T& newElement) {
            try{
                this->head->insert(newElement);
                ++(this->listLength);
            }
            catch (std::bad_alloc& e) {
                throw;
            }
        }

        /**
         * @brief Retrieves the number of elements in the sorted list.
         * 
         * @return int The length of the list.
         */
        int length() const {
            return this->listLength;
        }

        /**
         * @brief Removes an element from the sorted list using a specified iterator.
         *
         * @param iter A const iterator pointing to the element to be removed.
         */
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

        /**
         * @brief Filters the sorted list based on a specified condition.
         *
         * @tparam Condition A callable object that determines whether an element is included.
         * @param condition The filtering condition.
         * @return SortedList<T> A new list containing elements that satisfy the condition.
         */
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

        /**
         * @brief Applies a transformation to each element in the sorted list.
         *
         * @tparam Function A callable object that defines the transformation.
         * @param function The transformation function.
         * @return SortedList<T> A new list containing the transformed elements.
         */
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

            /**
            * @brief Copy constructor for the ConstIterator class.
            *
            * @param other The ConstIterator instance to copy.
            */
            ConstIterator(const ConstIterator& other) {
                this->current = other.current;
            }

            /**
             * @brief Assignment operator for the ConstIterator class.
             *
             * @param other The ConstIterator instance to assign.
             * @return ConstIterator& A reference to the assigned iterator.
             */
            ConstIterator& operator=(const ConstIterator& other) = default;

            ~ConstIterator() = default;

            /**
            * @brief Accesses the element that the iterator points to.
            *
            * @return T& A reference to the element.
            * @throws std::out_of_range If the iterator is out of range.
            */
            T& operator*() const{
                if(current->isTail()) {
                    throw std::out_of_range("Iterator is out of range");
                }
                return *(current->data);
            }

            /**
            * @brief Advances the iterator to the next element in the list.
            *
            * @return ConstIterator& A reference to the advanced iterator.
            * @throws std::out_of_range If the iterator is out of range.
            */
            ConstIterator& operator++(){
                if(current->isTail()){
                    throw std::out_of_range("Iterator is out of range");
                }
                current = current->next;
                return *this;
            }

            /**
            * @brief Compares two iterators for inequality.
            *
            * @param other The iterator to compare against.
            * @return true If the iterators point to different elements.
            * @return false If the iterators point to the same element.
            */
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