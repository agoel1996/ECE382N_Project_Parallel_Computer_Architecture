
// iu.cpp
//   by Derek Chiou
//      March 4, 2007
//      modified Oct. 8, 2023
// 

// STUDENTS: YOU ARE EXPECTED TO MAKE MOST OF YOUR MODIFICATIONS IN THIS FILE.
// for 382N-10

#include <climits>
#include "types.h"
#include "helpers.h"
#include "my_fifo.h"
#include "cache.h"
#include "iu.h"
#include "test.h"
using namespace std;

iu_t::iu_t(int __node) {
  node = __node;
  for (int i = 0; i < MEM_SIZE; ++i) 
    for (int j = 0; j < CACHE_LINE_SIZE; ++j)
      mem[i][j] = 0;

  proc_cmd_p = false;
  proc_cmd_writeback_p = false;
  
  top    = (node-MESH_DIM)<0                       ? -1 : node-MESH_DIM;
  left   = (node%MESH_DIM)==0                      ? -1 : node-1;
  bottom = (node+MESH_DIM)>((MESH_DIM*MESH_DIM)-1) ? -1 : node+MESH_DIM;
  right  = (node%MESH_DIM)==(MESH_DIM-1)           ? -1 : node+1;
  for(int n = 0; n< MESH_DIM*MESH_DIM; n++) {
    if(n == node)
      ROUTE_TABLE[n] = (ROUTE_T_ENTRY){0,0,0,0,0};
    else
      ROUTE_TABLE[n] = (ROUTE_T_ENTRY){INT_MAX,INT_MAX,INT_MAX,INT_MAX, INT_MAX};
  }
  for(int i=0; i<5;i++){
    NUM_REG[i] = NETWORK_LATENCY;
  }
  for(int i = 0; i < MESH_DIM*MESH_DIM; i++) {
    for(int t = 0; t < MESH_DIM*MESH_DIM; t++) {
      count_pkt[i][t] = 0;
      latency[i][t] = 0;
    }
  }
  node_x = node%MESH_DIM;
  node_y = node/MESH_DIM;
}

void iu_t::routingTableInit(){
  net_cmd_t net_cmd;
  net_cmd.src = node;
  if(top != -1) {
    net_cmd.proc_cmd.fDest = top; // changed min dist for top 
    net_cmd.proc_cmd.data[0] = NETWORK_LATENCY;
    ROUTE_TABLE[top].top = NETWORK_LATENCY;
    ROUTE_TABLE[top].min = NETWORK_LATENCY;
    if(bottom != -1) {
      net_cmd.dest = bottom;
      net->to_net(node, PRI0, net_cmd);
    }
    if(right != -1) {
      net_cmd.dest = right;
      net->to_net(node, PRI0, net_cmd);
    }
    if(left != -1) {
      net_cmd.dest = left;
      net->to_net(node, PRI0, net_cmd);
    }
    
  }
  if(bottom != -1) {
    net_cmd.proc_cmd.fDest = bottom; // changed min dist for bottom 
    net_cmd.proc_cmd.data[0] = NETWORK_LATENCY;
    ROUTE_TABLE[bottom].bottom = NETWORK_LATENCY;
    ROUTE_TABLE[bottom].min = NETWORK_LATENCY;
    if(top != -1) {
      net_cmd.dest = top;
      net->to_net(node, PRI0, net_cmd);
    }
    if(right != -1) {
      net_cmd.dest = right;
      net->to_net(node, PRI0, net_cmd);
    }
    if(left != -1) {
      net_cmd.dest = left;
      net->to_net(node, PRI0, net_cmd);
    }
  }
  if(right != -1) {
    net_cmd.proc_cmd.fDest = right; // changed min dist for right 
    net_cmd.proc_cmd.data[0] = NETWORK_LATENCY;
    ROUTE_TABLE[right].right = NETWORK_LATENCY;
    ROUTE_TABLE[right].min = NETWORK_LATENCY;
    if(bottom != -1) {
      net_cmd.dest = bottom;
      net->to_net(node, PRI0, net_cmd);
    }
    if(top != -1) {
      net_cmd.dest = top;
      net->to_net(node, PRI0, net_cmd);
    }
    if(left != -1) {
      net_cmd.dest = left;
      net->to_net(node, PRI0, net_cmd);
    }
  }
  if(left != -1) {
    net_cmd.proc_cmd.fDest = left; // changed min dist for left 
    net_cmd.proc_cmd.data[0] = NETWORK_LATENCY;
    ROUTE_TABLE[left].left = NETWORK_LATENCY;
    ROUTE_TABLE[left].min = NETWORK_LATENCY;
    if(bottom != -1) {
      net_cmd.dest = bottom;
      net->to_net(node, PRI0, net_cmd);
    }
    if(right != -1) {
      net_cmd.dest = right;
      net->to_net(node, PRI0, net_cmd);
    }
    if(top != -1) {
      net_cmd.dest = top;
      net->to_net(node, PRI0, net_cmd);
    }
    
  }
}
void iu_t::bind(cache_t *c, network_t *n) {
  cache = c;
  net = n;
}

void iu_t::advance_one_cycle() {
  while (net->from_net_p(node, PRI0)) { 
    update_routing_table(net->from_net(node, PRI0));
  }
  if (net->from_net_p(node, PRI1)) {
    process_net_request(net->from_net(node, PRI1).proc_cmd,bottom);
  }
  if (net->from_net_p(node, PRI2)) {
    process_net_request(net->from_net(node, PRI2).proc_cmd, left);
  }
  if (net->from_net_p(node, PRI3)) {
    process_net_request(net->from_net(node, PRI3).proc_cmd, top);
  }
  if (net->from_net_p(node, PRI4)) {
    process_net_request(net->from_net(node, PRI4).proc_cmd, right);
  }
  if (proc_cmd_p) {
    process_net_request(proc_cmd, node);
    proc_cmd_p = false;
  }
  update_min();
  if(cur_cycle == 1000) {
    for(int dest = 0; dest < MESH_DIM*MESH_DIM; dest++) {
      shortestPath[node][dest] = min(min(ROUTE_TABLE[dest].top, ROUTE_TABLE[dest].bottom), min(ROUTE_TABLE[dest].right, ROUTE_TABLE[dest].left));
    }
  }
  //if(node == 9) {
  //  //debug: printf("%d:, Node9 Route table update: ROUTE_TABLE[10].top = %d, ROUTE_TABLE[10].right = %d\n",cur_cycle, ROUTE_TABLE[10].top, ROUTE_TABLE[10].right); 
  //}
} 

// processor side

// this interface method buffers a non-writeback request from the processor, returns true if cannot complete
bool iu_t::from_proc(proc_cmd_t pc) {
  if (!proc_cmd_p) {
    proc_cmd_p = true;
    proc_cmd = pc;

    return(false);
  } else {
    return(true);
  }
}

// this interface method buffers a writeback request from the processor, returns true if cannot complete
bool iu_t::from_proc_writeback(proc_cmd_t pc) {
  if (!proc_cmd_writeback_p) {
    proc_cmd_writeback_p = true;
    proc_cmd_writeback = pc;

    return(false);
  } else {
    return(true);
  }
}

bool iu_t::process_proc_request(proc_cmd_t pc) {
  return 0;
}


// receive a net request
bool iu_t::process_net_request(proc_cmd_t pc, int src) {
  //proc_cmd_t pc = net_cmd.proc_cmd;
  int fDest = pc.fDest;
  int fDest_x = fDest%MESH_DIM;
  int fDest_y = fDest/MESH_DIM;
  int dir_x = (fDest_x == node_x) ? 0: (fDest_x > node_x) ? 1 : -1; //0 -> no change, 1-> right, -1->left
  int dir_y = (fDest_y == node_y) ? 0: (fDest_y > node_y) ? 1 : -1; //0 -> no change, 1-> bottom, -1->top

  if(fDest != node){
    int cur_min_val;
    int send_node;
    pri_t send_pri;
    int leftVal   = left == -1   ? INT_MAX : ROUTE_TABLE[fDest].left+max(net->from_net_fifos[PRI4][left]->num, NETWORK_LATENCY);
    int rightVal  = right == -1  ? INT_MAX : ROUTE_TABLE[fDest].right+max(net->from_net_fifos[PRI2][right]->num, NETWORK_LATENCY); 
    int topVal    = top == -1    ? INT_MAX : ROUTE_TABLE[fDest].top+max(net->from_net_fifos[PRI1][top]->num, NETWORK_LATENCY); 
    int bottomVal = bottom == -1 ? INT_MAX : ROUTE_TABLE[fDest].bottom+max(net->from_net_fifos[PRI3][bottom]->num, NETWORK_LATENCY); 
    if(dir_x == -1 && dir_y == -1) {
      //min of left and top
      cur_min_val = min(leftVal,topVal);
      send_node   = (leftVal == cur_min_val)? left: top;
      send_pri    = (leftVal == cur_min_val)? PRI4: PRI1;
    } 
    else if(dir_x == -1 && dir_y == 0) {
      //go left
      send_node = left;
      send_pri  = PRI4;
    } 
    else if(dir_x == -1 && dir_y == 1) {
      //min of left and bottom
      cur_min_val = min(leftVal,bottomVal);
      send_node   = (leftVal == cur_min_val)? left: bottom;
      send_pri    = (leftVal == cur_min_val)? PRI4: PRI3;
    } 
    else if(dir_x == 0 && dir_y == -1) {
      //go top
      send_node = top;
      send_pri  = PRI1;
    } 
    else if(dir_x == 0 && dir_y == 1) {
      //go bottom
      send_node = bottom;
      send_pri  = PRI3;
    } 
    else if(dir_x == 0 && dir_y == 0) {
      //debug: printf("Derek says Happy Black Friday today!!! :) \n");
    } 
    else if(dir_x == 1 && dir_y == -1) {
      //min of right and top
      cur_min_val = min(rightVal, topVal);
      send_node   = (rightVal == cur_min_val)? right: top;
      send_pri    = (rightVal == cur_min_val)? PRI2: PRI1;
    } 
    else if(dir_x == 1 && dir_y == 0) {
      //go right
      send_node = right;
      send_pri  = PRI2;
    } 
    else if(dir_x == 1 && dir_y == 1) {
      //min of right and bottom
      cur_min_val = min(rightVal, bottomVal);
      send_node   = (rightVal == cur_min_val)? right: bottom;
      send_pri    = (rightVal == cur_min_val)? PRI2: PRI3;
    } 
    
    net_cmd_t net_cmd_send;
    net_cmd_send.dest = send_node;
    net_cmd_send.src  = node;
    net_cmd_send.proc_cmd = pc;
    net->to_net(node, send_pri, net_cmd_send);
      //debug: printf("%d: ARUN: Packet %d of Src: %d forwarded from %d to Dest: %d\n", cur_cycle, pc.pktID, pc.fsrc, node, fDest);
    }
    else{
      //process the request
      //debug: printf("%d: ARUN: Packet %d of Src: %d Received at Dest: %d\n", cur_cycle, pc.pktID, pc.fsrc, node);
      count_pkt[pc.fsrc][pc.fDest]++;
      latency[pc.fsrc][pc.fDest] += cur_cycle - pc.pktID;
      //if(pc.fsrc == 7)
        //debug: printf("cur_cycle = %d, pktID = %d\n", cur_cycle, pc.pktID);
    }
    return false;
}

bool iu_t::update_routing_table(net_cmd_t net_cmd) {
  proc_cmd_t pc = net_cmd.proc_cmd;
  int dest = net_cmd.dest;
  int fDest = net_cmd.proc_cmd.fDest;
  int src = net_cmd.src;
  if(fDest == node) return false;
  pri_t pri = (src == top)? PRI1: (src == right)? PRI2: (src == bottom)? PRI3: PRI4;  
  
  int num_diff = NUM_REG[pri] - max(net->from_net_fifos[pri][src]->num, NETWORK_LATENCY);
    if(num_diff != 0){
      for(int i=0; i<MESH_DIM*MESH_DIM; i++){
        if(src == top && ROUTE_TABLE[i].top != INT_MAX){ ROUTE_TABLE[i].top -= num_diff; }
        if(src == bottom && ROUTE_TABLE[i].bottom != INT_MAX){ ROUTE_TABLE[i].bottom -= num_diff; }
        if(src == right && ROUTE_TABLE[i].right != INT_MAX){ ROUTE_TABLE[i].right -= num_diff; }
        if(src == left && ROUTE_TABLE[i].left != INT_MAX){ ROUTE_TABLE[i].left -= num_diff; }
      }
    }
  //NUM_REG[pri] = net->from_net_fifos[pri][src]->num;
  NUM_REG[pri] = max(net->from_net_fifos[pri][src]->num, NETWORK_LATENCY);
  int data =  net_cmd.proc_cmd.data[0] + NUM_REG[pri];
  //int data =  net_cmd.proc_cmd.data[0] + NETWORK_LATENCY + NUM_REG[pri];
  int orig_min = ROUTE_TABLE[fDest].min;
  if(src == top) ROUTE_TABLE[fDest].top = data;
  else if(src == bottom) ROUTE_TABLE[fDest].bottom = data;
  else if(src == right)ROUTE_TABLE[fDest].right = data;
  else if(src == left) ROUTE_TABLE[fDest].left = data;
  return false;
}

void iu_t::update_min() {
  for(int dest=0; dest<MESH_DIM*MESH_DIM; dest++){
    int min_val = std::min(std::min(ROUTE_TABLE[dest].top,ROUTE_TABLE[dest].bottom), std::min(ROUTE_TABLE[dest].right,ROUTE_TABLE[dest].left));
    if(min_val != ROUTE_TABLE[dest].min){
      ROUTE_TABLE[dest].min = min_val;
      net_cmd_t net_cmd;
      net_cmd.src = node;
      net_cmd.proc_cmd.fDest = dest;
      net_cmd.proc_cmd.data[0] = min_val;
      if(top != -1)   {net_cmd.dest = top;     net->to_net(node, PRI0, net_cmd);}
      if(bottom != -1){net_cmd.dest = bottom;  net->to_net(node, PRI0, net_cmd);}
      if(right != -1) {net_cmd.dest = right;   net->to_net(node, PRI0, net_cmd);}
      if(left != -1)  {net_cmd.dest = left;    net->to_net(node, PRI0, net_cmd);}
    }
  }
}

void iu_t::print_stats() {
  //debug: printf("------------------------------\n");
  //debug: printf("%d: iu\n", node);
  
  //debug: printf("num local  accesses = %d\n", local_accesses);
  //debug: printf("num global accesses = %d\n", global_accesses);
}
