#include "libvc.h"

#include <211.h>

#include <stdio.h>

#define OOM_MESSAGE     "%s: out of memory\n"
#define DROP_MESSAGE    "%s: vote dropped: %s\n"
#define FINAL_MESSAGE   "%s: %zu vote(s) dropped\n"

int main(int argc, char* argv[])
{
    //
    // TODO: your code goes here.
    //
    vote_count_t vc = vc_create();
    if ( vc == NULL ) {
       fprintf(stderr, "%s", OOM_MESSAGE);
       return 1;
    }
    size_t  votes_dropped = 0;
    char* line;
    while ((line = read_line()) != NULL) {
        if ( vc_update( vc, line ) == NULL) {
            fprintf(stderr, DROP_MESSAGE, argv[0], line);
            votes_dropped++;
        } else {
           *vc_update( vc, line )+=1;
        }
        free(line);
    }    
    vc_print( vc );
    vc_destroy( vc );
    if ( votes_dropped > 0 ) {
        fprintf(stderr, FINAL_MESSAGE, argv[0], votes_dropped);
        return 2;
    }
    return 0;
}

