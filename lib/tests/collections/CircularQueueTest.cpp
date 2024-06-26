//
// Created by ghisi on 01.09.22.
//

#include <gtest/gtest.h>
#include "collections/CircularQueue.h"
#include "cstdint"

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

TEST(BlockingQueue, ShouldAcceptElementAndReturnTrue_WhenQueueHasSpaceAvailable) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);

    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_TRUE(queue.push(element));
    ASSERT_TRUE(queue.push(element));
}

TEST(BlockingQueue, ShouldNotAcceptElementAndReturnFalse_WhenQueueIsFull) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(1, elementTwoData, 26);
    uint8_t elementThreeData[] = "xyz";
    auto* elementThree = new Dummy(1, elementThreeData, 4);

    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_TRUE(queue.push(elementTwo));
    ASSERT_FALSE(queue.push(elementThree));
}

TEST(BlockingQueue, ShouldReturnNullptr_WhenEmpty) {
    auto queue = CircularQueue<Dummy*, 2>();

    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(2, elementTwoData, 26);
    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_TRUE(queue.push(elementTwo));

    Dummy* firstPolled = queue.pop();
    ASSERT_EQ(elementOne, firstPolled);

    Dummy* secondPolled = queue.pop();
    ASSERT_EQ(elementTwo, secondPolled);

    Dummy* thirdPolled = queue.pop();
    ASSERT_EQ(nullptr, thirdPolled);
}

TEST(BlockingQueue, ShouldReturnTheNextElement_WhenNotEmpty) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(2, elementTwoData, 26);

    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_TRUE(queue.push(elementTwo));

    Dummy* firstPolled = queue.pop();
    ASSERT_EQ(elementOne, firstPolled);

    Dummy* secondPolled = queue.pop();
    ASSERT_EQ(elementTwo, secondPolled);

    Dummy* thirdPolled = queue.pop();
    ASSERT_EQ(nullptr, thirdPolled);
}

TEST(BlockingQueue, ShouldReturnAcceptAndReturnElements_WhenUsedExtensivelly) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(2, elementTwoData, 26);
    uint8_t elementThreeData[] = "xyz";
    auto* elementThree = new Dummy(2, elementThreeData, 4);

    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_TRUE(queue.isEmpty());

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.push(elementTwo));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.isFull());
    ASSERT_FALSE(queue.push(elementThree));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.isFull());

    Dummy* firstPolled = queue.pop();
    ASSERT_EQ(elementOne, firstPolled);
    ASSERT_FALSE(queue.isEmpty());

    ASSERT_TRUE(queue.push(elementThree));

    Dummy* secondPolled = queue.pop();
    ASSERT_EQ(elementTwo, secondPolled);
    ASSERT_FALSE(queue.isEmpty());

    Dummy* thirdPolled = queue.pop();
    ASSERT_EQ(elementThree, thirdPolled);
    ASSERT_TRUE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());
}

TEST(BlockingQueue, ShouldBeEmpty_WhenQueueHasNoItems) {
    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_TRUE(queue.isEmpty());
}

TEST(BlockingQueue, ShouldNotBeEmpty_WhenQueueHasAtLeastOneItem) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);
    auto queue = CircularQueue<Dummy*, 2>();

    queue.push(element);

    ASSERT_FALSE(queue.isEmpty());
}

TEST(BlockingQueue, ShouldBeFull_WhenNoMoreElementsAreAccepted) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);
    auto queue = CircularQueue<Dummy*, 2>();

    ASSERT_FALSE(queue.isFull());
    queue.push(element);
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());
    queue.push(element);
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.isFull());
}