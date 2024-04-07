//
// Created by ghisi on 01.09.22.
//

#include <gtest/gtest.h>
#include "collections/LinkedList.h"

class Dummy {
private:
public:
    Dummy() {
        this->someNumber = 0;
        this->someArray = new uint8_t[0];
        this->arraySize = 0;
    }
    Dummy(uint8_t someNumber, uint8_t* someArray, uint8_t arraySize) {
        this->someNumber = someNumber;
        this->arraySize = arraySize;
        this->someArray = someArray;
    }
    ~Dummy() {
        delete [] this->someArray;
    }

    uint8_t someNumber;
    uint8_t arraySize;
    uint8_t* someArray;
};

TEST(LinkedList, ShouldAcceptElementAndReturnTrue_WhenQueueHasSpaceAvailable) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);

    auto list = LinkedList<Dummy*>();

    ASSERT_TRUE(list.isEmpty());
    list.push(element);
    ASSERT_FALSE(list.isEmpty());

    auto popped = list.pop();
    ASSERT_EQ(element, popped);
    ASSERT_TRUE(list.isEmpty());
}

TEST(LinkedList, ShouldAcceptElementAndReturnTrue_WhenQueueHasSpaceAvailable2) {
    uint8_t elementData[] = "abcde";
    auto* elementOne = new Dummy(1, elementData, 5);
    auto* elementTwo = new Dummy(2, elementData, 6);
    auto* elementThree = new Dummy(3, elementData, 7);
    auto* elementFour = new Dummy(4, elementData, 8);

    auto list = LinkedList<Dummy*>();

    ASSERT_TRUE(list.isEmpty());

    list.push(elementOne);
    list.push(elementTwo);
    list.push(elementThree);
    list.push(elementFour);

    ASSERT_FALSE(list.isEmpty());

    ASSERT_EQ(elementOne, list.pop());
    ASSERT_EQ(elementTwo, list.pop());
    ASSERT_EQ(elementThree, list.pop());
    ASSERT_EQ(elementFour, list.pop());

    ASSERT_TRUE(list.isEmpty());
}
