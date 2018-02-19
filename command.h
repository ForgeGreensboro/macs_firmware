#ifndef __command_h__
#define __command_h__

#define COMMAND_BUFFER_LENGTH        256

class Macs;

class Command {
public:
    bool getCommandLineFromSerialPort();
    bool DoMyCommand();

    Command(Macs *app);

private:
    const char *statusCommandToken = "status";
    const char *setupCommandToken = "setup";
    Macs * _app;

    char   CommandLine[COMMAND_BUFFER_LENGTH + 1];

    char * readWord();
    void nullCommand(char * ptrToCommandName);

    void statusCommand();
    void setupCommand();
};

#endif