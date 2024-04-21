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
} dt2;


extern void read2(FILE *file)
{
  //  Initialize distance_table to 999
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      dt2.costs[i][j] = 999;
    }
  }

  char *line = NULL;
  size_t len = 0;

  // Read file
  getline(&line, &len, file);
  sscanf(line, "%d %d %d %d", &dt2.costs[0][0], &dt2.costs[1][1], &dt2.costs[2][2], &dt2.costs[3][3]);

}

extern void rtinit2()
{
  /* TODO */
  int sourceID = 2;
  int destID[3] ={0,1,3};
  for(int i=0;i<4;i++){
    dt2.costs[i][sourceID] = dt2.costs[i][i];
  }

  struct rtpkt pk1, pk2, pk3;
  pk1.sourceid = sourceID;
  pk1.destid = destID[0];
  pk2.sourceid = sourceID;
  pk2.destid = destID[1];
  pk3.sourceid = sourceID;
  pk3.destid = destID[2];

  for(int i=0;i<4;i++){
    pk1.mincost[i] = dt2.costs[i][sourceID];
    pk2.mincost[i] = dt2.costs[i][sourceID];
    pk3.mincost[i] = dt2.costs[i][sourceID];
  }

  //If neighbor, send
  if(dt2.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
  if(dt2.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
  if(dt2.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);
}

extern void rtupdate2(struct rtpkt *rcvdpkt)
{
  /* TODO */
  int changed = 0;
  int min_array[4];
  
  for(int i=0;i<4;i++){
    int min;
    min = dt2.costs[i][0];
    if(dt2.costs[i][1]<min) min = dt2.costs[i][1];
    if(dt2.costs[i][2]<min) min = dt2.costs[i][2];
    if(dt2.costs[i][3]<min) min = dt2.costs[i][3];
    min_array[i] = min;
  }
  for(int i=0;i<4;i++){
    dt2.costs[i][rcvdpkt->sourceid]  = rcvdpkt->mincost[i] + dt2.costs[rcvdpkt->sourceid][rcvdpkt->destid];
    if(dt2.costs[i][rcvdpkt->sourceid] > 999) dt2.costs[i][rcvdpkt->sourceid] = 999;
  }

  for(int i=0;i<4;i++){
      int min;
      min = dt2.costs[i][0];
      if(dt2.costs[i][1]<min) min = dt2.costs[i][1];
      if(dt2.costs[i][2]<min) min = dt2.costs[i][2];
      if(dt2.costs[i][3]<min) min = dt2.costs[i][3];
      if(min_array[i] != min) changed = 1;
  }

  if(changed){
    int sourceID = 2;
    int destID[3] ={0,1,3};
    struct rtpkt pk1, pk2, pk3;

    pk1.sourceid = sourceID;
    pk1.destid = destID[0];
    pk2.sourceid = sourceID;
    pk2.destid = destID[1];
    pk3.sourceid = sourceID;
    pk3.destid = destID[2];


    for(int i=0;i<4;i++){
      int min;
      min = dt2.costs[i][0];
      if(dt2.costs[i][1]<min) min = dt2.costs[i][1];
      if(dt2.costs[i][2]<min) min = dt2.costs[i][2];
      if(dt2.costs[i][3]<min) min = dt2.costs[i][3];
      pk1.mincost[i] = min;
      pk2.mincost[i] = min;
      pk3.mincost[i] = min;
    }

    //If neighbor, send
    if(dt2.costs[destID[0]][destID[0]] != 999) tolayer2(pk1);
    if(dt2.costs[destID[1]][destID[1]] != 999) tolayer2(pk2);
    if(dt2.costs[destID[2]][destID[2]] != 999) tolayer2(pk3);
    
  }
  /*
  printf("2\n");
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      printf("%d ", dt2.costs[i][j]);
    }
    printf("\n");
  }
  */

}

void printdt2(void) 
{
  struct distance_table *dtptr = &dt2;

  printf("                via     \n");
  printf("   D2 |    0     1    3 \n");
  printf("  ----|-----------------\n");
  printf("     0|  %3d   %3d   %3d\n",dtptr->costs[0][0],
	 dtptr->costs[0][1],dtptr->costs[0][3]);
  printf("dest 1|  %3d   %3d   %3d\n",dtptr->costs[1][0],
	 dtptr->costs[1][1],dtptr->costs[1][3]);
  printf("     3|  %3d   %3d   %3d\n",dtptr->costs[3][0],
	 dtptr->costs[3][1],dtptr->costs[3][3]);
}

extern void print_min_cost2()
{

  int min_cost[4];

  for (int i = 0; i < 4; i++)
  {
    int min = dt2.costs[i][i];
    for (int j = 0; j < 4; j++)
    {
      min = min > dt2.costs[i][j] ? dt2.costs[i][j] : min;
    }

    min_cost[i] = min;
  }

  printf("Min cost %d : %d %d %d %d\n", 2, min_cost[0], min_cost[1],
         min_cost[2], min_cost[3]);
}
