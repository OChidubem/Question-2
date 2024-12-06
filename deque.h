#ifndef DEQUE_H
#define DEQUE_H

#include <deque>

template <typename ItemType>
class Deque {
private:
    std::deque<ItemType> deque;  // Internal deque container

public:
    // Adds an item to the front
    void pushFront(const ItemType& item) {
        deque.push_front(item);
    }

    // Adds an item to the back
    void pushBack(const ItemType& item) {
        deque.push_back(item);
    }

    // Removes an item from the front
    void popFront() {
        if (!deque.empty()) {
            deque.pop_front();
        }
    }

    // Removes an item from the back
    void popBack() {
        if (!deque.empty()) {
            deque.pop_back();
        }
    }

    // Retrieves the front item
    ItemType front() const {
        if (deque.empty()) {
            throw std::runtime_error("Deque is empty");
        }
        return deque.front();
    }

    // Retrieves the back item
    ItemType back() const {
        if (deque.empty()) {
            throw std::runtime_error("Deque is empty");
        }
        return deque.back();
    }

    // Checks if the deque is empty
    bool isEmpty() const {
        return deque.empty();
    }
};

#endif // DEQUE_H
