///
/// Tests for functions in ../src/ballot_box.c.
///

#include "ballot_box.h"
#include "ballot.h"
#include "helpers.h"

#include <211.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


///
/// FORWARD DECLARATIONS
///

// Takes the expected winner, followed by all the votes. Each ballot is
// given as successive string arguments, terminated by "%". The final
// argument, after the final ballot's "%", must be NULL.
static void check_election(const char* expected_winner, ...);
static ballot_box_t read_ballot_box_tester ( const char * filename );
// Test case functions (you need more!):
static void three_candidates_tied(void),
            win_on_third_round(void),
            example_from_wikipedia(void),
            read_three_ballots_two_candidates ( void ),
            read_two_empty_ballots ( void ),
            stress_test ( void ),
            test_from_write_up ( void ),
            check_four_empty_ballots ( void ),
            test_all_same ( void ),
            test_one_ballot_one_candidate( void ),
            test_different_sizes ( void ),
            test_five_ballots_three_candidates ( void ),
            win_on_second_round( void ),
            test_all_different ( void ),
            test_bb_count_empty( void );
            
            


///
/// MAIN FUNCTION
///

int main(void)
{
    three_candidates_tied();
    win_on_third_round();
    example_from_wikipedia();
    read_three_ballots_two_candidates ();
    //read_two_empty_ballots ();
    //stress_test ();
    test_from_write_up ();
    read_two_empty_ballots();
    check_four_empty_ballots ();
    test_all_same ();
    test_one_ballot_one_candidate();
    test_different_sizes ();
    test_five_ballots_three_candidates ( );
    win_on_second_round( );
    test_all_different ( );
    test_bb_count_empty( );
}


///
/// TEST CASE FUNCTIONS
///

static void three_candidates_tied(void)
{
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("A",
            "a", "b", "c", "%",
            "a", "b", "c", "%",
            NULL);
}

static void win_on_third_round(void)
{
    if (MAX_CANDIDATES < 6) {
        return;
    }

    check_election("BIDEN",
            "Bloomberg", "Biden", "%",
            "Yang", "Biden", "%",
            "Warren", "Biden", "%",
            "Sanders", "Biden", "%",
            "Biden", "Sanders", "%",
            "Biden", "Gabbard", "%",
            NULL);
}

static void example_from_wikipedia(void)
{
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("SUE",
            "bob", "bill", "s u e", "%",
            "Sue", "Bob", "Bill", "%",
            "Bill!", "Sue!", "BoB!", "%",
            "bob", "bill", "sue", "%",
            "sue", "bob", "bill", "%",
            NULL);
}

// TEST CASE: check election on 4 empty ballots
static void check_four_empty_ballots ( void ) {
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election(NULL,
                    "%",
                    "%", 
                    "%", 
                    "%",
                    NULL);
}


// Test case: read_ballot_box, three ballots two candidates

static void read_three_ballots_two_candidates ( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = read_ballot_box_tester("Resources/three_ballots_two_candidates.in");
    char* winner = get_irv_winner(bb);
    CHECK_STRING(winner, "LUNA");
    free(winner);
    ballot_t b1 = ballot_create();
    ballot_insert(b1, strdupb("Luna", "read_three_ballots_two_candidates"));
    ballot_insert(b1, strdupb("Olaf", "read_three_ballots_two_candidates"));
    bb_insert(&bb, b1);
    vote_count_t vc = bb_count(bb);
    CHECK_INT(vc_lookup(vc, "LUNA"), 3);
    CHECK_INT(vc_lookup(vc, "OLAF"), 1);
    vc_destroy(vc);
    bb_destroy( bb );
    
}

// Test case: test read_ballot_box, two empty ballots, should give a ballot box with two entries, then
// try to insert and test outcome  
static void read_two_empty_ballots ( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = read_ballot_box_tester("Resources/two_empty_ballots.in");
    
    ballot_t b1 = ballot_create(); 
    ballot_t b2 = ballot_create(); 
    ballot_t b3 = ballot_create();


    ballot_insert(b1, strdupb("Luna", "read_two_empty_ballots"));
    ballot_insert(b1, strdupb("Olaf", "read_two_empty_ballots"));
    
    ballot_insert(b2, strdupb("Olaf", "read_two_empty_ballots"));
    ballot_insert(b2, strdupb("Luna", "read_two_empty_ballots"));
    
    ballot_insert(b3, strdupb("Luna", "read_two_empty_ballots"));
    ballot_insert(b3, strdupb("Olaf", "read_two_empty_ballots"));

    bb_insert(&bb, b1);
    bb_insert(&bb, b2);
    bb_insert(&bb, b3);
    
    char* winner = get_irv_winner(bb);
    CHECK_STRING(winner, "LUNA");
    free(winner);    
    bb_destroy( bb );
    
    
} 

static void test_one_ballot_one_candidate( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = read_ballot_box_tester("Resources/one_ballot_one_canidate.in");
    char* winner = get_irv_winner(bb);
    CHECK_STRING(winner, "NAME");
    free(winner);
    vote_count_t vc = bb_count(bb);
    CHECK_INT(vc_lookup(vc, "NAME"), 1);
    vc_destroy(vc);
    bb_destroy( bb );
    
    check_election(NULL,
                        "%",
                        "%", 
                        "%", 
                        "%",
                        NULL);

}



// Test case: ties
static void test_all_different ( void ) {
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("C", "A",
                        "%",
                        "B",
                        "%",
                        "C",
                        "%",
                        NULL);
}

// Test case: all same 

static void test_all_same ( void ) {
    if (MAX_CANDIDATES < 3) {
        return;
    }

    check_election("A",
                    "A",
                    "A",
                    "%",
                    "A",
                    "A" ,
                    "%",
                    "A",
                    "A" ,
                    "%",
                    NULL);
}

// Test case: Stress test via Gradescope
static void stress_test ( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = NULL;/*read_ballot_box_tester("Resources/stress_test.in");*/
    
   ballot_t b1 = ballot_create();
    ballot_t b2 = ballot_create();
    ballot_t b3 = ballot_create();
    ballot_t b4 = ballot_create();
    
    ballot_insert(b1, strdupb("MEMPHIS", "stress_test"));
    ballot_insert(b1, strdupb("NASHVILLE", "stress_test"));
    ballot_insert(b1, strdupb("CHATTANOOGA", "stress_test"));
    ballot_insert(b1, strdupb("KNOXVILLE", "stress_test"));

    ballot_insert(b2, strdupb("NASHVILLE", "stress_test"));
    ballot_insert(b2, strdupb("CHATTANOOGA","stress_test"));
    ballot_insert(b2, strdupb("KNOXVILLE", "stress_test"));
    ballot_insert(b2, strdupb("MEMPHIS", "stress_test"));

    ballot_insert(b3, strdupb("CHATTANOOGA","stress_test"));
    ballot_insert(b3, strdupb("KNOXVILLE", "stress_test"));    
    ballot_insert(b3, strdupb("NASHVILLE", "stress_test"));
    ballot_insert(b3, strdupb("MEMPHIS", "stress_test"));

    ballot_insert(b4, strdupb("KNOXVILLE", "stress_test"));    
    ballot_insert(b4, strdupb("CHATTANOOGA","stress_test"));
    ballot_insert(b4, strdupb("NASHVILLE", "stress_test"));
    ballot_insert(b4, strdupb("MEMPHIS", "stress_test"));

    for (size_t i = 0; i < 42; ++i) {
        bb_insert(&bb, b1);
    }
    for (size_t i = 0; i < 26; ++i) {
        bb_insert(&bb, b2);
    }

    for (size_t i = 0; i < 15; ++i) {
        bb_insert(&bb, b3);
    }

    for (size_t i = 0; i < 17; ++i) {
        bb_insert(&bb, b4);
        print_ballot(stdout, b4);
        printf("-----");
    }


    vote_count_t vc = bb_count(bb);
    CHECK_INT(vc_lookup(vc, "MEMPHIS"), 42);
    CHECK_INT(vc_lookup(vc, "NASHVILLE"), 26);
    CHECK_INT(vc_lookup(vc, "CHATTANOOGA"), 15);
    CHECK_INT(vc_lookup(vc, "KNOXVILLE"), 58);
    vc_destroy(vc);
    /*bb_eliminate(bb, "CHATTANOOGA");
    vote_count_t vc1 = bb_count(bb);
    CHECK_INT(vc_lookup(vc1, "MEMPHIS"), 42);
    CHECK_INT(vc_lookup(vc1, "NASHVILLE"), 26);
    CHECK_INT(vc_lookup(vc1, "KNOXVILLE"), 32);
    vc_destroy(vc1);
    bb_eliminate(bb, "NASHVILLE");
    vote_count_t vc2 = bb_count(bb);
    CHECK_INT(vc_lookup(vc2, "MEMPHIS"), 42);
    CHECK_INT(vc_lookup(vc2, "KNOXVILLE"), 58);
    vc_destroy(vc2);*/
    
    bb_destroy(bb); 
} 

// Test Case: Election from homework writeup
static void test_from_write_up ( void ) {
    if (MAX_CANDIDATES < 3) {
        return;
    }
    ballot_box_t bb = read_ballot_box_tester("Resources/test_from_write_up.in");
        char* winner = get_irv_winner(bb);
        CHECK_STRING(winner, "CAMPBELL");
        free(winner);
        /*ballot_destroy(b1);
        ballot_destroy(b2);    
        ballot_destroy(b3);*/
        bb_destroy( bb );
        
    check_election("CAMPBELL", 
                    "Abbott",
                    "Borden", 
                    "Campbell", 
                    "%",
                    "Campbell",  
                    "Abbott", 
                    "Borden",                    
                    "%",
                    "Borden", 
                    "Campbell", 
                    "Abbott", 
                    "%",
                    "Abbott", 
                    "borden", 
                    "campbell", 
                    "%",
                    "campbell", 
                    "abbott", 
                    "borden", 
                    "%",
                    NULL);
        
}

static void test_different_sizes ( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = read_ballot_box_tester("Resources/different_sizes.in");
    char* winner = get_irv_winner(bb);
    CHECK_STRING(winner, "LUNA");
    free(winner);
    vote_count_t vc = bb_count(bb);
    CHECK_INT(vc_lookup(vc, "LUNA"), 2);
    CHECK_INT(vc_lookup(vc, "OLAF"), 0);
    CHECK_INT(vc_lookup(vc, "JACK"), 1);
    check_election("LUNA", "Luna",
                            "%",
                            "Luna",
                            "Olaf",
                            "%",
                            "Jack",
                            "Olaf",
                            "Luna",
                            NULL);
    vc_destroy(vc);
    bb_destroy( bb );
}

static void test_five_ballots_three_candidates ( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = read_ballot_box_tester("Resources/test_five_ballots_three_candidates.in");
    char* winner = get_irv_winner(bb);
    CHECK_STRING(winner, "BOB");
    free(winner);
    bb_destroy(bb);
}

static void win_on_second_round ( void ) {
    if (MAX_CANDIDATES < 3) {
        return;
    }
    check_election("SUE","BOB", 
                         "BILL", 
                         "SUE", 
                         "%" ,
                         "SUE", 
                         "BILL", 
                         "BOB", 
                         "%", 
                         "BILL", 
                         "SUE", 
                         "BOB",
                         "%",
                         "BOB", 
                         "BILL", 
                         "SUE",
                         "%",
                         "SUE", 
                         "BILL", 
                         "BOB",
                         "%",
                         NULL);
}

static void test_bb_count_empty( void ) {
    if (MAX_CANDIDATES < 3) {
            return;
        }
    ballot_box_t bb = empty_ballot_box;
    vote_count_t vc = bb_count( bb );
    CHECK_SIZE(vc_lookup(vc, "BRUH"), 0);
    CHECK_SIZE(vc_lookup(vc, "BRANDEN"), 0);
    vc_destroy( vc);
    bb_destroy( bb );
    
}


///
/// HELPER FUNCTIONS YOU SHOULD USE
///

// These are helpers for `check_election()` that you are unlikely to
// call directly.
static ballot_box_t build_ballot_box(va_list);
static void check_irv_winner(const char*, ballot_box_t);


// Simulates an election and checks the winner. The first argument
// is the name of the expected winner, and the remaining arguments
// are the names on the ballots, with "%" in between and a NULL
// marking the end. (See uses of this function above for examples.)
static void
check_election(const char* expected, ...)
{
    va_list ap;
    va_start(ap, expected);
    ballot_box_t bb = build_ballot_box(ap);
    va_end(ap);

    check_irv_winner(expected, bb);

    bb_destroy(bb);
}

static ballot_box_t read_ballot_box_tester ( const char * filename ) {
    FILE* inf = fopen( filename, "r");
    ballot_box_t ballot_box = read_ballot_box( inf );
    fclose(inf);
    return ballot_box;
}

/*static void list_print (ballot_box_t* bbp) {
    printf("list:");
    struct bb_node* curr = *bbp;
    while (curr != NULL) {
       print_ballot(stdout, curr->ballot);
        curr = curr->next;
    }
    printf("(null)\n");
} */


/// HELPERS FOR THE HELPERS

// Builds a ballot box from multiple string arguments. Returns
// ownership of the ballot box to the caller.
static ballot_box_t
build_ballot_box(va_list ap)
{
    ballot_box_t bb = empty_ballot_box;
    ballot_t ballot = NULL;

    char* name;
    while ((name = va_arg(ap, char*))) {
        if (!ballot) {
            ballot = ballot_create();
        }

        if (strcmp(name, "%") == 0) {
            bb_insert(&bb, ballot);
            ballot = NULL;
        } else {
            ballot_insert(ballot, strdupb(name, "check_election"));
        }
    }

    if (ballot) {
        bb_insert(&bb, ballot);
    }

    return bb;
}

// Checks that `expected` is the winner of `bb`. Borrows both arguments.
static void
check_irv_winner(const char* expected, ballot_box_t bb)
{
    char* actual = get_irv_winner(bb);

    if (expected) {
        CHECK_STRING(actual, expected);
    } else {
        CHECK_POINTER(actual, expected);
    }

    free(actual);
}
