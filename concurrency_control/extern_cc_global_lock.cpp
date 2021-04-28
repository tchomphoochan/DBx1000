/* Simple implementation of register_txn (uses single global lock). */

#include <pthread.h>

#include <cstddef>
#include <vector>

#include "extern_cc.h"
#include "query.h"
#include "row.h"
#include "test.h"
#include "wl.h"

pthread_mutex_t global_lock;

typedef struct {
    txn_man* m_txn;
    base_query* m_query;
    row_t** reads;
    row_t** writes;
} txn_state;

void* lock_and_commit(void* arg) {
    txn_state* s = (txn_state*)arg;

    pthread_mutex_lock(&global_lock);

    if (WORKLOAD == TEST) {
        if (g_test_case == READ_WRITE) {
            ((TestTxnMan*)s->m_txn)->commit_txn(g_test_case, 0, s->reads, s->writes);
            ((TestTxnMan*)s->m_txn)->commit_txn(g_test_case, 1, s->reads, s->writes);
            printf("READ_WRITE TEST PASSED\n");
        } else if (g_test_case == CONFLICT) {
            ((TestTxnMan*)s->m_txn)->commit_txn(g_test_case, 0, s->reads, s->writes);
        }
    } else {
        s->m_txn->commit_txn(s->m_query, s->reads, s->writes);
    }

    pthread_mutex_unlock(&global_lock);

    delete[] s->reads;
    delete[] s->writes;
    delete s;

    return NULL;
}

#if EXTERN_CC_IMPL == EXTERN_CC_GL

UInt32 g_extern_thread_cnt = 0;
std::vector<pthread_t> g_extern_p_thds;
pthread_mutex_t g_extern_thread_lock;

void register_txn(txn_man* m_txn, base_query* m_query, row_t* reads[], row_t* writes[],
                  size_t num_reads, size_t num_writes) {
    pthread_mutex_lock(&g_extern_thread_lock);
    g_extern_thread_cnt++;
    pthread_mutex_unlock(&g_extern_thread_lock);

    pthread_t thr;
    pthread_create(&thr, NULL, lock_and_commit,
                   new txn_state{m_txn, m_query, reads, writes});

    pthread_mutex_lock(&g_extern_thread_lock);
    g_extern_p_thds.push_back(thr);
    pthread_mutex_unlock(&g_extern_thread_lock);
}

#endif
