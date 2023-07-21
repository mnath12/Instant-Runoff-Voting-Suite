#include "ballot_box.h"
#include "helpers.h"

#include <211.h>

#include <stdlib.h>
#include <string.h>

// A `ballot_box_t` (defined in `ballot_box.h`) is a linked list made
// of `struct bb_node`s. Each node owns the next node, so the whole list
// is owned by the pointer to the first node.
struct bb_node
{
    ballot_t ballot;
    struct bb_node* next;
};


// The empty ballot box is the null pointer.
const ballot_box_t empty_ballot_box = NULL;


void bb_destroy(ballot_box_t bb)
{
    //
    // TODO: your code here
    //
    struct bb_node* current = bb; 
    while ( current != NULL ) {
        struct bb_node* next = current->next;
        ballot_destroy(current->ballot);
        free(current);
        current = next;
    }    
}

void bb_insert(ballot_box_t* bbp, ballot_t ballot)
{
    //
    // TODO: your code here
    //
    // Goal: adds a ballot to a ballot box at the front of the list.
    // Source: https://www.learn-c.org/en/Linked_lists#:~:text=To%20iterate%20over%20all%20the,we%20reach%20the%20last%20item.
    ballot_box_t new_bb = mallocb(sizeof(struct bb_node), "bb_insert");
    new_bb->ballot = ballot;
    new_bb->next = *bbp;
    *bbp = new_bb;



}


ballot_box_t read_ballot_box(FILE* inf)
{
    //
    // TODO: replace with your code:
    //
    ballot_box_t bb = empty_ballot_box;
    ballot_t ballot = NULL;
    while ( ( ballot = read_ballot(inf) ) != NULL) {
        ballot_box_t* bbp = &bb;
        bb_insert(bbp, ballot);
    }  
    return bb;
}

vote_count_t bb_count(ballot_box_t bb)
{
    vote_count_t result = vc_create();

    //
    // TODO: your code here
    //
    struct bb_node* current = bb; 
    while ( current != NULL ) {
        ballot_t current_ballot = current->ballot;
        count_ballot(result, current_ballot);
        current = current->next;
    }
    bb_destroy(current);
    return result;
}

void bb_eliminate(ballot_box_t bb, const char* candidate)
{
    //
    // TODO: your code here
    //
    struct bb_node* current = bb; 
    while ( current != NULL ) {
        ballot_t current_ballot = current->ballot;
        ballot_eliminate( current_ballot, candidate );
        struct bb_node* next = current->next;
        current = next;
    }
}

char* get_irv_winner(ballot_box_t bb)
{
    //
    // TODO: replace with your code:
    //
    char* result = strdupb("FIXME", "get_irv_winner");
    //
    // TODO: your code here too
    //
    bool is_winner = 0; 
   
    while ( is_winner != 1 ) {
        vote_count_t vc = bb_count( bb ); 
        int total_votes = vc_total( vc ); 
        const char* leader = vc_max( vc );
        int leader_votes = vc_lookup( vc, leader );
        if ( (float) leader_votes / (float) total_votes > .5) {
            is_winner = 1;
            result = realloc(result, strlen(leader) + 1);
            strcpy(result, leader);
        } else if ( vc_total( vc ) == 0 ) {
            vc_destroy( vc );
            free(result);
            return NULL;
        } else {
            const char* loser = vc_min( vc );
            bb_eliminate( bb, loser);
        }
        vc_destroy( vc );
    }
    return result;
}

