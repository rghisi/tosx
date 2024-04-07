//
// Created by ghisi on 08.03.24.
//

#ifndef AVROSTESTS_STRING_H
#define AVROSTESTS_STRING_H


#include "cstdint"
#include "cstddef"
#include "cstring"

class String {
public:
    static const int BEGINNING = 0;

    static int_fast16_t findFirst(char c, const char* in) {
        return find(c, in, BEGINNING);
    }

    static int_fast16_t find(char c, const char* in, int_fast16_t startingAfter) {
        if (in == nullptr) {
            return -1;
        }

        int_fast16_t index = startingAfter;
        char nextC = in[index];
        while (nextC != c && nextC != 0) {
            nextC = in[++index];
        }

        if (nextC == c) {
            return index;
        } else {
            return -1;
        }
    }

    static size_t count(char c, const char* in) {
        size_t index = 0;
        size_t count = 0;
        while (in[index] != 0) {
            if (in[index] == c) {
                count++;
            }
            index++;
        }

        return count;
    }

    static char** split(char separator, const char* in) {
        size_t numberOfParts = count(separator, in) + 1;
        auto parts = new char*[numberOfParts];
        int_fast16_t start = 0;
        for (size_t i = 0; i < numberOfParts; i++) {
            int_fast16_t end = find(separator, in, start);
            if (end == -1) {
                end = strlen(in);
            }
            size_t size = end - start;
            auto part = new char[size + 1];
            for (size_t ic = start; ic < end; ic++) {
                part[ic - start] = in[ic];
            }
            part[size] = 0;
            parts[i] = part;
            start = end + 1;
        }

        return parts;
    }
};

#endif //AVROSTESTS_STRING_H
