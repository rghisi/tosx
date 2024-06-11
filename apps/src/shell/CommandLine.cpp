//
// Created by ghisi on 09.03.24.
//

#include <string_view>
#include <memory>
#include "shell/CommandLine.h"

CommandLine::CommandLine(const char *args) {
    this->args = args;
}

CommandLine::~CommandLine() = default;

std::string_view CommandLine::command() {
    auto view = std::string_view(args);
    auto firstSpace = view.find(' ');
    if (firstSpace != std::string_view::npos) {
        view.remove_suffix(view.length() - firstSpace);
    }
//    printf("%.*s\r\n", static_cast<int>(view.length()), view.data());
    return view;
}

std::string_view CommandLine::parameter(uint_fast16_t index) {
    auto view = std::string_view(args);
    auto i = index + 1;
    size_t separatorPosition = 0;
    do {
        separatorPosition = view.find(' ', separatorPosition);
        i--;
    } while (i > 0);
    view.remove_prefix(separatorPosition + 1);

    return view;
}

bool CommandLine::endsWith(char c) {
    auto view = std::string_view(args);
    return view.ends_with(c);
}

std::string_view CommandLine::parameters() {
    auto view = std::string_view(args);
    auto firstSpace = view.find(' ');
    if (firstSpace != std::string_view::npos) {
        view.remove_prefix(firstSpace + 1);
    }

    return view;
}

bool CommandLine::hasParameters() {
    return NumberOfParameters() > 0;
}

uint_fast8_t CommandLine::NumberOfParameters() {
    auto view = std::string_view(args);
    return std::count(view.begin(), view.end(), ' ');
}
