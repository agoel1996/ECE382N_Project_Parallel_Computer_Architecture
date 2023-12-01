// test.cpp
//   Derek Chiou
//     Oct. 8, 2023


#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "generic_error.h"
#include "helpers.h"
#include "cache.h"
#include "iu.h"
#include "test.h"
#include <vector>
#include <algorithm>

using namespace std;
int dest;
vector <int> uniqueSrc;
int rand_src_gen();
proc_t::proc_t(int __p) {
  proc = __p;
  init();
}

void proc_t::init() {
  response.retry_p = false;
  ld_p = false;
}

void proc_t::bind(cache_t *c) {
  cache = c;
}


// ***** FYTD ***** 

// this is just a simple random test.  I'm not giving
// you any more test cases than this.  You will be tested on the
// correctness and performance of your solution.

extern args_t args;
extern int addr_range;
extern cache_t **caches;
extern iu_t **ius;

test_args_t test_args;

void init_test() {
  for(int packet_count = 0; packet_count < NUM_SRC; packet_count++) {
    rand_src_gen();
  }
  if(args.test != 4) {
    dest = rand() % args.num_procs;
    while(find(uniqueSrc.begin(), uniqueSrc.end(), dest) != uniqueSrc.end())
      dest = rand() % args.num_procs;
  }
  test_args.addr_range = 512;
}

void finish_test() {
  double hr;
  double avg_latency = 0;
  double temp1;
  double temp2;
  int final_count = 0;
  for(int src = 0; src < MESH_DIM*MESH_DIM; src ++) {
    for(int dest = 0; dest < MESH_DIM*MESH_DIM; dest++){
      if(count_pkt[src][dest] != 0) {
        final_count++;
        temp1 = (latency[src][dest])/(count_pkt[src][dest]);
        temp2 = temp1/shortestPath[src][dest];
        avg_latency += temp2;
        //debug: printf("Src = %d, Dest = %d, Average Latency = %lf\n",src,dest,temp2);
        //if(src == 7 && dest == 10)
          //debug: printf("Latency = %lf, shortestPath = %lf\n",latency[src][dest], shortestPath[src][dest]);
      } 
    }
  }
  printf("%lf\n", avg_latency/final_count);
  //debug: printf("Avg Latency of system = %lf\n", avg_latency/final_count);
  //debug: printf("passed\n");
}

bool sent = false;
void proc_t::advance_one_cycle() {
  int data;

  switch (args.test) {
  case 0:
    if(proc == 0 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 0, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 2 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 2, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 1 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 1, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 4 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 4, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }

    if(proc == 5 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 5, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 6 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 6, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }

    if(proc == 7 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 7, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 3 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){56, 3, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }



    break;
////////////////////////////////////////////////////////////////////////////////////////
  case 1:
    //if(proc == 0 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
    //  proc_cmd_t proc_cmd = (proc_cmd_t){56, 0, cur_cycle, 1, 1};
    //  ius[proc]->from_proc(proc_cmd);
    //  //sent = true;
    //}
    //if(proc == 2 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
    //  proc_cmd_t proc_cmd = (proc_cmd_t){58, 2, cur_cycle, 1, 1};
    //  ius[proc]->from_proc(proc_cmd);
    //  //sent = true;
    //}
    //if(proc == 1 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
    //  proc_cmd_t proc_cmd = (proc_cmd_t){57, 1, cur_cycle, 1, 1};
    //  ius[proc]->from_proc(proc_cmd);
    //  //sent = true;
    //}
    //if(proc == 4 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
    //  proc_cmd_t proc_cmd = (proc_cmd_t){60, 4, cur_cycle, 1, 1};
    //  ius[proc]->from_proc(proc_cmd);
    //  //sent = true;
    //}

    if(proc == 1 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){6, 1, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 2 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){5, 2, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }

    if(proc == 3 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){4, 3, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 0 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){7, 0, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    break;

//////////////////////////////////////////////////////////////////////////////
  case 2:
    if(proc == 8 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){args.num_procs, 8, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 9 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){11, 9, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 3 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){57, 3, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 15 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){11, 15, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }

    if(proc == 0 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){args.num_procs, 0, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 57 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){args.num_procs,57, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }

    if(proc == 43 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){62, 43, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }
    if(proc == 10 && sent == false && cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){19, 10, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
      //sent = true;
    }



    break;
//////////////////////////////////////////////////////////////////////////////
  case 3:
  //int dest;
  if(find(uniqueSrc.begin(), uniqueSrc.end(), proc) != uniqueSrc.end()) {
    //dest = rand() % args.num_procs;
    //while(dest == proc) {
    //  dest = rand() % args.num_procs;
    //}
    if(cur_cycle > 1000 && cur_cycle < 1100){
      proc_cmd_t proc_cmd = (proc_cmd_t){dest , proc, cur_cycle, 1, 1};
      ius[proc]->from_proc(proc_cmd);
    }
  }
    break;
  
  case 4:
    if(find(uniqueSrc.begin(), uniqueSrc.end(), proc) != uniqueSrc.end()) {
      int localDest;
      localDest = rand() % args.num_procs;
      while(localDest == proc) {
        localDest = rand() % args.num_procs;
      }
      if(cur_cycle > 1000 && cur_cycle < (1000+args.numPkts)){
        proc_cmd_t proc_cmd = (proc_cmd_t){localDest , proc, cur_cycle, 1, 1};
        ius[proc]->from_proc(proc_cmd);
      }
    }
    break;

  default:
    ERROR("don't know this test case");
  }
}

int rand_src_gen() {
  int src = rand()%args.num_procs;
  while(find(uniqueSrc.begin(), uniqueSrc.end(), src) != uniqueSrc.end()) {
    src = rand()%args.num_procs;
  }
  uniqueSrc.push_back(src);
  return src;
}
