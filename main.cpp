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
  std::vector<int> edges;
};

struct matrix{
  int m, n;
  std::vector<struct pixel> pixels;
  std::vector<struct edge> edges;
};

void printMatrix(struct matrix *matrix){
  /*int pixelCounter = 0;
  for(int a=0; a<matrix->m; a++){
    for(int b=0; b<matrix->n; b++){
        printf("%d ", matrix->pixels[pixelCounter].edges[0].cf);
        pixelCounter++;
    }
    printf("\n");
  }*/
  for(size_t a = 0; a<matrix->pixels.size(); a++) {
    printf("%ld: ", a);
    for(size_t b=0; b<matrix->pixels[a].edges.size(); b++){
        //printf("%d ", edges[matrix->pixels[a].edges[b]].cf);
        printf("%d ", matrix->edges[matrix->pixels[a].edges[b]].cf);
    }
    printf("\n");
  }
}

void saturateDirectEdges(struct matrix *matrix){
  for(int a=1; a<matrix->pixels.size()-1; a++){
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

int BFS(struct matrix *matrix, int startNode, std::vector<int> visitedPixels, std::vector<int> currentPathCapacity, std::vector<int> flows) {
  std::queue<int> q;
  q.push(startNode);
  for(int i = 0; i<visitedPixels.size(); i++){
    visitedPixels[i] = -1;
  }
  for(int i = 0; i < currentPathCapacity.size(); i++){
    currentPathCapacity[i] = 0;
  }
  printf("visitedPixels[0]: %d\n", visitedPixels[2]);
  visitedPixels[startNode] = -2;
  currentPathCapacity[startNode] = 1000;
  while(!q.empty()){
    int currentNode = q.front();
    printf("currentNode: %d\n", currentNode);
    q.pop();
    //the target was found which means a path has been founded and saved
    /*if(currentNode == matrix->pixels.size()-1){
      return currentPathCapacity[matrix->pixels.size()-1];
    }*/
    for(int i=1; i<matrix->pixels[currentNode].edges.size(); i++){
      struct edge e = matrix->edges[matrix->pixels[currentNode].edges[i]];
      //flows.push_back(i);
        printf("e.cf: %d\n", e.cf);
        printf("destiny: %d\n", e.destiny);
      if(visitedPixels[e.destiny]==-1){ //check if a pixel has been visited
        if(e.cf > 0){
          visitedPixels[e.destiny]=currentNode;
          currentPathCapacity[e.destiny] = std::min(currentPathCapacity[currentNode], e.cf);
          printf("currentPathCapacityf: %d\n", currentPathCapacity[e.destiny]);
          printf("e.destiny: %d\n", e.destiny);
          //printf("Estamos no pixel %d: %d\n Pixel Final:%ld\n", e.origin, currentPathCapacity[e.destiny], matrix->pixels.size()-1);
          if(e.destiny == matrix->pixels.size()-1){
            return currentPathCapacity[e.destiny];
          }
          q.push(e.destiny);
        }

      }
    }
  }
  return 0;
}

int edmondsKarp(struct matrix *matrix, int startNode, std::vector<int> visitedPixels, std::vector<int> currentPathCapacity) { //returns the maximum flow
  int maxFlow = 0;
  while(true){
    std::vector<int> flows;
    int flow = BFS(matrix, startNode, visitedPixels, currentPathCapacity, flows);
    printf("%d\n", flow);
    int i = flows.size()-1;
    if(){
      break;
    }
    maxFlow+=flow;
    int currentNode = matrix->pixels.size()-1;
    while(currentNode != startNode){
      int previousNode = visitedPixels[currentNode];
      printf("%d\n", flow);
      matrix->pixels[currentNode].edges[flows[i]].cf -= flow;
      matrix->pixels[currentNode].edges[flows[i]].backwardsCF += flow;
      currentNode = previousNode;
      i--;
    }
  }
  printf("maxflow: %d\n", maxFlow);
  return maxFlow;
}

int main() {
  struct matrix matrix;
  scanf("%d %d", &(matrix.m), &(matrix.n));
  int aux;
  struct pixel target, source;
  std::vector<int> currentPathCapacity(matrix.n*matrix.m+2);
  std::vector<int> visitedPixels(matrix.m*matrix.n+2);

  //reading black (primeiro plano) values
  int pixelCounter = 1;
  int edgeCounter = 0;
  matrix.pixels.push_back(source);
  for(int a=0; a<matrix.m; a++){
    for(int b=0; b<matrix.n; b++){
      //add connections from each pixel to source
      struct pixel p;
      struct edge edge_aux;
      scanf("%d", &aux);
      p.edges.push_back(edgeCounter);
      matrix.pixels.push_back(p);
      //add connections from source for each vertex
      edge_aux.origin = 0; //corresponds to source pixel
      edge_aux.destiny = pixelCounter++; //corresponds to current pixel
      edge_aux.cf = aux;
      edge_aux.backwardsCF = aux;
      //matrix.pixels[0].edges.push_back(edge_aux); //add source to matrix
      matrix.edges.push_back(edge_aux);
      matrix.pixels[0].edges.push_back(edgeCounter++);
    }
  }

  //reading white (cenario) values
  pixelCounter = 1;
  matrix.pixels.push_back(target);
  int matrixSize = matrix.pixels.size()-1;
  for(int a=0; a<matrix.m; a++){
    for(int b=0; b<matrix.n; b++){
      struct edge edge_aux;
      scanf("%d", &aux);
      matrix.pixels[pixelCounter].edges.push_back(edgeCounter);
      //matrix.pixels[pixelCounter].edges.push_back(e);
      //add connections from target for each vertex
      edge_aux.origin = pixelCounter++; //corresponds to target pixel
      edge_aux.destiny = matrixSize; //corresponds to current pixel
      edge_aux.cf = aux;
      edge_aux.backwardsCF = aux;
      //matrix.pixels[matrixSize].edges.push_back(edge_aux);
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
  //printMatrix(&matrix);
  edmondsKarp(&matrix, 1, visitedPixels, currentPathCapacity);
  return 0;
}
