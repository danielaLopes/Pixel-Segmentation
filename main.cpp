/*
      SECOND ASA'S PROJECT
  --- PIXEL SEGMENTATION ---

  Daniela Lopes, 86403
  Joana Teodoro, 86440
*/


#include <list>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <queue>
#include <cstring>
#include <algorithm>

const int BLACK = 0;
const int WHITE = 1;

struct edge{
  int origin, destiny; //vector indexes of the origin and destiny pixels
  int cf; //capacity flow
  int backwardsCF; //backwards capacity flow
};

struct pixel{
  int color;
  int blackWeight, whiteWeight;
  std::vector<int> edges;
};

struct matrix{
  int m, n;
  std::vector<struct pixel> pixels;
  std::vector<struct edge> edges;
  std::vector<int> currentPathCapacity;
  std::vector<int> visitedPixels;
  std::vector<int> flows;
};

void printMatrix(struct matrix *matrix){
  int pixelCounter = 1;
  for(int a = 0; a<matrix->m; a++) {
    for(int b=0; b<matrix->n; b++){
      if(matrix->pixels[pixelCounter].color == BLACK){
        printf("P ");
      }
      else { printf("C "); }
      pixelCounter++;
    }
    printf("\n");
  }
}

void saturateDirectEdges(struct matrix *matrix){
  for(size_t a=1; a<matrix->pixels.size()-1; a++){
    int edgeIndex1 = matrix->pixels[a].edges[0];
    int edgeIndex2 = matrix->pixels[a].edges[1];
    int cf1 = matrix->edges[edgeIndex1].cf;
    int cf2 = matrix->edges[edgeIndex2].cf;
    int aux = std::min(cf1, cf2);
    matrix->edges[edgeIndex1].cf -= aux;
    matrix->edges[edgeIndex1].backwardsCF += aux;
    matrix->edges[edgeIndex2].cf -= aux;
    matrix->edges[edgeIndex2].backwardsCF += aux;
  }
}

int BFS(struct matrix *matrix, int startNode) {
  std::queue<int> q;
  q.push(startNode);
  for(size_t i = 0; i<matrix->visitedPixels.size(); i++){
    matrix->visitedPixels[i] = -1;
  }
  for(size_t i = 0; i < matrix->currentPathCapacity.size(); i++){
    matrix->currentPathCapacity[i] = 0;
  }
  for(size_t i = 0; i<matrix->flows.size(); i++){
    matrix->flows[i] = -1;
  }
  matrix->visitedPixels[startNode] = -2;
  matrix->currentPathCapacity[startNode] = 1000;
  while(!q.empty()){
    int currentNode = q.front();
    q.pop();
    //the target was found which means a path has been founded and saved
    /*if(currentNode == matrix->pixels.size()-1){
      return currentPathCapacity[matrix->pixels.size()-1];
    }*/
    for(size_t i=0; i<matrix->pixels[currentNode].edges.size(); i++){
      struct edge e = matrix->edges[matrix->pixels[currentNode].edges[i]];
      if(currentNode==e.origin) {
        if(matrix->visitedPixels[e.destiny]==-1){ //check if a pixel has been visited
          if(e.cf > 0){
            matrix->visitedPixels[e.destiny]=currentNode;
            matrix->flows[e.destiny] = matrix->pixels[currentNode].edges[i];
            matrix->currentPathCapacity[e.destiny] = std::min(matrix->currentPathCapacity[currentNode], e.cf);
            if((size_t) e.destiny == matrix->pixels.size()-1){
              return matrix->currentPathCapacity[e.destiny];
            }
            q.push(e.destiny);
          }
        }
      }
      else if(currentNode==e.destiny) {
        if(matrix->visitedPixels[e.origin]==-1){
          if(e.backwardsCF > 0){
            matrix->visitedPixels[e.origin]=currentNode;
            matrix->flows[e.origin] = matrix->pixels[currentNode].edges[i];
            matrix->currentPathCapacity[e.origin] = std::min(matrix->currentPathCapacity[currentNode], e.backwardsCF);
            q.push(e.origin);
          }
        }
      }
    }
  }
  return 0;
}

int edmondsKarp(struct matrix *matrix, int startNode, int* maxFlow) { //returns the maximum flow
  while(true){
    int flow = BFS(matrix, startNode);
    //printf("bfs flow: %d\n", flow);
    int currentNode = matrix->pixels.size()-1;
    //printf("Inicio do caminho no target: %d\n", currentNode);
    if(flow == 0){
      break;
    }
    *maxFlow+=flow;
    while(currentNode != startNode){
      int previousNode = matrix->visitedPixels[currentNode];
      struct edge e = matrix->edges[matrix->flows[currentNode]];
      if(currentNode == e.origin){
        matrix->edges[matrix->flows[currentNode]].cf += flow;
        matrix->edges[matrix->flows[currentNode]].backwardsCF -= flow;
      }
      else if(currentNode == e.destiny){
        matrix->edges[matrix->flows[currentNode]].cf -= flow;
        matrix->edges[matrix->flows[currentNode]].backwardsCF += flow;
      }
      //printf("Fui para: %d\n", currentNode);
      currentNode = previousNode;
    }
  }
  int counterWhite = 0;
  int counterBlack = 0;
  int weightsSum= 0;
  for(size_t a=1; a<matrix->visitedPixels.size()-1;a++){
    if (matrix->visitedPixels[a] != -1){
      matrix->pixels[a].color = WHITE;
      weightsSum+=matrix->pixels[a].whiteWeight;
      counterWhite++;

    }
    else{
      weightsSum+=matrix->pixels[a].blackWeight;
      counterBlack++;
    }
  }
  if(counterBlack == 0 || counterWhite == 0) {
    return weightsSum;
  }
  return *maxFlow + weightsSum;
}

int main() {
  struct matrix matrix;
  scanf("%d %d", &(matrix.m), &(matrix.n));
  int aux;
  struct pixel target, source;
  target.color = BLACK;
  source.color = BLACK;
  int maxFlow = 0;
  //reading black (primeiro plano) values
  int pixelCounter = 1;
  int edgeCounter = 0;
  matrix.pixels.push_back(source);
  matrix.visitedPixels.push_back(-1);
  matrix.currentPathCapacity.push_back(0);
  matrix.flows.push_back(0);
  for(int a=0; a<matrix.m; a++){
    for(int b=0; b<matrix.n; b++){
      //add connections from each pixel to source
      struct pixel p;
      p.color = BLACK;
      struct edge edge_aux;
      scanf("%d", &aux);
      p.blackWeight = aux;
      p.edges.push_back(edgeCounter);
      matrix.pixels.push_back(p);
      //initialize auxiliar vectors
      matrix.visitedPixels.push_back(-1);
      matrix.currentPathCapacity.push_back(0);
      matrix.flows.push_back(0);
      //add connections from source for each vertex
      edge_aux.origin = 0; //corresponds to source pixel
      edge_aux.destiny = pixelCounter++; //corresponds to current pixel
      edge_aux.cf = aux;
      edge_aux.backwardsCF = aux;
      matrix.edges.push_back(edge_aux);
      matrix.pixels[0].edges.push_back(edgeCounter++);
    }
  }

  //reading white (cenario) values
  pixelCounter = 1;
  matrix.pixels.push_back(target);
  matrix.visitedPixels.push_back(-1);
  matrix.currentPathCapacity.push_back(0);
  matrix.flows.push_back(0);
  int matrixSize = matrix.pixels.size()-1;
  for(int a=0; a<matrix.m; a++){
    for(int b=0; b<matrix.n; b++){
      struct edge edge_aux;
      scanf("%d", &aux);
      matrix.pixels[pixelCounter].whiteWeight = aux;
      matrix.pixels[pixelCounter].edges.push_back(edgeCounter);
      //add connections from target for each vertex
      edge_aux.origin = pixelCounter++; //corresponds to target pixel
      edge_aux.destiny = matrixSize; //corresponds to current pixel
      edge_aux.cf = aux;
      edge_aux.backwardsCF = aux;
      matrix.edges.push_back(edge_aux);
      matrix.pixels[matrixSize].edges.push_back(edgeCounter++);
    }
  }

  //Save horizontal edges
  if(matrix.m > 1){
    int pixelCounter = 1;
    for(int i=0; i<matrix.m;i++){
      for(int j=0; j<matrix.n; j++){
        struct edge edge_aux;

        //if pixel in the first column
        if(j!= matrix.n-1){
          edge_aux.origin = pixelCounter;
          edge_aux.destiny = pixelCounter + 1;
          scanf("%d", &edge_aux.cf);
          edge_aux.backwardsCF = edge_aux.cf;
          matrix.pixels[pixelCounter+1].edges.push_back(edgeCounter);
          matrix.pixels[pixelCounter].edges.push_back(edgeCounter++);
          matrix.edges.push_back(edge_aux);
        }
        pixelCounter++;
      }
    }

    //Save vertical edges
    if(matrix.n > 1){
      pixelCounter= 1;
      for(int i=0; i<matrix.m;i++){
        for(int j=0; j<matrix.n; j++){
          struct edge edge_aux;

          //if pixel in the first line
          if(i!=matrix.m-1){
            edge_aux.origin = pixelCounter;
            edge_aux.destiny = pixelCounter + matrix.n;
            scanf("%d", &(edge_aux.cf));
            edge_aux.backwardsCF = edge_aux.cf;
            matrix.pixels[pixelCounter+matrix.n].edges.push_back(edgeCounter);
            matrix.pixels[pixelCounter].edges.push_back(edgeCounter++);
            matrix.edges.push_back(edge_aux);
          }
          pixelCounter++;
        }
      }
    }
  }
  saturateDirectEdges(&matrix);
  printf("%d\n\n", edmondsKarp(&matrix, 0, &maxFlow));
  printMatrix(&matrix);
  return 0;
}
