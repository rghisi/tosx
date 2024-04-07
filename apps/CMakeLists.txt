target_sources(${PROJECT_NAME} INTERFACE
        apps/include/misc/Clear.h
        apps/include/misc/CountDown.h
        apps/include/misc/Echo.h
        apps/include/misc/Free.h
        apps/include/misc/LongTask.h
        apps/include/misc/PiTask.h
        apps/include/shell/CommandLine.h
        apps/include/shell/Console.h
        apps/include/shell/ExecutableFile.h
        apps/include/shell/Shell.h
        apps/include/shell/ShellCommand.h
        apps/include/shell/commands/List.h
)

target_sources(${PROJECT_NAME} PUBLIC
        apps/src/shell/ExecutableFile.cpp
        apps/src/shell/Shell.cpp
)