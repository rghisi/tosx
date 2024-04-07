//
// Created by ghisi on 19.11.23.
//

#include "shell/Shell.h"
#include "system/OS.h"
#include "shell/commands/List.h"
#include "algorithm"
#include "misc/Clear.h"
#include "misc/Echo.h"
#include "misc/Free.h"
#include "misc/PiTask.h"
#include "misc/LongTask.h"
#include "misc/CountDown.h"
#include "system/TaskCompletionPromise.h"
#include "misc/Uptime.h"

Shell::Shell() {
    apps = {
                new ExecutableFile("echo", &(Echo::run)),
                new ExecutableFile("free", &(Free::run)),
                new ExecutableFile("pi", &(PiTask::run)),
                new ExecutableFile("long", &(LongTask::run)),
                new ExecutableFile("clear", &(Clear::run)),
                new ExecutableFile("cdn", &(CountDown::run)),
                new ExecutableFile("uptime", &(Uptime::run))
            };
    commands = { new List(this) };
}

[[noreturn]] void Shell::run() {
    Serial::send("Welcome to the Shell\n\r");
    while (true) {
        if (column == FIRST && prompt) {
            Serial::send("\x1b[1;92m#\x1b[0m ");
            prompt = false;
        }
        auto promise = static_cast<PromiseWithReturn<char>*>(OS::await(Serial::readCharAsync()));
        auto character = promise->data;
        delete promise;
        Serial::send(character);
        switch (character) {
            case BACKSPACE:
                if (column > 0) {
                    line[--column] = 0;
                    Serial::send(ERASE);
                } else {
                    Serial::send(MOVE_ONE_RIGHT);
                }
                break;
            case CARRIAGE_RETURN:
                Serial::send(LINE_FEED);
                executeLineHandler();
                resetLine();
                break;
            default:
                if (column == LAST) {
                    Serial::send(LINE_FEED);
                    resetLine();
                } else {
                    line[column++] = character;
                }
                break;
        }
    }
}

void Shell::resetLine() {
    for (char &character : line) {
        character = 0;
    }
    column = 0;
    prompt = true;
}

void Shell::executeLineHandler() {
    if (column == 0) {
        return;
    }

    auto commandLine = new CommandLine(line);

    auto appNameMatcher = [&](ExecutableFile *app) -> bool {
        return strcmp(app->name, (const char*)commandLine->command()) == 0;
    };

    auto found = std::find_if(apps.begin(), apps.end(), appNameMatcher);

    if (found != std::end(apps)) {
        auto index = found - apps.begin();
        int_fast8_t (*entryPointFunction)(char *) = apps[index]->entryPointFunction;
        delete commandLine;
        if (commandLine->endsWith('&')) {
            executeBackground(entryPointFunction, line);
        } else {
            executeForeground(entryPointFunction, line);
        }
    } else {
        auto commandNameMatcher = [&](ShellCommand *shellCommand) -> bool {
            return strcmp(shellCommand->name, (const char*)commandLine->command()) == 0;
        };
        auto commandFound = std::find_if(commands.begin(), commands.end(), commandNameMatcher);
        if (commandFound != std::end(commands)) {
            delete commandLine;
            auto index = commandFound - commands.begin();
            commands[index]->run(line);
        } else {
            Serial::send(COLOR_WHITE_BRIGHT);
            Serial::send(commandLine->command());
            Serial::send(COLOR_RED_BRIGHT);
            Serial::send(" NOT FOUND\r\n");
            Serial::send(RESET_STYLE);
            delete commandLine;
        }
    }
}

void Shell::executeForeground(int_fast8_t (*entryPoint)(char *), char *args) {
    auto task = OS::createTask(entryPoint, args);
    auto taskCompletionPromise = new TaskCompletionPromise(task);
    OS::schedule(task);
    auto promise = OS::await(taskCompletionPromise);
    delete promise;
}

void Shell::executeBackground(int_fast8_t (*entryPoint)(char *), char *args) {
    auto task = OS::createTask(entryPoint, args);
    OS::schedule(task);
    OS::yield();
}

int_fast8_t Shell::run(char *args) {
    auto shell = new Shell();
    shell->run();
    return 0;
}

//void Shell::executeTask(int_fast8_t entryPoint) {

//    auto promise = await(OS::execAsync(task));
//    delete promise;
//}
