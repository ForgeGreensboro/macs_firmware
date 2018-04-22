#include "command.h"
#include "macs.h"

#include <Arduino.h>
#include <strings.h>
#include <stdlib.h>

#define print2(x,y) (Serial.print(x), Serial.println(y))

#define CR '\r'
#define LF '\n'
#define BS '\b'
#define NULLCHAR '\0'
#define SPACE ' '

const char *delimiters            = ", \n";

Command::Command(Macs * app)
{
    _app = app;
}

bool Command::getCommandLineFromSerialPort()
{
    static uint8_t charsRead = 0;

    while(Serial.available())
    {
        char c = Serial.read();
        switch(c) {
            case CR:
            case LF:
            {
                CommandLine[charsRead] = NULLCHAR;
                if(charsRead > 0) {
                    charsRead = 0;
                    Serial.println(CommandLine);
                    return true;
                }
            }
            case BS:
            {
                if(charsRead > 0)
                {
                    CommandLine[--charsRead] == NULLCHAR;
                    Serial << byte(BS) << byte(SPACE) << byte(BS);
                }
                break;
            }
            default:
            {
                if (charsRead < COMMAND_BUFFER_LENGTH) {
                    CommandLine[charsRead++] = c;
                }
                CommandLine[charsRead] = NULLCHAR;
                break;
            }
        }
    }
}

char * Command::readWord()
{
    char * word = strtok(NULL, delimiters);
    return word;
}

void Command::nullCommand(char * ptrToCommandName)
{
    print2("Command not found: ", ptrToCommandName);
}

void Command::statusCommand()
{
    Serial.println("Status");
    Serial.print("Current Tag: ");
    Serial.println(_app->getCurrentTag());
    Serial.println();
    Serial.print("Relay Status:");
    Serial.print(_app->getCurrentTag() ? "On" : "Off");

    //print2("Wireless is", Wifi.status() == WL_CONNECTED ? "Connected" : "Not Connected");
}

void Command::setupCommand()
{
    char * attributeName = readWord();
    char * value = readWord();

    Serial.print("Setup Command: ");
    print2(attributeName, value);
}

bool Command::DoMyCommand()
{
    int result;

    char * ptrToCommandName = strtok(CommandLine, delimiters);

    if(strcmp(ptrToCommandName, statusCommandToken) == 0)
    {
        statusCommand();
    } else if(strcmp(ptrToCommandName, setupCommandToken) == 0)
    {
        setupCommand();
    } else {
        nullCommand(ptrToCommandName);
    }
}
