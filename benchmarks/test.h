#ifndef _TEST_H_
#define _TEST_H_

#include "global.h"
#include "txn.h"
#include "wl.h"

class TestWorkload : public workload
{
public:
	RC init();
	RC init_table();
	RC init_schema(string schema_file);
	RC get_txn_man(txn_man *& txn_manager, thread_t * h_thd);
	void summarize();
	void tick() { time = get_sys_clock(); };
	INDEX * the_index;
	table_t * the_table;
private:
	uint64_t time;
};

class TestTxnMan : public txn_man 
{
public:
	void init(thread_t * h_thd, workload * h_wl, uint64_t part_id); 
	RC run_txn(int type, int access_num);
	RC run_txn(base_query * m_query) { assert(false); };
	RC commit_txn(base_query * query, row_t * reads[], row_t * writes[]) { assert(false); };
	RC commit_txn(int type, int access_num, row_t * reads[], row_t * writes[]);
private:
	RC testReadwrite(int access_num);
	RC testConflict(int access_num);
	RC commitReadwrite(int access_num, row_t * reads[], row_t * writes[]);
	RC commitConflict(int access_num, row_t * reads[], row_t * writes[]);
	
	TestWorkload * _wl;
};

#endif
