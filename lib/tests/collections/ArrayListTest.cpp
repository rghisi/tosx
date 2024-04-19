//
// Created by ghisi on 01.09.22.
//

#include <gtest/gtest.h>
#include "collections/ArrayList.h"
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

TEST(ArrayList, ShouldAcceptElementAndReturnTrue_WhenQueueHasSpaceAvailable) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);

    auto queue = ArrayList<Dummy*>();

    ASSERT_TRUE(queue.push(element));
    ASSERT_TRUE(queue.push(element));
}

TEST(ArrayList, ShouldNotAcceptElementAndReturnFalse_WhenQueueIsFull) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(1, elementTwoData, 26);
    uint8_t elementThreeData[] = "xyz";
    auto* elementThree = new Dummy(1, elementThreeData, 4);

    auto queue = ArrayList<Dummy*>();

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_TRUE(queue.push(elementTwo));
    ASSERT_TRUE(queue.isFull());
    ASSERT_TRUE(queue.push(elementThree));
    ASSERT_FALSE(queue.isFull());
}

TEST(ArrayList, ShouldReturnNullptr_WhenEmpty) {
    auto queue = ArrayList<Dummy*>();

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

TEST(ArrayList, ShouldReturnTheNextElement_WhenNotEmpty) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(2, elementTwoData, 26);

    auto queue = ArrayList<Dummy*>();

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_TRUE(queue.push(elementTwo));

    Dummy* firstPolled = queue.pop();
    ASSERT_EQ(elementOne, firstPolled);

    Dummy* secondPolled = queue.pop();
    ASSERT_EQ(elementTwo, secondPolled);

    Dummy* thirdPolled = queue.pop();
    ASSERT_EQ(nullptr, thirdPolled);
}

TEST(ArrayList, ShouldReturnAcceptAndReturnElements_WhenUsedExtensivelly) {
    uint8_t elementOneData[] = "abcde";
    auto* elementOne = new Dummy(1, elementOneData, 6);
    uint8_t elementTwoData[] = "abcdefghijklmnopqrstuvxyz";
    auto* elementTwo = new Dummy(2, elementTwoData, 26);
    uint8_t elementThreeData[] = "xyz";
    auto* elementThree = new Dummy(2, elementThreeData, 4);
    uint8_t elementFourData[] = "xpto";
    auto* elementFour = new Dummy(4, elementFourData, 5);

    auto queue = ArrayList<Dummy*>();

    ASSERT_TRUE(queue.isEmpty());

    ASSERT_TRUE(queue.push(elementOne));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.push(elementTwo));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.isFull());
    ASSERT_TRUE(queue.push(elementThree));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());
    ASSERT_TRUE(queue.push(elementFour));
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_TRUE(queue.isFull());

    Dummy* firstPolled = queue.pop();
    ASSERT_EQ(elementOne, firstPolled);
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());

    Dummy* secondPolled = queue.pop();
    ASSERT_EQ(elementTwo, secondPolled);
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());

    Dummy* thirdPolled = queue.pop();
    ASSERT_EQ(elementThree, thirdPolled);
    ASSERT_FALSE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());

    Dummy* fourthPolled = queue.pop();
    ASSERT_EQ(elementFour, fourthPolled);
    ASSERT_TRUE(queue.isEmpty());
    ASSERT_FALSE(queue.isFull());
}

TEST(ArrayList, ShouldBeEmpty_WhenQueueHasNoItems) {
    auto queue = ArrayList<Dummy*>();

    ASSERT_TRUE(queue.isEmpty());
}

TEST(ArrayList, ShouldNotBeEmpty_WhenQueueHasAtLeastOneItem) {
    uint8_t elementData[] = "abcde";
    auto* element = new Dummy(1, elementData, 6);
    auto queue = ArrayList<Dummy*>();

    queue.push(element);

    ASSERT_FALSE(queue.isEmpty());
}
