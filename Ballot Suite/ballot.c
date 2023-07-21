#include "ballot.h"
#include "helpers.h"

#include <211.h>

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A `ballot_t` (defined in `ballot.h`) is be a pointer to a
// heap-allocated `struct ballot`, with the following invariant:
//
//  - `length <= MAX_CANDIDATES`
//
//  - the first `length` elements of `entries` are initialized
//
//  - the `name` field of each of the first `length` entries is non-NULL
//    and points to an OWNED, heap-allocated string containing only
//    letters, all of which are uppercase (a la isupper(3)).
//
// The remaining elements of `entries` (`MAX_CANDIDATES - length`)
// should be considered uninitialized.

struct entry
{
    char* name;
    bool active;
};

struct ballot
{
    size_t length;
    struct entry entries[MAX_CANDIDATES];
};


ballot_t ballot_create(void)
{
    //
    // TODO: replace with your code:
    //
    ballot_t ballot = mallocb( sizeof( struct ballot ) , "ballot_create" );  // something malloc
    if (ballot == NULL) {
        return NULL;
    }
    ballot->length = 0;
    for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
        ballot->entries[i] = (struct entry) {NULL, false};
    }
    return ballot;
}

void ballot_destroy(ballot_t ballot)
{
    //
    // TODO: your code here
    //
    if ( ballot != NULL ) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            free( ballot->entries[i].name );
        }
        free( ballot ); 
    }

}

// ballot_find_unused_entry: Helper function that finds first unused entry in a ballot and returns a pointer to it

static struct entry* ballot_find_unused_entry (ballot_t b) {
    for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
        if ( b->entries[i].active == 0 ) {
            return &b->entries[i];
        }
    }
    return NULL;
}

void ballot_insert(ballot_t ballot, char* name)
{
    //
    // TODO: your code here
    //

    /*  standardizes name using the clean_name function, adds it
    to the first unused entry in the ballot, and marks it active.*/
    clean_name(name);
    if (ballot->length  == MAX_CANDIDATES) {
        exit(3);
    } 
    struct entry* first_unused_entry_ptr = ballot_find_unused_entry(ballot);
    if ( first_unused_entry_ptr == NULL) {
        free(name);
        return;
    }
    first_unused_entry_ptr->name = name;
    first_unused_entry_ptr->active = true;
    ballot->length++;
    //need to expand the array???????
    // mark first unused entry active
    // work on helper function next ballot_find_unused_entry
}


const char* ballot_leader(ballot_t ballot)
{
    //
    // TODO: replace with your code:
    //
    char* leader = ballot->entries[0].name;
    if (ballot != NULL) {
        for (size_t i = 0; i < ballot->length; ++i) {
            if (ballot->entries[i].active == true ) {
                leader  =  ballot->entries[i].name;
                return leader;
            }
        }   
    }

    return NULL;
}

void ballot_eliminate(ballot_t ballot, const char* name)
{
    //
    // TODO: your code here
    //
    if (ballot != NULL) {
        for (size_t i = 0; i < ballot->length; ++i) {
            if ( ballot->entries[i].name != NULL) {
                if ( strcmp( ballot->entries[i].name, name ) == 0 ) {
                    ballot->entries[i].active = false;
                }  
            } 
        }   
    }
}

void count_ballot(vote_count_t vc, ballot_t ballot)
{
    //
    // TODO: your code here
    //
    if ( ballot_leader( ballot ) != NULL ) {
        size_t* cp = vc_update( vc, ballot_leader( ballot ) );
        if ( cp == NULL ) {
            exit(4);
        }
        *cp += 1;
    }
}

ballot_t read_ballot(FILE* inf)
{
    //
    // TODO: replace with your code:
    //
    char* line;
    if ( ( line =  fread_line(inf) ) == NULL) {
        return NULL;
    } 
    ballot_t ballot = ballot_create();
    while ( line != NULL ) {
         if ( strcmp(line, "%") != 0 ) {
            ballot_insert(ballot, line);
            line = fread_line(inf);
         } else {
            break;
         }    
    }
    free(line);
    // if first line is null return Null
    return ballot;
}

void clean_name(char* name)
{
    //
    // TODO: your code here
    //
    char* const name_copy = strdupb(name, "clean_name");
    size_t src = 0;
    size_t dst = 0;
    while (( src < strlen(name) ) && (name[src] != '\0')) {
        if ( !isalpha(name[src]) ) {
            src++;
        } else {
            if ( islower( name[src] ) ) {
                name_copy[dst] = toupper(name[src]);
                src++;
                dst++;                
            } else {
                name_copy[dst] = name[src];     
                src++;
                dst++;
            }   
        }  
    }
   name_copy[dst] = '\0';
   strcpy(name, name_copy);
   free( name_copy );
}

void print_ballot(FILE* outf, ballot_t ballot)
{
    //
    // TODO: your code here
    //
    // Loop over entires in ballot
    for (size_t i=0; i<ballot->length; ++i) {

        // Display differently if ballot is inactive
        const char* start_str = " ";
        const char* end_str = "";
        if (!(ballot->entries[i].active)) {
            start_str = "[";
            end_str = "]";
        }

        // Actually print the entry
        fprintf(outf, "%s%s%s\n", start_str, ballot->entries[i].name, end_str);
    }
}
