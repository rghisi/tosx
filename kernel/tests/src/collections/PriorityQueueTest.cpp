//
// Created by ghisi on 26.10.22.
//

#include <gtest/gtest.h>
#include "../../../avr/src/collections/PriorityQueue.h"
#include "../../../avr/src/collections/PriorityQueue.cpp"

class Dummy {
private:
public:
    Dummy(uint8_t someNumber, uint8_t *someArray, uint8_t arraySize) {
        this->someNumber = someNumber;
        this->arraySize = arraySize;
        this->someArray = someArray;
    }

    bool operator<(const Dummy &rhs) const {
        return someNumber < rhs.someNumber;
    }

    bool operator>(const Dummy &rhs) const {
        return rhs < *this;
    }

    bool operator<=(const Dummy &rhs) const {
        return !(rhs < *this);
    }

    bool operator>=(const Dummy &rhs) const {
        return !(*this < rhs);
    }

    uint8_t someNumber;
    uint8_t arraySize;
    uint8_t *someArray;
};

TEST(PriorityQueue, ShouldAcceptElementAndReturnTrue) {
    uint8_t elementData1[] = "abcde";
    uint8_t elementData2[] = "abcde";
    uint8_t elementData3[] = "abcde";
    auto element1 = Dummy(1, elementData1, 6);
    auto element2 = Dummy(2, elementData2, 6);
    auto element3 = Dummy(3, elementData3, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_TRUE(queue.offer(&element2));
}

TEST(PriorityQueue, ShouldPopSmallerElementsFirst) {
    uint8_t elementData[] = "abcde";
    auto element1 = Dummy(1, elementData, 6);
    auto element2 = Dummy(2, elementData, 6);
    auto element3 = Dummy(3, elementData, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_TRUE(queue.offer(&element2));
    ASSERT_EQ(&element1, queue.peek());
    queue.pop();
    ASSERT_EQ(&element2, queue.peek());
    queue.pop();
    ASSERT_EQ(&element3, queue.peek());
    queue.pop();
    ASSERT_EQ(&element3, queue.peek());
    queue.pop();

    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_TRUE(queue.offer(&element2));
    ASSERT_EQ(&element1, queue.peek());
    queue.pop();
    ASSERT_EQ(&element2, queue.peek());
    queue.pop();
    ASSERT_EQ(&element3, queue.peek());
}

TEST(PriorityQueue, ShouldPeekSmallerElementsFirst) {
    uint8_t elementData[] = "abcde";
    auto element1 = Dummy(1, elementData, 6);
    auto element2 = Dummy(2, elementData, 6);
    auto element3 = Dummy(3, elementData, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_TRUE(queue.offer(&element2));
    ASSERT_EQ(&element1, queue.peek());
    ASSERT_EQ(&element1, queue.peek());
}

TEST(PriorityQueue, ShouldBeEmptyWhenNoElementsAreAdded) {
    uint8_t elementData[] = "abcde";

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.isEmpty());
}

TEST(PriorityQueue, ShouldNotBeEmptyWhenAtLeastOneElementIsAdded) {
    uint8_t elementData[] = "abcde";
    auto element = Dummy(1, elementData, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.offer(&element));
    ASSERT_FALSE(queue.isEmpty());
}

TEST(PriorityQueue, ShouldBecomeEmptyWhenAllElementsAreRemoved) {
    uint8_t elementData[] = "abcde";
    auto element1 = Dummy(1, elementData, 6);
    auto element2 = Dummy(2, elementData, 6);
    auto element3 = Dummy(3, elementData, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.isEmpty());
    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.offer(&element2));
    ASSERT_FALSE(queue.isEmpty());
    queue.pop();
    ASSERT_FALSE(queue.isEmpty());
    queue.pop();
    ASSERT_FALSE(queue.isEmpty());
    queue.pop();
    ASSERT_TRUE(queue.isEmpty());
    queue.pop();
    ASSERT_TRUE(queue.isEmpty());
}

TEST(PriorityQueue, ShouldAcceptElementAndReturnTrue2) {
    uint8_t elementData1[] = "abcde";
    uint8_t elementData2[] = "abcde";
    uint8_t elementData3[] = "abcde";
    auto element1 = Dummy(1, elementData1, 6);
    auto element2 = Dummy(2, elementData2, 6);
    auto element3 = Dummy(3, elementData3, 6);

    auto queue = PriorityQueue<Dummy *>();

    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element3));
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_TRUE(queue.offer(&element2));
    ASSERT_EQ(&element1, queue.peek());
    queue.pop();
    element1.someNumber = 4;
    ASSERT_TRUE(queue.offer(&element1));
    ASSERT_EQ(&element2, queue.peek());
    queue.pop();
    ASSERT_EQ(&element3, queue.peek());
    queue.pop();
    ASSERT_EQ(&element3, queue.peek());
    queue.pop();
    ASSERT_EQ(&element1, queue.peek());
}