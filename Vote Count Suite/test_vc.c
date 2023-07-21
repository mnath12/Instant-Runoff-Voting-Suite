#include "../src/libvc.h"

#include <211.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_create_destroy(void)
{
    vc_destroy(vc_create());
}

static void test_2_candidates(void)
{
    // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 2) {
        return;
    }

    size_t* cp;
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }
    CHECK_POINTER( vc_max(vc), NULL );
    CHECK_POINTER( vc_min(vc), NULL );
    CHECK_SIZE( vc_lookup(vc, "alice"), 0 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 0 );

    cp = vc_update(vc, "alice");
    CHECK( cp != NULL );
    if (cp == NULL) {
        // Can't keep testing if cp is NULL
        return;
    }

    *cp += 1;
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "alice" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 1 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 1 );

    cp = vc_update(vc, "bob");
    CHECK( cp );
    *cp += 1;
    cp = vc_update(vc, "alice");
    CHECK( cp );
    *cp += 1;
    CHECK_STRING( vc_max(vc), "alice" );
    CHECK_STRING( vc_min(vc), "bob" );
    CHECK_SIZE( vc_lookup(vc, "alice"), 2 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 1 );
    CHECK_SIZE( vc_total(vc), 3 );

    vc_destroy(vc);
}

//
// You need more test case functions here.
//

static void test_vc_update ( void ) { 
    vote_count_t vc_map = vc_create();
    char data [100];
    for ( int i = 0; i < MAX_CANDIDATES +1; i ++) {
        snprintf ( data , 20 , "Candidate %d" , i );
        // data now holds string " Candidate <i >"
        vc_update ( vc_map , data ); // Use data as the name to insert
    }
    CHECK_SIZE( vc_lookup( vc_map, "Candidate 1" ), 0);
    size_t* cp = vc_update( vc_map, "Candidaate 3" );
    CHECK_POINTER( cp, NULL );
    vc_destroy( vc_map );
} 

static void test_3_candidates(void) {
     // Skip this test if MAX_CANDIDATES isn't large enough.
    if (MAX_CANDIDATES < 3) {
        return;
    }

    size_t* cp;
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }

    CHECK_POINTER( vc_max(vc), NULL );
    CHECK_POINTER( vc_min(vc), NULL );
    CHECK_SIZE( vc_lookup(vc, "jack"), 0 );
    CHECK_SIZE( vc_lookup(vc, "rob"), 0 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 0 );

    cp = vc_update(vc, "jack");
    CHECK( cp != NULL );
    if (cp == NULL) {
        // Can't keep testing if cp is NULL
        return;
    }

    *cp += 1;
    CHECK_STRING( vc_max(vc), "jack" );
    CHECK_STRING( vc_min(vc), "jack" );
    CHECK_SIZE( vc_lookup(vc, "jack"), 1 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 1 );

    cp = vc_update(vc, "rob");
    CHECK( cp );
    *cp += 1;
    cp = vc_update(vc, "jack");
    CHECK( cp );
    *cp += 1;
    CHECK_STRING( vc_max(vc), "jack" );
    CHECK_STRING( vc_min(vc), "rob" );
    CHECK_SIZE( vc_lookup(vc, "jack"), 2 );
    CHECK_SIZE( vc_lookup(vc, "rob"), 1 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 3 );

    cp = vc_update(vc, "rob");
    CHECK( cp );
    *cp += 100;
    cp = vc_update(vc, "jack");
    CHECK( cp );
    *cp += 12;
    CHECK_STRING( vc_max(vc), "rob" );
    CHECK_STRING( vc_min(vc), "jack" );
    CHECK_SIZE( vc_lookup(vc, "jack"), 14 );
    CHECK_SIZE( vc_lookup(vc, "rob"), 101 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 115 );
    
    cp = vc_update(vc, "jack");
    CHECK( cp );
    *cp += 87;
    CHECK_STRING( vc_max(vc), "jack" );
    CHECK_STRING( vc_min(vc), "rob" );
    CHECK_SIZE( vc_lookup(vc, "jack"), 101 );
    CHECK_SIZE( vc_lookup(vc, "rob"), 101 );
    CHECK_SIZE( vc_lookup(vc, "bob"), 0 );
    CHECK_SIZE( vc_total(vc), 202 );

    vc_destroy(vc);
}

static void test_vc_update_full (void) {
    if ( MAX_CANDIDATES != 2 ) {
        return;
    }
    size_t* cp;
    vote_count_t vc = vc_create();
    CHECK( vc != NULL );
    if (vc == NULL) {
        // Can't keep testing if vc is NULL
        return;
    }
    cp = vc_update(vc, "jack");
    CHECK( cp != NULL );
    if (cp == NULL) {
        // Can't keep testing if cp is NULL
        return;
    }

    *cp += 1;

    cp = vc_update(vc, "mona");
    CHECK( cp != NULL );
    if (cp == NULL) {
        // Can't keep testing if cp is NULL
        return;
    }

    *cp += 1;

    cp = vc_update(vc, "james");
    CHECK_POINTER( cp, NULL );

    vc_destroy( vc );
}

static void stress_test  ( void ) {
    if (MAX_CANDIDATES < 1000) {
        return;
    }
    vote_count_t vc_map = vc_create();
    char data [100];
    size_t* cp;
    for ( int i = 0; i < MAX_CANDIDATES; i ++) {
        snprintf ( data , 20 , "Candidate %d" , i );
        // data now holds string " Candidate <i >"
        cp = vc_update ( vc_map , data ); // Use data as the name to insert
        *cp += 1;
        
    }
    for ( int i = 0; i < MAX_CANDIDATES; i ++) {
        snprintf ( data , 20 , "Candidate %d" , i );
        // data now holds string " Candidate <i >"
        CHECK_SIZE( vc_lookup( vc_map , data ), 1 ); // Use data as the name to insert
        }
    vc_destroy( vc_map );
}

int main(void)
{
    test_create_destroy();
    test_2_candidates();
    // TODO: call additional test case functions here
    test_vc_update();
    test_3_candidates();
    test_vc_update_full();
    stress_test();
    return 0;
}


