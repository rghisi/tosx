//
// Created by ghisi on 19.11.23.
//

#include "shell/Shell.h"

#include "algorithm"
#include "misc/Clear.h"
#include "misc/CountDown.h"
#include "misc/CpuTest.h"
#include "misc/Echo.h"
#include "misc/Free.h"
#include "misc/LongTask.h"
#include "misc/MemTest.h"
#include "misc/MemStatus.h"
#include "misc/Null.h"
#include "misc/PiTask.h"
#include "misc/Uptime.h"
#include "shell/commands/List.h"
#include "system/OS.h"
#include "system/TaskCompletionPromise.h"

Shell::Shell() {
  apps = {
      new ExecutableFile("echo", &(Echo::run)),
      new ExecutableFile("free", &(Free::run)),
      new ExecutableFile("pi", &(PiTask::run)),
      new ExecutableFile("long", &(LongTask::run)),
      new ExecutableFile("clear", &(Clear::run)),
      new ExecutableFile("cdn", &(CountDown::run)),
      new ExecutableFile("uptime", &(Uptime::run)),
      new ExecutableFile("null", &(Null::run)),
      new ExecutableFile("memtest", &(MemTest::run)),
      new ExecutableFile("cputest", &(CpuTest::run)),
      new ExecutableFile("memstatus", &(MemStatus::run)),
  };
  commands = {new List(this)};
}

[[noreturn]] void Shell::run() {
  printf("Welcome to the Shell\r\n");
  while (true) {
    if (column == FIRST && prompt) {
      printf("\033[1;92m#\033[0m ");
      prompt = false;
    }
    auto promise = static_cast<PromiseWithReturn<char> *>(
    OS::await(Serial::readCharAsync()));
    auto character = promise->data;
    delete promise;
    printf("%c", character);
    switch (character) {
      case BACKSPACE:
        if (column > 0) {
          line[--column] = 0;
          printf(ERASE);
        } else {
          printf(MOVE_ONE_RIGHT);
        }
        break;
      case CARRIAGE_RETURN:
        printf("\n");
        executeLineHandler();
        resetLine();
        break;
      default:
        if (column == LAST) {
          printf("\n");
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
    return strcmp(app->name, (const char *)commandLine->command()) == 0;
  };

  auto found = std::find_if(apps.begin(), apps.end(), appNameMatcher);

  if (found != std::end(apps)) {
    auto index = found - apps.begin();
    auto app = apps[index];
    auto name = app->name;
    int_fast8_t (*entryPointFunction)(char *) = app->entryPointFunction;
    if (commandLine->endsWith('&')) {
      delete commandLine;
      executeBackground(name, entryPointFunction, line);
    } else {
      delete commandLine;
      executeForeground(name, entryPointFunction, line);
    }
  } else {
    auto commandNameMatcher = [&](ShellCommand *shellCommand) -> bool {
      return strcmp(shellCommand->name, (const char *)commandLine->command()) == 0;
    };
    auto commandFound =
        std::find_if(commands.begin(), commands.end(), commandNameMatcher);
    if (commandFound != std::end(commands)) {
      delete commandLine;
      auto index = commandFound - commands.begin();
      commands[index]->run(line);
    } else {
      printf("command %s%s %snot found%s\r\n",
             COLOR_WHITE_BRIGHT,
             commandLine->command(),
             COLOR_RED_BRIGHT,
             RESET_STYLE
      );
      delete commandLine;
    }
  }
}

void Shell::executeForeground(const char *name,
                              int_fast8_t (*entryPoint)(char *), char *args) {
  auto task = OS::createTask(name, entryPoint, args);
  auto taskCompletionPromise = new TaskCompletionPromise(task);
  OS::schedule(task);
  auto promise = OS::await(taskCompletionPromise);
  delete promise;
}

void Shell::executeBackground(const char *name,
                              int_fast8_t (*entryPoint)(char *), char *args) {
  auto task = OS::createTask(name, entryPoint, args);
  OS::schedule(task);
}

int_fast8_t Shell::run(char *args) {
  auto shell = new Shell();
  shell->run();
  return 0;
}
