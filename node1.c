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
  int costs[4][4];
} dt1;


extern void read1(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt1.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt1.costs[0][0], &dt1.costs[1][1], &dt1.costs[2][2], &dt1.costs[3][3]);
}

extern void rtinit1() 
{
  /* TODO */
  int sourceID = 1;
  int destID[3] ={0,2,3};
  for(int i=0;i<4;i++){
    dt1.costs[i][sourceID] = dt1.costs[i][i];
  }

  struct rtpkt pk1, pk2, pk3;
  pk1.sourceid = sourceID;
  pk1.destid = destID[0];
  pk2.sourceid = sourceID;
  pk2.destid = destID[1];
  pk3.sourceid = sourceID;
  pk3.destid = destID[2];

  for(int i=0;i<4;i++){
    pk1.mincost[i] = dt1.costs[i][sourceID];
    pk2.mincost[i] = dt1.costs[i][sourceID];
    pk3.mincost[i] = dt1.costs[i][sourceID];
  }

  //If neighbor, send
  if(dt1.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
  if(dt1.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
  if(dt1.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);
}

extern void rtupdate1(struct rtpkt *rcvdpkt)
{
  /* TODO */
  int changed = 0;
  int min_array[4];
  for(int i=0;i<4;i++){
    int min;
    min = dt1.costs[i][0];
    if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
    if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
    if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
    min_array[i] = min;
  }
  for(int i=0;i<4;i++){
    dt1.costs[i][rcvdpkt->sourceid]  = rcvdpkt->mincost[i] + dt1.costs[rcvdpkt->sourceid][rcvdpkt->destid];
    if(dt1.costs[i][rcvdpkt->sourceid] > 999) dt1.costs[i][rcvdpkt->sourceid] = 999;
  }

  for(int i=0;i<4;i++){
      int min;
      min = dt1.costs[i][0];
      if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
      if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
      if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
      if(min_array[i] != min) changed = 1;
  }

  if(changed){
    int sourceID = 1;
    int destID[3] ={0,2,3};
    struct rtpkt pk1, pk2, pk3;

    pk1.sourceid = sourceID;
    pk1.destid = destID[0];
    pk2.sourceid = sourceID;
    pk2.destid = destID[1];
    pk3.sourceid = sourceID;
    pk3.destid = destID[2];


    for(int i=0;i<4;i++){
      int min;
      min = dt1.costs[i][0];
      if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
      if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
      if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
      pk1.mincost[i] = min;
      pk2.mincost[i] = min;
      pk3.mincost[i] = min;
    }

    //If neighbor, send
    if(dt1.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
    if(dt1.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
    if(dt1.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);
    
  }
  /*
  printf("1\n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%d ", dt1.costs[i][j]);
    }
    printf("\n");
  }*/
}

void printdt1(void)  
{
  struct distance_table *dtptr = &dt1;

  printf("             via   \n");
  printf("   D1 |    0     2 \n");
  printf("  ----|-----------\n");
  printf("     0|  %3d   %3d\n",dtptr->costs[0][0], dtptr->costs[0][2]);
  printf("dest 2|  %3d   %3d\n",dtptr->costs[2][0], dtptr->costs[2][2]);
  printf("     3|  %3d   %3d\n",dtptr->costs[3][0], dtptr->costs[3][2]);

}

extern void linkhandler1(int linkid, int newcost)
/* called when cost from 1 to linkid changes from current value to newcost*/
/* You can leave this routine empty if you're an undergrad. If you want */
/* to use this routine, you'll need to change the value of the LINKCHANGE */
/* constant definition in prog3.c from 0 to 1 */
{
  /* TODO */
  int changed = 0;
  int min_array[4];
  for(int i=0;i<4;i++){
    int min;
    min = dt1.costs[i][0];
    if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
    if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
    if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
    min_array[i] = min;
  }

  int dif = newcost - dt1.costs[linkid][linkid];
  for(int i=0;i<4;i++){
    if(dt1.costs[i][linkid] != 999){
      dt1.costs[i][linkid] += dif;
    }
  }
  //dt1.costs[linkid][linkid] = newcost;
  dt1.costs[linkid][1] = newcost;

  for(int i=0;i<4;i++){
    int min;
    min = dt1.costs[i][0];
    if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
    if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
    if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
    if(min_array[i] != min) changed = 1;
  }

  if(changed){
    int sourceID = 1;
    int destID[3] ={0,2,3};
    struct rtpkt pk1, pk2, pk3;

    pk1.sourceid = sourceID;
    pk1.destid = destID[0];
    pk2.sourceid = sourceID;
    pk2.destid = destID[1];
    pk3.sourceid = sourceID;
    pk3.destid = destID[2];


    for(int i=0;i<4;i++){
      int min;
      min = dt1.costs[i][0];
      if(dt1.costs[i][1]<min) min = dt1.costs[i][1];
      if(dt1.costs[i][2]<min) min = dt1.costs[i][2];
      if(dt1.costs[i][3]<min) min = dt1.costs[i][3];
      pk1.mincost[i] = min;
      pk2.mincost[i] = min;
      pk3.mincost[i] = min;
    }

    //If neighbor, send
    if(dt1.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
    if(dt1.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
    if(dt1.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);

  }

}

extern void print_min_cost1()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt1.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt1.costs[i][j] ? dt1.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 1, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}