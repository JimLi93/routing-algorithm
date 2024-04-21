#include <stdio.h>

struct rtpkt {
  int sourceid;       /* id of sending router sending this pkt */
  int destid;         /* id of router to which pkt being sent 
                         (must be an immediate neighbor) */
  int mincost[4];    /* min cost to node 0 ... 3 */
  };

extern int TRACE;
extern int YES;
extern int NO;
extern void tolayer2(struct rtpkt packet);

  struct distance_table 
{
  int costs[4][4] ;
} dt0;


extern void read0(FILE *file)
{

  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt0.costs[i][j] = 999;
    }
  }
  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt0.costs[0][0], &dt0.costs[1][1], &dt0.costs[2][2], &dt0.costs[3][3]);
}


extern void rtinit0() 
{
  /* TODO */
  int sourceID = 0;
  int destID[3] ={1,2,3};
  for(int i=0;i<4;i++){
    dt0.costs[i][sourceID] = dt0.costs[i][i];
  }

  struct rtpkt pk1, pk2, pk3;
  pk1.sourceid = sourceID;
  pk1.destid = destID[0];
  pk2.sourceid = sourceID;
  pk2.destid = destID[1];
  pk3.sourceid = sourceID;
  pk3.destid = destID[2];

  for(int i=0;i<4;i++){
    pk1.mincost[i] = dt0.costs[i][sourceID];
    pk2.mincost[i] = dt0.costs[i][sourceID];
    pk3.mincost[i] = dt0.costs[i][sourceID];
  }

  if(dt0.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
  if(dt0.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
  if(dt0.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);

  
}


extern void rtupdate0(struct rtpkt *rcvdpkt)
{
  /* TODO */
  
  int changed = 0;
  int min_array[4];
  for(int i=0;i<4;i++){
    int min;
    min = dt0.costs[i][0];
    if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
    if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
    if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
    min_array[i] = min;
  }
  for(int i=0;i<4;i++){
    dt0.costs[i][rcvdpkt->sourceid]  = rcvdpkt->mincost[i] + dt0.costs[rcvdpkt->sourceid][rcvdpkt->destid];
    if(dt0.costs[i][rcvdpkt->sourceid] > 999) dt0.costs[i][rcvdpkt->sourceid] = 999;
  }

  for(int i=0;i<4;i++){
      int min;
      min = dt0.costs[i][0];
      if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
      if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
      if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
      if(min_array[i] != min) changed = 1;
  }

  if(changed){
    int sourceID = 0;
    int destID[3] ={1,2,3};
    struct rtpkt pk1, pk2, pk3;

    pk1.sourceid = sourceID;
    pk1.destid = destID[0];
    pk2.sourceid = sourceID;
    pk2.destid = destID[1];
    pk3.sourceid = sourceID;
    pk3.destid = destID[2];


    for(int i=0;i<4;i++){
      int min;
      min = dt0.costs[i][0];
      if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
      if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
      if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
      pk1.mincost[i] = min;
      pk2.mincost[i] = min;
      pk3.mincost[i] = min;
    }

    //If neighbor, send
    if(dt0.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
    if(dt0.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
    if(dt0.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);
    
  }
  /*
  printf("0\n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%d ", dt0.costs[i][j]);
    }
    printf("\n");
  }
  */
}


 void printdt0(void) 
{
  struct distance_table *dtptr = &dt0;
  printf("                via     \n");
  printf("   D0 |    1     2    3 \n");
  printf("  ----|-----------------\n");
  printf("     1|  %3d   %3d   %3d\n", dtptr->costs[1][1],
           dtptr->costs[1][2], dtptr->costs[1][3]);
  printf("dest 2|  %3d   %3d   %3d\n", dtptr->costs[2][1],
           dtptr->costs[2][2], dtptr->costs[2][3]);
  printf("     3|  %3d   %3d   %3d\n", dtptr->costs[3][1],
           dtptr->costs[3][2], dtptr->costs[3][3]);
}

extern void linkhandler0(int linkid, int newcost)
/* called when cost from 0 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
  /* TODO */

  int changed = 0;
  int min_array[4];
  for(int i=0;i<4;i++){
    int min;
    min = dt0.costs[i][0];
    if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
    if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
    if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
    min_array[i] = min;
  }

  int dif = newcost - dt0.costs[linkid][linkid];
    for(int i=0;i<4;i++){
      if(dt0.costs[i][linkid] != 999){
        dt0.costs[i][linkid] += dif;
      }
    }
    //dt0.costs[linkid][linkid] = newcost;
    dt0.costs[linkid][0] = newcost;

  for(int i=0;i<4;i++){
      int min;
      min = dt0.costs[i][0];
      if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
      if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
      if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
      if(min_array[i] != min) changed = 1;
  }

  if(changed){
    int sourceID = 0;
    int destID[3] ={1,2,3};
    struct rtpkt pk1, pk2, pk3;

    pk1.sourceid = sourceID;
    pk1.destid = destID[0];
    pk2.sourceid = sourceID;
    pk2.destid = destID[1];
    pk3.sourceid = sourceID;
    pk3.destid = destID[2];


    for(int i=0;i<4;i++){
      int min;
      min = dt0.costs[i][0];
      if(dt0.costs[i][1]<min) min = dt0.costs[i][1];
      if(dt0.costs[i][2]<min) min = dt0.costs[i][2];
      if(dt0.costs[i][3]<min) min = dt0.costs[i][3];
      pk1.mincost[i] = min;
      pk2.mincost[i] = min;
      pk3.mincost[i] = min;
    }

    //If neighbor, send
    if(dt0.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
    if(dt0.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
    if(dt0.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);

  }
    
}

extern void print_min_cost0()
{
  int min_cost[4];

  for(int i = 0;i < 4;i++)
  {
    int min = dt0.costs[i][i];
    for(int j = 0;j < 4; j++)
    {
      min = min > dt0.costs[i][j] ? dt0.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 0, min_cost[0], min_cost[1], min_cost[2], min_cost[3]);
}

