
#ifndef COMLINEHANDLER_IMPL
#define COMLINEHANDLER_IMPL

#include "comlinehandler.h"

clhOption* clhCreateOption(){
    return (clhOption*) calloc( 1, sizeof(clhOption) );
}

void clhAllocateOptionAliases( clhOption* opt, const char** strings, unsigned string_count, clhError* e ){

    clhError dummy;
    if( !e ){
        e = &dummy;
    }

    if( !opt ){
        *e = clhBadOption;
        return;
    }

    if( !strings || !string_count ){
        *e = clhNoAliasesProvided;
        return;
    }

    if( opt->Aliases ){
        *e = clhAliasAlreadySet;
        return;
    }

    char** q = (char**) malloc( string_count * sizeof(char*) );
    opt->Aliases = q;
    opt->AliasCount = string_count;

    while( string_count > 0 ){
        q[--string_count] = cfstrCreate(*strings++);
    }

    *e = clhNoError;
}

clhHandler* clhCreateHandler(){
    return (clhHandler*) calloc( 1, sizeof(clhHandler) );
}

void clhHandlerAddOption( clhHandler* handler, clhOption* option, clhError* e ){

    clhError dummy;
    if( !e ){
        e = &dummy;
    }

    if( !handler ){
        *e = clhBadHandler;
        return;
    }

    if( !option ){
        *e = clhBadOption;
        return;
    }

    if( handler->Ready ){
        *e = clhHandlerIsReady;
        return;
    }

    if( cfstrNullOrEmpty(option->Option) ){
        *e = clhInvalidOptionDesignator;
        return;
    }

    for( unsigned i = 0; i < option->AliasCount; ++i ){

        if( cfstrNullOrEmpty(option->Aliases[i]) ){
            *e = clhInvalidOptionDesignator;
            return;
        }

        if( cfstrCompare(option->Aliases[i], option->Option) ){
            *e = clhDuplicateOptionDesignator;
            return;
        }
    }

    struct clhnode__* q = handler->Options.nodes;
    while( q ){

        if( cfstrCompare(option->Option, q->opt->Option ) ){
            *e = clhDuplicateOptionDesignator;
            return;
        }

        char* qa = q->opt->Aliases;
        unsigned i = q->opt->AliasCount;
        while( i-- > 0 ){

            if( cfstrCompare(option->Option, qa[i]) ){
                *e = clhDuplicateOptionDesignator;
                return;
            }

            for( unsigned j = 0; j < option->AliasCount; ++j )
                if( cfstrCompare( option->Aliases[i], qa[i] ){
                    *e = clhDuplicateOptionDesignator;
                    return;
                }
        }

        q = q->nxt;
    }

    struct clhnode__* nd = (struct clhnode__*) malloc( sizeof(struct clhnode__) );
    nd->opt = option;

    handler->OptionCount++;
    if( !(handler->Options.nodes) ){
        nd->nxt = 0;
        handler->Options.nodes = nd;
    }else{
        nd->nxt = handler->Options.nodes;
        handler->Options.nodes = nd;
    }

    *e = clhNoError;
}

void clhFreeOption( clhOption* opt ){

    if( !opt ) return;
    if( opt->Option ) cfstrFree(opt->Option);
    if( opt->Aliases ){

        for( unsigned i = 0; i < opt->AliasCount; ++i )
            cfstrFree(opt->Aliases[i]);

        free( opt->Aliases );
    }

    free(opt);
}

void clhFreeHandler( clhHandler* handler ){

    if( !handler ) return;
    if( handler->Options.nodes ){

        if( handler->Ready ){

            for( unsigned i = 0; i < handler->OptionCount; ++i )
                clhFreeOption(handler->Options.arr[i]);

            free(handler->Options);

        }else{

            struct clhnode__* q = handler->Options.nodes;
            while( q ){

                struct clhnode__* t = q;
                clhFreeOption(q->opt);
                q = q->nxt;
                free(t);
            }
        }
    }

    free(handler);
}

void clhHandlerReady( clhHandler* handler, clhError* e ){

    clhError dummy;
    if( !e ){
        e = &dummy;
    }

    if( !handler ){
        *e = clhBadHandler;
        return;
    }

    if( handler->Ready ){
        *e = clhHandlerIsReady;
        return;
    }

    if( !(handler->Options.nodes) || !handler->OptionCount ){
        *e = clhNoOptionsProvided;
        return;
    }

    clhOption** q0 = (clhOption**) malloc( sizeof(clhOption*) * handler->OptionCount );
    clhOption** q1 = q0;
    struct clhnode__* nd = handler->Options.nodes;
    _Bool hasprimary = 0;

    while( nd ){
        hasprimary |= nd->Primary;
        *q1++ = nd->opt;
        struct clhnode__* t = nd;
        nd = nd->nxt;
        free(t);
    }

    handler->Options.arr = q0;

    if( !hasprimary ){
        *e = clhNoPrimary;
        return;
    }

    *e = clhNoError;
}

#endif
