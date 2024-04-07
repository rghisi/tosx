//
// Created by ghisi on 08.03.24.
//

#ifndef AVR_TEXT_H
#define AVR_TEXT_H

#include "cstddef"

class FragmentedText {
public:
    FragmentedText();
    void append(char c);
    void append(const char *chars);
    void clear();
    void brk();
    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t fragments() const;
    char* fragment(size_t i);
    char* fullText();

private:
    class Fragment {
    public:
        Fragment() {
            capacity = 8;
            size = 0;
            next = nullptr;
            initializeContainer(capacity);
        }

        void initializeContainer(size_t size) {
            container = new char[size + 1];
            for (size_t i = 0; i <= capacity; i++) {
                container[i] = 0;
            }
        }

        void expand() {
            auto oldContainer = container;
            auto oldCapacity = capacity;

            capacity = capacity << 1; //2x
            initializeContainer(capacity);

            for (size_t i = 0; i < oldCapacity; i++) {
                container[i] = oldContainer[i];
            }

            delete oldContainer;
        }

        void append(char c) {
            if (size == capacity) {
                expand();
            }
            container[size] = c;
            size++;
        }

        Fragment* addFragment() {
            next = new Fragment();

            return next;
        }

        [[nodiscard]] char *text() const {
            auto copy = new char[size + 1];
            for (size_t i = 0; i <= size; i++) {
                copy[i] = container[i];
            }

            return copy;
        }
        char* container{};
        size_t capacity;
        size_t size;
        Fragment* next;
    };

    static constexpr size_t SIZE = 40;
    static constexpr size_t INITIAL_FRAGMENTS = 4;
    Fragment* first;
    Fragment* last;
    size_t textSize;
    size_t fragmentCount;
    Fragment* getFragment(size_t index);

};

FragmentedText::FragmentedText() {
    first = new Fragment();
    last = first;
    textSize = 0;
    fragmentCount = 1;
}

void FragmentedText::append(char c) {
    last->append(c);
    textSize++;
}

void FragmentedText::append(const char *chars) {
    size_t index = 0;
    while (chars[index] != 0) {
        append(chars[index]);
        index++;
    }
}

void FragmentedText::clear() {
    auto toDelete = first;
    do {
        auto nextToDelete = toDelete->next;
        delete toDelete;
        toDelete = nextToDelete;
    } while (toDelete != nullptr);

    first = new Fragment();
    last = first;
    textSize = 0;
    fragmentCount = 1;
}

size_t FragmentedText::size() const {
    return textSize;
}

void FragmentedText::brk() {
    last = last->addFragment();
    fragmentCount++;
}

size_t FragmentedText::fragments() const {
    return fragmentCount;
}

FragmentedText::Fragment *FragmentedText::getFragment(size_t index) {
    auto fragment = first;
    size_t i = 0;
    while (i < index && fragment != nullptr) {
        fragment = fragment->next;
        i++;
    }

    return fragment;
}

char* FragmentedText::fragment(size_t i) {
    auto fragment = getFragment(i);

    if (fragment != nullptr) {
        return fragment->text();
    } else {
        return nullptr;
    }
}

char* FragmentedText::fullText() {
    auto copy = new char[textSize + 1];
    for (size_t i = 0; i <= textSize; i++) {
        copy[i] = 0;
    }
    size_t index = 0;
    auto fragment = first;
    while (fragment != nullptr) {
        for (size_t fragmentTextIndex = 0; fragmentTextIndex < fragment->size; fragmentTextIndex++) {
            copy[index] = fragment->container[fragmentTextIndex];
            index++;
        }
        fragment = fragment->next;
    }

    return copy;
}

#endif //AVR_TEXT_H
