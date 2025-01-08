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

            /**
             * @brief Constructs an empty node for the sorted list.
             */
            SortedListNode(): prev(nullptr), next(nullptr), data(nullptr) {}

            /**
             * @brief Constructs a node with the given data.
             *
             * @param data The data to store in the node.
             */
            SortedListNode(T data): SortedListNode() { //todo ask regev
                try{
                    this->data = new T(data);
                } catch (std::bad_alloc& e) {
                    delete this;
                    throw;
                }
            }

            /**
             * @brief Deletes the current node and adjusts its neighbors accordingly.
             */
            void deleteNode() {
                this->prev->next = this->next;
                this->next->prev = this->prev;
                this->next = nullptr;
                this->prev = nullptr;
                delete this;
            }

            /**
             * @brief Inserts a new node immediately after the current node.
             *
             * @param newNode The node to insert.
             */
            void addImmediate(SortedListNode* newNode) {
                this->next->prev = newNode;
                newNode->next = this->next;
                this->next = newNode;
                newNode->prev = this;
            }

            /**
             * @brief Inserts a new node into the sorted list, maintaining the order.
             *
             * @param newNode The node to insert.
             */
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
            /**
             * @brief Destructor for the SortedListNode class. Releases all allocated resources.
             */
            ~SortedListNode() {
                this->prev = nullptr;
                this->next = nullptr;
                delete this->data;
            }

            /**
             * @brief Checks if the node is the tail of the list.
             *
             * @return true If the node is the tail.
             * @return false Otherwise.
             */
            inline bool isTail() const {
                return (this->data == nullptr)&&(this->prev != nullptr)&&(this->next == nullptr);
            }

            /**
             * @brief Checks if the node is the head of the list.
             *
             * @return true If the node is the head.
             * @return false Otherwise.
             */
            inline bool isHead() const {
                return (this->data == nullptr)&&(this->next != nullptr)&&(this->prev == nullptr);
            }

            /**
             * @brief Checks if the node has a valid next node.
             *
             * @return true If there is a next node.
             * @return false Otherwise.
             */
            inline bool hasNext()   const {
                return !(this->next->isTail());
            }

            /**
             * @brief Inserts a new element into the sorted list starting from this node.
             *
             * @param data The element to insert.
             */
            void insert(const T& data) {
                SortedListNode* newNode = new SortedListNode(data);
                this->add(newNode);
            }

            /**
             * @brief Equality comparison operator.
             *
             * @param other The node to compare against.
             * @return true If the nodes are equal.
             * @return false Otherwise.
             */
            bool operator==(SortedListNode* other) const {
                return (this->data == other->data);
            }

            /**
             * @brief Inequality comparison operator.
             *
             * @param other The node to compare against.
             * @return true If the nodes are not equal.
             * @return false Otherwise.
             */
            bool operator!=(SortedListNode* other) const {
                return !(*this == *other);
            }

            /**
             * @brief Greater-than comparison operator.
             *
             * @param other The node to compare against.
             * @return true If this node is greater than the other.
             * @return false Otherwise.
             */
            bool operator>(SortedListNode* other) const {
                return (this->data > other->data);
            }

            /**
             * @brief Less-than comparison operator.
             *
             * @param lhs The left node.
             * @param rhs The right node.
             * @return true if lhs > rhs.
             * @return false Otherwise.
             */
            friend bool operator>(const SortedListNode& lhs, const SortedListNode& rhs) {
                if (rhs.isTail()) {
                    return true;
                }
                return (lhs.data > rhs.data);
            }

        };

        private:
        int listLength;
        SortedListNode* head;
        SortedListNode* tail;

        /**
         * @brief Deletes a specified node from the list.
         *
         * @param node The node to delete, is nullptr safe.
         */
        void deleteNode(SortedListNode* node) {
            if (node == nullptr) {
                return;
            }
            node->deleteNode();
            --(this->listLength);
        }

        /**
         * @brief Swaps the values of two things.
         *
         * @tparam V The type of the values to be swapped.
         * @param address1 Address of the first swapped value.
         * @param address2 Address of the second swapped value.
         */
        template <typename V> 
        void swap(V& address1, V& address2) { //todo ask regev
            V temp = address1;
            address1 = address2;
            address2 = temp;
        }

        /**
         * @brief Swaps the contents of the current list with another list.
         *
         * @param other The list to swap with.
         */
        void swapList(SortedList& other) {
            swap<SortedListNode*>(this->head, other.head);
            swap<SortedListNode*>(this->tail, other.tail);
            swap<int>(this->listLength, other.listLength);
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
            SortedList<T> temp = SortedList(other);
            this->swapList(temp);
            return *this;
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
            this->head->insert(newElement);
            ++(this->listLength);
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
            for(ConstIterator curr = begin(); curr!= last; ++curr, current = current->next){
                if(!(curr != iter)) {
                    this->deleteNode(current);
                    return;
                }
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
                    filteredList.insert(val);
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
                appliedList.insert(function(val));
            }
            return appliedList;
        }
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
            ConstIterator(const ConstIterator& other) = default;


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
            const T& operator*() const{
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
    };
};