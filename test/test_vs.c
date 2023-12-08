#include <stdio.h>
#include <assert.h>

#define __CCB_VS_IMP__
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
    

    return 0;
}