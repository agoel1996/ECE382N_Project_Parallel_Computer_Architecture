// iu.h
//   by Derek Chiou
//      March 4, 2007
// 

// STUDENTS: YOU ARE ALLOWED TO MODIFY THIS FILE, BUT YOU SHOULDN'T NEED TO MODIFY MUCH, IF ANYTHING.
// for 382N-10

#ifndef IU_H
#define IU_H
#include "types.h"
#include "my_fifo.h"
#include "cache.h"
#include "network.h"

typedef struct 
{
  int top;
  int left;
  int bottom;
  int right;
  int min;
}ROUTE_T_ENTRY;


class iu_t {
  int node;

  int local_accesses;
  int global_accesses;

  data_t mem[MEM_SIZE];
  data_t dir_mem[DIR_MEM_SIZE];

  cache_t *cache;
  network_t *net;

  bool proc_cmd_p;
  proc_cmd_t proc_cmd;

  bool proc_cmd_writeback_p;
  proc_cmd_t proc_cmd_writeback;

  // processor side
  bool process_proc_request(proc_cmd_t proc_cmd);

  // network side
  bool process_net_request(proc_cmd_t pc, int src);
  bool update_routing_table(net_cmd_t net_cmd);

  // Adjacent Nodes
  int top, left, bottom, right;

  // Routing Table
  ROUTE_T_ENTRY ROUTE_TABLE[MESH_DIM*MESH_DIM];
  int NUM_REG[5]; 

 public:
  iu_t(int __node);

  void bind(cache_t *c, network_t *n);

  void advance_one_cycle();
  void print_stats();

  // processor side
  bool from_proc(proc_cmd_t pc);
  bool from_proc_writeback(proc_cmd_t pc);
  
  // network side
  bool from_net(net_cmd_t nc);

  //init function for routing table
  void routingTableInit();
  void update_min();

  int node_x;
  int node_y;
};
#endif
