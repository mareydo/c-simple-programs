#include <stdio.h>
#include <stdlib.h>

/**
 *  this program simulates a pathfinding algorithm over two lines (rows) of tiles 
    . for walkable, x for blocked, 
    attempting to reach a goal tile in the far right part of either row. 
    It uses Depth-First Search (DFS) and a given jump distance. 
 */



int DFS(char* line, const unsigned long* len, const unsigned long* jmp,const unsigned long tile)
{

    static int step=0;
    static char found='N';
    printf("\nTile>%lu\nStep> %d\n-----\n",tile,step);
    //if done or wall return
    if( (tile==*len-1) || (tile==2*(*len)-1)||
        (tile==*len-*jmp)||(tile==2*(*len)-*jmp) ||
        (line[tile]=='M') || (line[tile]=='D') || (line[tile]=='x')
        )
        {
        printf("\nSTOP at %lu\n-----\n",tile);
        if((tile==*len-1) || (tile==2*(*len)-1)||
               (tile==*len-*jmp)||(tile==2*(*len)-*jmp)) {
            found='Y';
            ++step;
        }
        return step;
    }
    //marked
    line[tile]='M';
    //++step;
    //for every possible dfs
      //next line+jmp
      if((tile<*len)&&(tile+*len+*jmp<2*(*len))&&
         (line[tile+*len+*jmp]!='x')
         ) {//L->R
          step = DFS(line, len, jmp, tile + *len + *jmp);
      }

      if( (tile>=*len)&&(tile-*len+*jmp>0)&&
          (line[tile-*len+*jmp]!='x')
          ) {//R->L
          step = DFS(line, len, jmp, tile - *len + *jmp);
      }

      //one forward
      if( (tile<2*(*len))&&(line[tile+1]!='x') )
          step=DFS(line,len,jmp,tile+1);
      //one backward
      if( ((tile>0)&&(tile<*len)&&(tile>step))||
          ((tile>*len)&&(tile-*len>step))
          ) {
          step = DFS(line, len, jmp, tile - 1);
      }
    //mark as done
    line[tile]='D';

    return (found=='N') ? -1 : step;
}

int main(void)
{
    //variables
    char* line;
    unsigned long len, jmp,step;
    unsigned long i;

    //mem alocation
    scanf("%lu %lu",&len,&jmp);
    line=(char*)malloc(sizeof(char)*2*len);

    for(i=0; i<2*len;++i){
        while( ((scanf("%c",&line[i]))!=1)|| ( (line[i]!='.')&&(line[i]!='x')) );
    }

    //DFS
    step=DFS(line,&len,&jmp,0);

    //DEBUG PRINT
    printf("\n-------\n");

    for(i=0;i<2*len;++i){
        printf("%c",line[i]);
        if(i==len-1) printf("\n");

    }

    //final print
    printf("\n\n%lu",step);
    free(line);

    return 0;

}