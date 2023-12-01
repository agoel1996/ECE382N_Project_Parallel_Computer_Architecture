// types.h
//   by Derek Chiou
//      Oct. 6, 2023
// 

// STUDENTS: YOU ARE NOT ALLOWED TO MODIFY THIS FILE.
// for 382N-10

#ifndef TYPES_H
#define TYPES_H

const int ADDR_SIZE = 32;          // 32b address space
const int CACHE_LINE_SIZE = 8;     // number of words in cacheline,  needs to be consistent with LG_CACHE_LINE_SIZE
const int LG_CACHE_LINE_SIZE = 3;  // needs to be consistent with CACHE_LINE_SIZE
const int MEM_SIZE = 1024;
const int DIR_MEM_SIZE = 256;
const int NUM_PRIORITIES = 5;

const unsigned int NETWORK_LATENCY = 20;
extern int CONTROL_LATENCY;
extern int NUM_SRC;

const int MESH_DIM = 8;

typedef unsigned int uint;

typedef uint address_t;      // cannot be addr_t because cygwin defines
typedef uint address_tag_t;
typedef enum {INVALID, SHARED, EXCLUSIVE, MODIFIED} permit_tag_t;
typedef enum {READ, WRITEBACK, INVALIDATE, NO_BUSOP} busop_t;
typedef enum {PRI0, PRI1, PRI2, PRI3, PRI4} pri_t;
typedef int replacement_t;
typedef int bus_tag_t;

typedef int data_t[CACHE_LINE_SIZE];

typedef struct {
  int num_procs;
  int num_cycles;
  int numSrc;
  int numPkts;
  int cntrLatency;
  int test;
  int verbose;
} args_t;

extern args_t args;

// forward declarations
class iu_t;
class cache_t;
class network_t;

typedef struct {
  bool hit_p;
  bool retry_p;
} response_t;

typedef struct {
  int fDest;                 // Final destination
  int fsrc;
  int pktID;                 // Packet ID
  int flitID;
  int nFlits;
  data_t data;
  address_t addr;
  busop_t busop;
  permit_tag_t permit_tag;
} proc_cmd_t;

typedef struct {
  bool valid_p;
  int dest;
  int src;

  proc_cmd_t proc_cmd;
} net_cmd_t;

extern int cur_cycle;
extern int cache_line_size;
extern int count_pkt[MESH_DIM*MESH_DIM][MESH_DIM*MESH_DIM];
extern int latency[MESH_DIM*MESH_DIM][MESH_DIM*MESH_DIM];
extern int shortestPath[MESH_DIM*MESH_DIM][MESH_DIM*MESH_DIM];

#endif
