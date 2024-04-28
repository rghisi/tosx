//
// Created by ghisi on 09.03.24.
//

#include <gtest/gtest.h>
#include "../../../avr/src/console/CommandLine.h"

TEST(CommandLine, ShouldReturnCommand_WhenProvidedWithParameters) {
    auto string = "command parameterOne";

    auto commandLine = CommandLine(string);

    ASSERT_STREQ("command", commandLine.command());
}

TEST(CommandLine, ShouldReturnParameter_WhenProvidedWithParameters) {
    auto string = "command parameterOne parameterTwo";

    auto commandLine = CommandLine(string);

    ASSERT_STREQ("parameterOne", commandLine.parameter(0));
    ASSERT_STREQ("parameterTwo", commandLine.parameter(1));
}

TEST(CommandLine, ShouldReturnTrue_WhenParameterExists) {
    auto string = "command parameterOne parameterTwo";

    auto commandLine = CommandLine(string);

    ASSERT_TRUE(commandLine.hasParameter("parameterOne"));
    ASSERT_TRUE(commandLine.hasParameter("parameterTwo"));
    ASSERT_FALSE(commandLine.hasParameter("parameterThree"));
}

TEST(CommandLine, ShouldReturnTrue_WhenLastCharMatchesOnEndsWith) {
    auto string = "command parameterOne parameterTwo &";

    auto commandLine = CommandLine(string);

    ASSERT_TRUE(commandLine.endsWith('&'));
    ASSERT_FALSE(commandLine.endsWith('*'));
}

TEST(CommandLine, ShouldReturnParametersInASingleString_WhenAtLeastOneIsProvided) {
    auto commandWithParameters = "command parameterOne parameterTwo";
    auto commandLineWithParameters = CommandLine(commandWithParameters);

    auto commandWithoutParameters = "command";
    auto commandLineWithoutParameters = CommandLine(commandWithoutParameters);


    ASSERT_STREQ("parameterOne parameterTwo", commandLineWithParameters.parameters());
    ASSERT_STREQ("", commandLineWithoutParameters.parameters());
}