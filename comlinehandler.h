
#ifndef COMLINEHANDLER_HEADER
#define COMLINEHANDLER_HEADER

#include "comfystring\comfystr.c"

extern const char* clhErrorDescription[];
typedef enum {
    clhNoError = 1,
    clhBadOption,
    clhNoAliasesProvided,
    clhAliasAlreadySet,
    clhBadHandler,
    clhHandlerIsReady,
    clhNoOptionsProvided,
    clhDuplicateOptionDesignator,
    clhInvalidOptionDesignator,
    clhNoPrimary
} clhError;

typedef int (*clhMainFunc)( const char*, int, int );

typedef struct {
    char* Option;
    clhMainFunc Function;
    char** Aliases;
    unsigned AliasCount;
    unsigned Priority;
    _Bool Primary;
} clhOption;

struct clhnode__ {
    struct clhnode__* nxt;
    clhOption* opt;
};

typedef struct {

    union {
        struct clhnode__* nodes;
        clhOption** arr;
    } Options;

    unsigned OptionCount;
    _Bool Ready;
    
} clhHandler;

clhOption* clhCreateOption();
void clhAllocateOptionAliases( clhOption*, const char**, unsigned, clhError* );

clhHandler* clhCreateHandler();
void clhHandlerAddOption( clhHandler*, clhOption*, clhError* );
void clhHandlerReady( clhHandler*, clhError* );

void clhFreeOption( clhOption* );
void clhFreeHandler( clhHandler* );

#endif
