
#ifndef COMLINEHANDLER_ERRORS
#define COMLINEHANDLER_ERRORS

const char* clhErrorDescription[] = {
    "Not Used",
    "No Error",
    "Bad Option provided",
    "No Aliases provided",
    "Alias or Option name already exists",
    "Bad Handler provided",
    "Handler has been readied",
    "No Options provided",
    "Option Name was already registered",
    "Option name is empty or otherwise invalid",
    "Trying to ready handler without primary Option",
    "Trying to register Option without Function pointer",
    "Trying to use non ready handler",
    "Program was invoked without any primary function"
};

#endif
