#include <stdio.h>
#include <assert.h>

//#define __CCB_VS_IMP__
#include "../src/ccbase/vs/view_string.h"


int vs_test_true(int result, const char* description) {
    printf("Test True : %s\n", description);
    assert(result && "EXPECTED TRUE");
    return 1;
}

int vs_test_false(int result, const char* description) {
    printf("Test False: %s\n", description);
    assert(!result && "EXPECTED FALSE");
    return 1;
}

int main() {
    
    // test eq
    vs_test_true(ccb_vs_eq(ccb_cst2vst("a test"), 
                           ccb_cst2vst("a test")), 
                 "Test vs_eq true");
    
    vs_test_false(ccb_vs_eq(ccb_cst2vst("a test"), 
                            ccb_cst2vst("a tesr")), 
                "Test vs_eq false");

    // test head
    vs_test_true(ccb_vs_head(ccb_cst2vst("a test123"), 
                             ccb_cst2vst("a test")),
                 "Test vs_head true");
    
    vs_test_false(ccb_vs_head(ccb_cst2vst("a tesr145"), 
                              ccb_cst2vst("a test")), 
                  "Test vs_head false");

    // test tail
    vs_test_true(ccb_vs_tail(ccb_cst2vst("a test123"), 
                             ccb_cst2vst("t123")),
                 "Test vs_tail true");
    
    vs_test_false(ccb_vs_tail(ccb_cst2vst("a tesr145"), 
                              ccb_cst2vst("t123")), 
                 "Test vs_tail false");

    // test in
    vs_test_true(ccb_vs_in(ccb_cst2vst("a test1235z6ad1d51a65"), 
                           ccb_cst2vst("t123")),
                 "Test vs_in true");
    
    vs_test_false(ccb_vs_in(ccb_cst2vst("a tesr1451d5za151az56d1z"), 
                            ccb_cst2vst("t123")), 
                 "Test vs_in false");

    vs_test_true(ccb_vs_in(ccb_cst2vst("a test1235z6ad1d51a65"), 
                           ccb_cst2vst("a test")),
                 "Test vs_in true in front");
    
    vs_test_true(ccb_vs_in(ccb_cst2vst("a test1235z6ad1d51a65"), 
                           ccb_cst2vst("1a65")),
                 "Test vs_in true in tail");

    vs_test_true(ccb_vs_in(ccb_cst2vst("a test1235z6ad1d51a65"), 
                           ccb_cst2vst("5")),
                 "Test vs_in true 1ch");
    
    // test splits
    {
        const ccb_vs a = ccb_cst2vst("test");
        ccb_vs_split result = ccb_vs_split_char(a, 's');

        vs_test_true(result.found, "split true: found split");
        vs_test_true(ccb_vs_eq(result.front, ccb_cst2vst("te")), "split true: front ok");
        vs_test_true(ccb_vs_eq(result.back, ccb_cst2vst("t")), "split true: back ok");
    }

    {
        const ccb_vs a = ccb_cst2vst("test");
        ccb_vs_split result = ccb_vs_split_char(a, 'q');

        vs_test_false(result.found, "split false: found split"); 
    }

    {
        const ccb_vs a = ccb_cst2vst("test");
        char split_chars[] = " \n\tas";
        ccb_vs_split result = ccb_vs_split_set(a, split_chars, sizeof(split_chars)-1);

        vs_test_true(result.found, "split set true: found split chars");
        vs_test_true(ccb_vs_eq(result.front, ccb_cst2vst("te")), "split set true: front ok");
        vs_test_true(ccb_vs_eq(result.back, ccb_cst2vst("t")), "split set true: back ok");
    }

    {
        const ccb_vs a = ccb_cst2vst("test");
        char split_chars[] = " \n\taq";
        ccb_vs_split result = ccb_vs_split_set(a, split_chars, sizeof(split_chars)-1);

        vs_test_false(result.found, "split set false: found split chars");
    }

    {
        const ccb_vs a = ccb_cst2vst(" a certain test");
        const ccb_vs b = ccb_cst2vst("certain");
        ccb_vs_split result = ccb_vs_split_string(a, b);

        vs_test_true(result.found, "split string true: found split chars");
        vs_test_true(ccb_vs_eq(result.front, ccb_cst2vst(" a ")), "split string true: front ok");
        vs_test_true(ccb_vs_eq(result.back, ccb_cst2vst(" test")), "split string true: back ok");
    }

    {
        const ccb_vs a = ccb_cst2vst("a test or not a test");
        const ccb_vs b = ccb_cst2vst("certain");
        ccb_vs_split result = ccb_vs_split_string(a, b);

        vs_test_false(result.found, "split string false: found split chars");
    }

    {
        const ccb_vs a = ccb_cst2vst(" a certain test");
        const ccb_vs b = ccb_cst2vst("test");
        ccb_vs_split result = ccb_vs_split_string(a, b);

        vs_test_true(result.found, "split string tail: found split chars");
        vs_test_true(ccb_vs_eq(result.front, ccb_cst2vst(" a certain ")), "split string tail: front ok");
        vs_test_true(ccb_vs_eq(result.back, ccb_cst2vst("")), "split string tail: back ok");
    }

    // test trim left and trim right
    

    return 0;
}