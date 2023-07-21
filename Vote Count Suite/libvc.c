#include "libvc.h"

#include <211.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This definition is private to this file; code in all other files
// can only handle pointers to it: values of type `struct vote_count*`
// (a/k/a `vote_count_t`), not values of type `struct vote_count`. Only
// code in this file sees this definition, so only code in this file can
// work with `struct vote_count`s directly.
struct vote_count
{
    char*  candidate;
    size_t count;
};

// A vote_count_t will be a pointer to a `malloc`ed array of
// MAX_CANDIDATES `struct vote_count`s, with the following invariant:
//
//  - Some prefix of `candidate` fields are distinct, OWNED, `malloc`ed
//  strings, and the remaining `candidate` fields are NULL.
//
//  - When a `candidate` field is NULL the value of the corresponding
//  `count` does not mean anything, but when the `candidate` is non-NULL
//  then the `count` must be a properly initialized count.

/*
 * Q: Where are header comments for the following functions?
 * A: libvc.h
 */

vote_count_t vc_create(void)
{
    vote_count_t vc = malloc(MAX_CANDIDATES * sizeof(struct vote_count));
    if (vc == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
        vc[i].candidate = NULL;
    }

    return vc;
}

void vc_destroy(vote_count_t vc)
{
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            free(vc[i].candidate);
        }

        free(vc);
    }
}

// Helper function that should return a pointer to row whose `candidate` matches `name`, 
// or NULL if not found.
static vote_count_t
vc_find_name(vote_count_t vc, const char* name)
{
    //
    // TODO: replace with your code:
    //
    // Inputs:  a vote_count_t (array of vote counts) and candidate name
    // Returns a pointer to the first element of `vc `
    // whose ` candidate ` matches ` name ` , or NULL if not found
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            if (vc[i].candidate != NULL) {
                if (strcmp(vc[i].candidate, name) == 0) {
                    return &vc[i];
                }
            } 
        }
    }
            
    return NULL;
}

// Helper function that should return a pointer to first row with NULL `candidate`, or NULL
// if the data structure is full.
static vote_count_t
vc_find_empty(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    // Inputs: vote_count_t
    // Returns a pointer to the first element of `vc ` whose
    // ` candidate ` is NULL , or NULL it 's full .
    if (vc != NULL) {
            for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
                if (vc[i].candidate == NULL) {
                    return &vc[i];
                }
            }
        }

    return NULL;
}

// Helper function to use in some of the
// non-static library functions
static char* strdup_or_die(const char* src)
{
    char* dst = malloc(strlen(src) + 1);
    if (dst == NULL) {
        perror("libvc");
        exit(1);
    }
    strcpy(dst, src);
    return dst;
}

size_t* vc_update(vote_count_t vc, const char *name)
{
    //
    // TODO: replace with your code:
    //
   if (vc != NULL) {
        if ( vc_find_name( vc, name ) != NULL ) {
            return &vc_find_name( vc, name ) -> count;
        } else {
            vote_count_t next_empty_slot_ptr = vc_find_empty( vc );
            if ( vc_find_empty( vc ) == NULL) {
                return NULL;
            }
            char* name_copy = strdup_or_die(name);
            next_empty_slot_ptr -> candidate  = name_copy; 
            next_empty_slot_ptr -> count = 0;
            return &next_empty_slot_ptr -> count;
        }
    }
    return NULL;
}

size_t vc_lookup(vote_count_t vc, const char* name)
{
    //
    // TODO: replace with your code:
    //;
    if (vc != NULL) {
        if ( vc_find_name( vc, name ) != NULL ) {
            return vc_find_name( vc, name ) -> count;
        }
    }
   return 0;
   
}

size_t vc_total(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    size_t retval = 0;
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            if (vc[i].candidate != NULL) {
                retval += vc[i].count;
                }
            } 
        return retval;    
    }
    return 0;
}

const char* vc_max(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    int max_votes = 0;
    const char* winner = vc[0].candidate;
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            if (vc[i].candidate != NULL) {
                if ( vc_lookup( vc, vc[i].candidate ) >  max_votes) {
                    max_votes = vc[i].count;
                    winner = vc[i].candidate;
                } else if ( vc_lookup( vc, vc[i].candidate ) == max_votes) {
                    max_votes = max_votes;
                    winner = winner;
                }
            }
            
        } 
        if (max_votes > 0 ){
            return winner; 
        }
   
    }
    return NULL;
}

const char* vc_min(vote_count_t vc)
{
    //
    // TODO: replace with your code:
    //
    int min_votes = 99999999;
    const char* winner = vc[0].candidate;
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            if (vc[i].candidate != NULL) {
                if ( vc_lookup( vc, vc[i].candidate ) <  min_votes) {
                    min_votes = vc[i].count;
                    winner = vc[i].candidate;
                } else if ( vc_lookup( vc, vc[i].candidate ) == min_votes) {
                    min_votes = vc[i].count;
                    winner = vc[i].candidate;
                }
            }
        } 
        if (min_votes > 0 ){
                return winner; 
            }   
    }
    return NULL;
}

void vc_print(vote_count_t vc)
{
    //
    // TODO: your code here
    //;
    if (vc != NULL) {
        for (size_t i = 0; i < MAX_CANDIDATES; ++i) {
            if ( vc[i].candidate != NULL ) {
                printf("%-20s", vc[i].candidate);
                printf(" ");
                printf("%9zu\n", vc[i].count);
            }
            
        } 
    }
}



