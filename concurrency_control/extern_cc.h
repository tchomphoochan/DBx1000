/* External concurrency management for DBx1000.
 *
 * This header contains declarations of functions that should be implemented
 * (and linked into the binary) by an external concurrency manager when the
 * EXTERN_CC algorithm is used.
 */

#ifndef _EXTERN_CC_H_
#define _EXTERN_CC_H_

#include <cstddef>
#include <vector>

#include "query.h"
#include "row.h"
#include "wl.h"

extern UInt32 g_extern_thread_cnt;
extern std::vector<pthread_t> g_extern_p_thds;

// must ensure that txn_man::commit_txn is called when transaction is ready.
void register_txn(txn_man* m_txn, base_query* m_query, row_t* reads[],
                  row_t* writes[], size_t num_reads, size_t num_writes);

#endif
