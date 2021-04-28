/* Dummy implementation of register_txn (calls commit immediately). */

#include <cstddef>

#include "extern_cc.h"
#include "query.h"
#include "row.h"
#include "test.h"
#include "wl.h"

#if EXTERN_CC_IMPL == EXTERN_CC_DUMMY

// This is equivalent to no concurrency control.
void register_txn(txn_man* m_txn, base_query* m_query, row_t* reads[],
                  row_t* writes[], size_t num_reads, size_t num_writes) {
    if (WORKLOAD == TEST) {
        if (g_test_case == READ_WRITE) {
            ((TestTxnMan*)m_txn)->commit_txn(g_test_case, 0, reads, writes);
            ((TestTxnMan*)m_txn)->commit_txn(g_test_case, 1, reads, writes);
            printf("READ_WRITE TEST PASSED\n");
        } else if (g_test_case == CONFLICT) {
            ((TestTxnMan*)m_txn)->commit_txn(g_test_case, 0, reads, writes);
        }
    } else {
        m_txn->commit_txn(m_query, reads, writes);
    }
    delete[] reads;
    delete[] writes;
}

#endif
