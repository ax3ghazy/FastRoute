////////////////////////////////////////////////////////////////////////////////
// Authors: Vitor Bandeira, Eder Matheus Monteiro e Isadora Oliveira
//          (Advisor: Ricardo Reis)
//
// BSD 3-Clause License
//
// Copyright (c) 2019, Federal University of Rio Grande do Sul (UFRGS)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <algorithm>
#include "DataType.h"
#include "flute.h"
#include "pdrev.h"
#include "DataProc.h"
#include "FastRoute.h"
#include "utility.h"
#include "RSMT.h"
#include "maze.h"
#include "RipUp.h"
#include "route.h"
#include "maze3D.h"
#include <iostream>
#include <string>
#include <utility>
#include <fstream>

namespace FastRoute {

int newnetID;
int segcount;
int pinInd;
int numAdjust;
int vCapacity;
int hCapacity;
int MD;

FT::FT() {
        newnetID = 0;
        segcount = 0;
        pinInd = 0;
        numAdjust = 0;
        vCapacity = 0;
        hCapacity = 0;
        MD = 0;
        numNets = 0;
        invalidNets = 0;
}

FT::~FT() {
        int i, deg, numEdges, edgeID;
        TreeEdge* treeedge;

        for (i = 0; i < (numNets - invalidNets); i++) {
                if (nets[i]->pinX)
                        delete[] nets[i]->pinX;
                nets[i]->pinX = nullptr;

                if (nets[i]->pinY)
                        delete[] nets[i]->pinY;
                nets[i]->pinY = nullptr;

                if (nets[i]->pinL)
                        delete[] nets[i]->pinL;
                nets[i]->pinL = nullptr;
        }

        for (i = 0; i < numNets; i++) {
                if (nets[i])
                        delete nets[i];
                nets[i] = nullptr;
        }

        if (nets)
                delete[] nets;

        nets = nullptr;

        if (h_edges)
                delete[] h_edges;
        h_edges = nullptr;

        if (v_edges)
                delete[] v_edges;
        v_edges = nullptr;

        if (seglist)
                delete[] seglist;
        seglist = nullptr;

        if (seglistIndex)
                delete[] seglistIndex;
        seglistIndex = nullptr;

        if (seglistCnt)
                delete[] seglistCnt;
        seglistCnt = nullptr;

        if (gxs)
                delete[] gxs;
        gxs = nullptr;

        if (gys)
                delete[] gys;
        gys = nullptr;

        if (gs)
                delete[] gs;
        gs = nullptr;

        if (treeOrderPV)
                delete[] treeOrderPV;
        treeOrderPV = nullptr;

        if (treeOrderCong)
                delete[] treeOrderCong;
        treeOrderCong = nullptr;

        if (h_edges3D)
                delete[] h_edges3D;
        h_edges3D = nullptr;

        if (v_edges3D)
                delete[] v_edges3D;
        v_edges3D = nullptr;

        if (trees)
                delete[] trees;
        trees = nullptr;

        for (i = 0; i < numValidNets; i++) {
                deg = sttrees[i].deg;
                numEdges = 2 * deg - 3;
                for (edgeID = 0; edgeID < numEdges; edgeID++) {
                        treeedge = &(sttrees[i].edges[edgeID]);
                        if (treeedge->len > 0) {
                                if (treeedge->route.gridsX)
                                        free(treeedge->route.gridsX);
                                if (treeedge->route.gridsY)
                                        free(treeedge->route.gridsY);
                                if (treeedge->route.gridsL)
                                        free(treeedge->route.gridsL);
                                treeedge->route.gridsX = nullptr;
                                treeedge->route.gridsY = nullptr;
                                treeedge->route.gridsL = nullptr;
                        }
                }

                if (sttrees[i].nodes)
                        delete[] sttrees[i].nodes;
                sttrees[i].nodes = nullptr;

                if (sttrees[i].edges)
                        delete[] sttrees[i].edges;
                sttrees[i].edges = nullptr;
        }

        if (sttrees)
                delete[] sttrees;
        sttrees = nullptr;

        for (i = 0; i < yGrid; i++) {
                if (parentX1[i])
                        delete[] parentX1[i];
                if (parentY1[i])
                        delete[] parentY1[i];
                if (parentX3[i])
                        delete[] parentX3[i];
                if (parentY3[i])
                        delete[] parentY3[i];

                parentX1[i] = nullptr;
                parentY1[i] = nullptr;
                parentX3[i] = nullptr;
                parentY3[i] = nullptr;
        }

        if (parentX1)
                delete[] parentX1;
        if (parentY1)
                delete[] parentY1;
        if (parentX3)
                delete[] parentX3;
        if (parentY3)
                delete[] parentY3;
        if (pop_heap2)
                delete[] pop_heap2;
        if (heap1)
                delete[] heap1;
        if (heap2)
                delete[] heap2;

        parentX1 = nullptr;
        parentY1 = nullptr;
        parentX3 = nullptr;
        parentY3 = nullptr;
        pop_heap2 = nullptr;
        heap1 = nullptr;
        heap2 = nullptr;

        if (xcor)
                delete[] xcor;
        if (ycor)
                delete[] ycor;
        if (dcor)
                delete[] dcor;
        if (netEO)
                delete[] netEO;

        xcor = nullptr;
        ycor = nullptr;
        dcor = nullptr;
        netEO = nullptr;

        for (int i = 0; i < YRANGE; i++) {
                if (HV[i])
                        delete[] HV[i];
                HV[i] = nullptr;
        }

        if (HV)
                delete[] HV;
        HV = nullptr;
        
        for (int i = 0; i < YRANGE; i++) {
                if (hyperV[i])
                        delete[] hyperV[i];
                hyperV[i] = nullptr;
        }

        if (hyperV)
                delete[] hyperV;
        hyperV = nullptr;
        
        for (int i = 0; i < XRANGE; i++) {
                if (hyperH[i])
                        delete[] hyperH[i];
                hyperH[i] = nullptr;
        }

        if (hyperH)
                delete[] hyperH;
         hyperH = nullptr;
        
        for (int i = 0; i < YRANGE; i++) {
                if (inRegion[i])
                        delete[] inRegion[i];
                inRegion[i] = nullptr;
        }

        if (inRegion)
                delete[] inRegion;
        inRegion = nullptr;
        
        for (int i = 0; i < YRANGE; i++) {
                if (corrEdge[i])
                        delete[] corrEdge[i];
                corrEdge[i] = nullptr;
        }

        if (corrEdge)
                delete[] corrEdge;
        corrEdge = nullptr;
        
        if (d13D)
                delete[] d13D;
        if (d23D)
                delete[] d23D;
        if (d1)
                delete[] d1;
        if (d2)
                delete[] d2;

        d13D = nullptr;
        d23D = nullptr;
        d1 = nullptr;
        d2 = nullptr;
        
        if (vCapacity3D)
                delete[] vCapacity3D;
        if (hCapacity3D)
                delete[] hCapacity3D;

        vCapacity3D = nullptr;
        hCapacity3D = nullptr;
        
        if (MinWidth)
                delete[] MinWidth;
        if (MinSpacing)
                delete[] MinSpacing;
        if (ViaSpacing)
                delete[] ViaSpacing;

        MinWidth = nullptr;
        MinSpacing = nullptr;
        ViaSpacing = nullptr;
        
        if (gridHs)
                delete[] gridHs;
        if (gridVs)
                delete[] gridVs;

        gridHs = nullptr;
        gridVs = nullptr;
        
        for (int i = 0; i < numLayers; i++) {
                if (layerGrid[i])
                        delete[] layerGrid[i];
                layerGrid[i] = nullptr;
        }

        if (layerGrid)
                delete[] layerGrid;
        layerGrid = nullptr;
        
        for (int i = 0; i < numLayers; i++) {
                if (gridD[i])
                        delete[] gridD[i];
                gridD[i] = nullptr;
        }

        if (gridD)
                delete[] gridD;
        gridD = nullptr;
        
        for (int i = 0; i < numLayers; i++) {
                if (viaLink[i])
                        delete[] viaLink[i];
                viaLink[i] = nullptr;
        }

        if (viaLink)
                delete[] viaLink;
        viaLink = nullptr;
        
        if (costHVH)
                delete[] costHVH;
        if (costVHV)
                delete[] costVHV;
        if (costH)
                delete[] costH;
        if (costV)
                delete[] costV;
        if (costLR)
                delete[] costLR;
        if (costTB)
                delete[] costTB;
        if (costHVHtest)
                delete[] costHVHtest;
        if (costVtest)
                delete[] costVtest;
        if (costTBtest)
                delete[] costTBtest;

        costHVH = nullptr;
        costVHV = nullptr;
        costH = nullptr;
        costV = nullptr;
        costLR = nullptr;
        costTB = nullptr;
        costHVHtest = nullptr;
        costVtest = nullptr;
        costTBtest = nullptr;

        newnetID = 0;
        segcount = 0;
        pinInd = 0;
        numAdjust = 0;
        vCapacity = 0;
        hCapacity = 0;
        MD = 0;
        numNets = 0;
}

void FT::setGridsAndLayers(int x, int y, int nLayers) {
        xGrid = x;
        yGrid = y;
        numLayers = nLayers;
        numGrids = xGrid * yGrid;
        if (std::max(xGrid, yGrid) >= 1000) {
                XRANGE = std::max(xGrid, yGrid);
                YRANGE = std::max(xGrid, yGrid);
        } else {
                XRANGE = 1000;
                YRANGE = 1000;
        }

        vCapacity3D = new int[numLayers];
        hCapacity3D = new int[numLayers];

        for (int i = 0; i < numLayers; i++) {
                vCapacity3D[i] = 0;
                hCapacity3D[i] = 0;
        }

        MinWidth = new int[numLayers];
        MinSpacing = new int[numLayers];
        ViaSpacing = new int[numLayers];
        gridHs = new int[numLayers];
        gridVs = new int[numLayers];
        
        layerGrid = new int*[numLayers];
        for (int i = 0; i < numLayers; i++) {
                layerGrid[i] = new int[MAXLEN];
        }
        
        gridD = new int*[numLayers];
        for (int i = 0; i < numLayers; i++) {
                gridD[i] = new int[MAXLEN];
        }
        
        viaLink = new int*[numLayers];
        for (int i = 0; i < numLayers; i++) {
                viaLink[i] = new int[MAXLEN];
        }

        d13D = new int[numLayers*YRANGE*XRANGE];
        d23D = new short[numLayers*YRANGE*XRANGE];

        d1 = new float[YRANGE * XRANGE];
        d2 = new float[YRANGE * XRANGE];

        HV = new Bool*[YRANGE];
        for (int i = 0; i < YRANGE; i++) {
                HV[i] = new Bool[XRANGE];
        }
        
        hyperV = new Bool*[YRANGE];
        for (int i = 0; i < YRANGE; i++) {
                hyperV[i] = new Bool[XRANGE];
        }
        
        hyperH = new Bool*[YRANGE];
        for (int i = 0; i < YRANGE; i++) {
                hyperH[i] = new Bool[XRANGE];
        }

        corrEdge = new int*[YRANGE];
        for (int i = 0; i < YRANGE; i++) {
                corrEdge[i] = new int[XRANGE];
        }

        inRegion = new Bool*[YRANGE];
        for (int i = 0; i < YRANGE; i++) {
                inRegion[i] = new Bool[XRANGE];
        }

        costHVH = new float[XRANGE];  // Horizontal first Z
        costVHV = new float[YRANGE];  // Vertical first Z
        costH = new float[YRANGE];    // Horizontal segment cost
        costV = new float[XRANGE];    // Vertical segment cost
        costLR = new float[YRANGE];   // Left and right boundary cost
        costTB = new float[XRANGE];   // Top and bottom boundary cost

        costHVHtest = new float[YRANGE];  // Vertical first Z
        costVtest = new float[XRANGE];    // Vertical segment cost
        costTBtest = new float[XRANGE];   // Top and bottom boundary cost
}

void FT::addVCapacity(int verticalCapacity, int layer) {
        vCapacity3D[layer - 1] = verticalCapacity;
        vCapacity += vCapacity3D[layer - 1];
}

void FT::addHCapacity(int horizontalCapacity, int layer) {
        hCapacity3D[layer - 1] = horizontalCapacity;
        hCapacity += hCapacity3D[layer - 1];
}

void FT::addMinWidth(int width, int layer) {
        MinWidth[layer - 1] = width;
}

void FT::addMinSpacing(int spacing, int layer) {
        MinSpacing[layer - 1] = spacing;
}

void FT::addViaSpacing(int spacing, int layer) {
        ViaSpacing[layer - 1] = spacing;
}

void FT::setNumberNets(int nNets) {
        numNets = nNets;
        nets = new Net*[numNets];
        for (int i = 0; i < numNets; i++)
                nets[i] = new Net;
        seglistIndex = new int[numNets];
}

void FT::setLowerLeft(int x, int y) {
        xcorner = x;
        ycorner = y;
}

void FT::setTileSize(int width, int height) {
        wTile = width;
        hTile = height;
}

void FT::setLayerOrientation(int x) {
        layerOrientation = x;
}

void FT::addNet(char *name, int netIdx, int nPins, int minWidth, PIN pins[], float alpha) {
        // std::cout << "Adding net " << name << "\n";
        int pinX, pinY, pinL, netID, numPins;
        long pinX_in, pinY_in;

        // TODO: check this size
        int pinXarray[nPins];
        int pinYarray[nPins];
        int pinLarray[nPins];

        netID = netIdx;
        numPins = nPins;

        allNets[name] = std::vector<PIN>(&pins[0], &pins[numPins]);

        // TODO: check this, there was an if pinInd < 2000
        pinInd = 0;
        for (int j = 0; j < numPins; j++) {
                pinX_in = pins[j].x;
                pinY_in = pins[j].y;
                pinL = pins[j].layer;
                pinX = (int)((pinX_in - xcorner) / wTile);
                pinY = (int)((pinY_in - ycorner) / hTile);
                if (!(pinX < 0 || pinX >= xGrid || pinY < -1 || pinY >= yGrid || pinL > numLayers || pinL <= 0)) {
                        bool remove = false;
                        for (int k = 0; k < pinInd; k++) {
                                if (pinX == pinXarray[k] && pinY == pinYarray[k] && pinL == pinLarray[k]) {
                                        remove = true;
                                        break;
                                }
                        }
                        if (!remove)  // the pin is in different grid from other pins
                        {
                                pinXarray[pinInd] = pinX;
                                pinYarray[pinInd] = pinY;
                                pinLarray[pinInd] = pinL;
                                pinInd++;
                        }
                }
        }
        if (pinInd > 1)  // valid net
        {
                MD = std::max(MD, pinInd);
                // std::cout << "Net name: " << nets[newnetID]->name << "; num pins: " << nets[newnetID]->numPins << "\n";
                strcpy(nets[newnetID]->name, name);
                nets[newnetID]->netIDorg = netID;
                nets[newnetID]->numPins = numPins;
                nets[newnetID]->deg = pinInd;
                nets[newnetID]->pinX = new short[pinInd];
                nets[newnetID]->pinY = new short[pinInd];
                nets[newnetID]->pinL = new short[pinInd];
                nets[newnetID]->alpha = alpha;

                for (int j = 0; j < pinInd; j++) {
                        nets[newnetID]->pinX[j] = pinXarray[j];
                        nets[newnetID]->pinY[j] = pinYarray[j];
                        nets[newnetID]->pinL[j] = pinLarray[j];
                }
                seglistIndex[newnetID] = segcount;
                newnetID++;
                segcount += 2 * pinInd - 3;  // at most (2*numPins-2) nodes, (2*numPins-3) nets for a net
        } else {
                invalidNets++;
        }
}

std::map<std::string, std::vector<PIN>> FT::getNets() {
        return allNets;
}

void FT::initEdges() {
        LB = 0.9;
        UB = 1.3;
        int grid, j, k, i;
        int TC;
        vCapacity_lb = LB * vCapacity;
        hCapacity_lb = LB * hCapacity;
        vCapacity_ub = UB * vCapacity;
        hCapacity_ub = UB * hCapacity;

        // TODO: check this, there was an if pinInd > 1 && pinInd < 2000
        if (pinInd > 1) {
                seglistIndex[newnetID] = segcount;  // the end pointer of the seglist
        }
        numValidNets = newnetID;

        // allocate memory and initialize for edges

        h_edges = new Edge[((xGrid - 1) * yGrid)];
        v_edges = new Edge[(xGrid * (yGrid - 1))];

        init_usage();

        v_edges3D = new Edge3D[(numLayers * xGrid * yGrid)];
        h_edges3D = new Edge3D[(numLayers * xGrid * yGrid)];

        //2D edge innitialization
        TC = 0;
        for (i = 0; i < yGrid; i++) {
                for (j = 0; j < xGrid - 1; j++) {
                        grid = i * (xGrid - 1) + j;
                        h_edges[grid].cap = hCapacity;
                        TC += hCapacity;
                        h_edges[grid].usage = 0;
                        h_edges[grid].est_usage = 0;
                        h_edges[grid].red = 0;
                        h_edges[grid].last_usage = 0;
                }
        }
        for (i = 0; i < yGrid - 1; i++) {
                for (j = 0; j < xGrid; j++) {
                        grid = i * xGrid + j;
                        v_edges[grid].cap = vCapacity;
                        TC += vCapacity;
                        v_edges[grid].usage = 0;
                        v_edges[grid].est_usage = 0;
                        v_edges[grid].red = 0;
                        v_edges[grid].last_usage = 0;
                }
        }

        for (k = 0; k < numLayers; k++) {
                for (i = 0; i < yGrid; i++) {
                        for (j = 0; j < xGrid - 1; j++) {
                                grid = i * (xGrid - 1) + j + k * (xGrid - 1) * yGrid;
                                h_edges3D[grid].cap = hCapacity3D[k];
                                h_edges3D[grid].usage = 0;
                                h_edges3D[grid].red = 0;
                        }
                }
                for (i = 0; i < yGrid - 1; i++) {
                        for (j = 0; j < xGrid; j++) {
                                grid = i * xGrid + j + k * xGrid * (yGrid - 1);
                                v_edges3D[grid].cap = vCapacity3D[k];
                                v_edges3D[grid].usage = 0;
                                v_edges3D[grid].red = 0;
                        }
                }
        }
}

void FT::setNumAdjustments(int nAdjustments) {
        numAdjust = nAdjustments;
}

void FT::addAdjustment(long x1, long y1, int l1, long x2, long y2, int l2, int reducedCap, bool isReduce) {
        int grid, k;
        int reduce, cap;
        int increase;
        reducedCap = reducedCap;

        k = l1 - 1;

        if (y1 == y2)  //horizontal edge
        {
                grid = y1 * (xGrid - 1) + x1 + k * (xGrid - 1) * yGrid;
                cap = h_edges3D[grid].cap;

                if (((int)cap - reducedCap) < 0) {
                        if (isReduce) {
                                std::cout << "[WARNING] Underflow in reduce\n";
                                std::cout << "[WARNING] cap, reducedCap: " << cap << ", " << reducedCap << "\n";
                        }
                        reduce = 0;
                } else {
                        reduce = cap - reducedCap;
                }

                h_edges3D[grid].cap = reducedCap;
                h_edges3D[grid].red = reduce;
                
                grid = y1 * (xGrid - 1) + x1;
                if (!isReduce) {
                        increase = reducedCap - cap;
                        h_edges[grid].cap += increase;
                }

                h_edges[grid].cap -= reduce;
                h_edges[grid].red += reduce;

        } else if (x1 == x2)  //vertical edge
        {
                grid = y1 * xGrid + x1 + k * xGrid * (yGrid - 1);
                cap = v_edges3D[grid].cap;
                reduce = cap - reducedCap;

                if (((int)cap - reducedCap) < 0) {
                        if (isReduce) {
                                std::cout << "[WARNING] Underflow in reduce\n";
                                std::cout << "[WARNING] cap, reducedCap: " << cap << ", " << reducedCap << "\n";
                        }
                        reduce = 0;
                } else {
                        reduce = cap - reducedCap;
                }

                v_edges3D[grid].cap = reducedCap;
                v_edges3D[grid].red = reduce;

                grid = y1 * xGrid + x1;
                if (!isReduce) {
                        increase = reducedCap - cap;
                        h_edges[grid].cap += increase;
                }
                
                v_edges[grid].cap -= reduce;
                v_edges[grid].red += reduce;
        }
}

int FT::getEdgeCapacity(long x1, long y1, int l1, long x2, long y2, int l2) {
        int grid, k;
        int cap;

        k = l1 - 1;

        if (y1 == y2)  //horizontal edge
        {
                grid = y1 * (xGrid - 1) + x1 + k * (xGrid - 1) * yGrid;
                cap = h_edges3D[grid].cap;
        } else if (x1 == x2)  //vertical edge
        {
                grid = y1 * xGrid + x1 + k * xGrid * (yGrid - 1);
                cap = v_edges3D[grid].cap;
        }

        return cap;
}

void FT::setMaxNetDegree(int deg) {
        maxNetDegree = deg;
}

void FT::initAuxVar() {
        int k, i;
        treeOrderCong = NULL;
        stopDEC = FALSE;

        seglistCnt = new int[numValidNets];
        seglist = new Segment[segcount];
        trees = new Tree[numValidNets];
        sttrees = new StTree[numValidNets];
        gxs = new DTYPE*[numValidNets];
        gys = new DTYPE*[numValidNets];
        gs = new DTYPE*[numValidNets];

        gridHV = XRANGE * YRANGE;
        gridH = (xGrid - 1) * yGrid;
        gridV = xGrid * (yGrid - 1);
        for (k = 0; k < numLayers; k++) {
                gridHs[k] = k * gridH;
                gridVs[k] = k * gridV;
        }

        MaxDegree = MD;

        parentX1 = new short*[yGrid];
        parentY1 = new short*[yGrid];
        parentX3 = new short*[yGrid];
        parentY3 = new short*[yGrid];

        for (i = 0; i < yGrid; i++) {
                parentX1[i] = new short[xGrid];
                parentY1[i] = new short[xGrid];
                parentX3[i] = new short[xGrid];
                parentY3[i] = new short[xGrid];
        }

        pop_heap2 = new Bool[yGrid * XRANGE];

        // allocate memory for priority queue
        heap1 = new float*[yGrid * xGrid];
        heap2 = new float*[yGrid * xGrid];

        sttreesBK = NULL;
}

std::vector<NET> FT::getResults() {
        short *gridsX, *gridsY, *gridsL;
        int netID, d, i, k, edgeID, nodeID, deg, lastX, lastY, lastL, xreal, yreal, l, routeLen;
        TreeEdge *treeedges, *treeedge;
        TreeNode *nodes;
        TreeEdge edge;
        std::vector<NET> netsOut;
        netsOut.clear();
        for (netID = 0; netID < numValidNets; netID++) {
                NET currentNet;
                std::string netName(nets[netID]->name);
                currentNet.name = netName;
                currentNet.id = netID;
                treeedges = sttrees[netID].edges;
                deg = sttrees[netID].deg;

                nodes = sttrees[netID].nodes;
                for (edgeID = 0; edgeID < 2 * deg - 3; edgeID++) {
                        edge = sttrees[netID].edges[edgeID];
                        treeedge = &(treeedges[edgeID]);
                        if (treeedge->len > 0) {
                                routeLen = treeedge->route.routelen;
                                gridsX = treeedge->route.gridsX;
                                gridsY = treeedge->route.gridsY;
                                gridsL = treeedge->route.gridsL;
                                lastX = wTile * (gridsX[0] + 0.5) + xcorner;
                                lastY = hTile * (gridsY[0] + 0.5) + ycorner;
                                lastL = gridsL[0];
                                for (i = 1; i <= routeLen; i++) {
                                        xreal = wTile * (gridsX[i] + 0.5) + xcorner;
                                        yreal = hTile * (gridsY[i] + 0.5) + ycorner;

                                        ROUTE routing;
                                        routing.initX = lastX;
                                        routing.initY = lastY;
                                        routing.initLayer = lastL + 1;
                                        routing.finalX = xreal;
                                        routing.finalY = yreal;
                                        routing.finalLayer = gridsL[i] + 1;
                                        lastX = xreal;
                                        lastY = yreal;
                                        lastL = gridsL[i];
                                        currentNet.route.push_back(routing);
                                }
                        }
                }
                netsOut.push_back(currentNet);
        }

        return netsOut;
}

void FT::writeCongestionReport2D(std::string fileName) {
        long xReal, yReal;
        int grid, j, k, i;
        std::ofstream congestFile;
        congestFile.open(fileName);
        
        if (!congestFile.is_open()) {
                std::cout << "[ERROR] Congestion report file could not be open!" << std::endl;
                congestFile.close();
                std::exit(1);
        }
        congestFile << "FastRoute congestion report\n\n";

        congestFile << "Area    Vertical Capacity/Usage    Horizontal Capacity/Usage\n";
        congestFile << "---------------------------------------------\n";

        for (i = 0; i < yGrid; i++) {
                for (j = 0; j < xGrid - 1; j++) {
                        gridH = i * (xGrid - 1) + j;
                        gridV = i * xGrid + j;

                        unsigned short capH = h_edges[gridH].cap;
                        unsigned short usageH = h_edges[gridH].usage;

                        unsigned short capV = v_edges[gridV].cap;
                        unsigned short usageV = v_edges[gridV].usage;

                        xReal = wTile * (j + 0.5) + xcorner;
                        yReal = hTile * (i + 0.5) + ycorner;

                        long llX = xReal - (wTile / 2);
                        long llY = yReal - (hTile / 2);

                        long urX = xReal + (wTile / 2);
                        long urY = yReal + (hTile / 2);

                        congestFile << "(" << llX << ", " << llY << ") "
                                    << "(" << urX << ", " << urY << ")"
                                    << "    V: " << capV << "/" << usageV
                                    << "    H: " << capH << "/" << usageH << "\n";
                }
        }

        congestFile.close();
}

void FT::writeCongestionReport3D(std::string fileName) {
        long xReal, yReal;
        int grid, j, k, i;
        std::ofstream congestFile;
        congestFile.open(fileName);
        
        if (!congestFile.is_open()) {
                std::cout << "[ERROR] Congestion report file could not be open!" << std::endl;
                congestFile.close();
                std::exit(1);
        }
        congestFile << "FastRoute congestion report\n\n";

        congestFile << "Area    Vertical Capacity/Usage    Horizontal Capacity/Usage\n";
        congestFile << "---------------------------------------------\n";

        for (k = 0; k < numLayers; k++) {
                congestFile << "Layer " << k + 1 << "\n";
                for (i = 0; i < yGrid; i++) {
                        for (j = 0; j < xGrid - 1; j++) {
                                gridH = i * (xGrid - 1) + j + k * (xGrid - 1) * yGrid;
                                gridV = i * xGrid + j + k * xGrid * (yGrid - 1);

                                unsigned short capH = h_edges3D[gridH].cap;
                                unsigned short usageH = h_edges3D[gridH].usage;

                                unsigned short capV = v_edges3D[gridV].cap;
                                unsigned short usageV = v_edges3D[gridV].usage;

                                xReal = wTile * (j + 0.5) + xcorner;
                                yReal = hTile * (i + 0.5) + ycorner;

                                long llX = xReal - (wTile / 2);
                                long llY = yReal - (hTile / 2);

                                long urX = xReal + (wTile / 2);
                                long urY = yReal + (hTile / 2);

                                congestFile << "(" << llX << ", " << llY << ") "
                                            << "(" << urX << ", " << urY << ")"
                                            << "    V: " << capV << "/" << usageV
                                            << "    H: " << capH << "/" << usageH << "\n";
                        }
                }
        }

        congestFile.close();
}

int FT::run(std::vector<NET> &result) {
        //    char benchFile[FILESTRLEN];
        char routingFile[STRINGLEN];
        char degreeFile[STRINGLEN];
        char optionS[STRINGLEN];
        clock_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
        float gen_brk_Time, P1_Time, P2_Time, P3_Time, maze_Time, totalTime, congestionmap_time;
        int iter, last_totalOverflow, diff_totalOverflow, enlarge, ripup_threshold;
        int i, j, past_overflow, cur_overflow;
        int L_afterSTOP;
        int ESTEP1, CSTEP1, thStep1;
        int ESTEP2, CSTEP2, thStep2;
        int ESTEP3, CSTEP3, thStep3, tUsage, CSTEP4;
        int Ripvalue, LVIter, cost_step;
        int maxOverflow, past_cong, last_cong, finallength, numVia, ripupTH3D, newTH, healingTrigger;
        int updateType, minofl, minoflrnd, mazeRound, upType, cost_type, bmfl, bwcnt;

        Bool goingLV, healingNeed, noADJ, extremeNeeded;

        // TODO: check this size
        int maxPin = maxNetDegree;
        maxPin = 2* maxPin;
        xcor = new int[maxPin];
        ycor = new int[maxPin];
        dcor = new int[maxPin];
        netEO = new OrderNetEdge[maxPin];


        Bool input, WriteOut;
        input = WriteOut = 0;

        LB = 0.9;
        UB = 1.3;

        SLOPE = 5;
        THRESH_M = 20;
        ENLARGE = 15;  //5
        ESTEP1 = 10;   //10
        ESTEP2 = 5;    //5
        ESTEP3 = 5;    //5
        CSTEP1 = 2;    //5
        CSTEP2 = 2;    //3
        CSTEP3 = 5;    //15
        CSTEP4 = 1000;
        COSHEIGHT = 4;
        L = 0;
        VIA = 2;
        L_afterSTOP = 1;
        Ripvalue = -1;
        ripupTH3D = 10;
        goingLV = TRUE;
        noADJ = FALSE;
        thStep1 = 10;
        thStep2 = 4;
        healingNeed = FALSE;
        updateType = 0;
        LVIter = 3;
        extremeNeeded = FALSE;
        mazeRound = 500;
        bmfl = BIG_INT;
        minofl = BIG_INT;

        // call FLUTE to generate RSMT and break the nets into segments (2-pin nets)

        t1 = clock();

        VIA = 2;
        //viacost = VIA;
        viacost = 0;
        gen_brk_RSMT(FALSE, FALSE, FALSE, FALSE, noADJ);
        if (verbose > 1)
                printf("First L Route\n");
        routeLAll(TRUE);
        gen_brk_RSMT(TRUE, TRUE, TRUE, FALSE, noADJ);
        getOverflow2D(&maxOverflow);
        if (verbose > 1)
                printf("Second L Route\n");
        newrouteLAll(FALSE, TRUE);
        getOverflow2D(&maxOverflow);
        spiralRouteAll();
        newrouteZAll(10);
        if (verbose > 1)
                printf("First Z Route\n");
        past_cong = getOverflow2D(&maxOverflow);

        convertToMazeroute();

        enlarge = 10;
        newTH = 10;
        healingTrigger = 0;
        stopDEC = 0;
        upType = 1;
        //iniBDE();
        costheight = COSHEIGHT;
        if (maxOverflow > 700) {
                costheight = 8;
                LOGIS_COF = 1.33;
                VIA = 0;
                THRESH_M = 0;
                CSTEP1 = 30;
                slope = BIG_INT;
        }

        for (i = 0; i < LVIter; i++) {
                LOGIS_COF = std::max<float>(2.0 / (1 + log(maxOverflow)), LOGIS_COF);
                LOGIS_COF = 2.0 / (1 + log(maxOverflow));
                if (verbose > 1)
                        printf("[INFO] LV routing round %d, enlarge %d \n", i, enlarge);
                routeLVAll(newTH, enlarge);

                past_cong = getOverflow2Dmaze(&maxOverflow, &tUsage);

                enlarge += 5;
                newTH -= 5;
                if (newTH < 1) {
                        newTH = 1;
                }
        }

        //	past_cong = getOverflow2Dmaze( &maxOverflow);

        t3 = clock();
        InitEstUsage();

        i = 1;
        costheight = COSHEIGHT;
        enlarge = ENLARGE;
        ripup_threshold = Ripvalue;

        minofl = totalOverflow;
        stopDEC = FALSE;

        slope = 20;
        L = 1;
        cost_type = 1;

        InitLastUsage(upType);
        if (totalOverflow > 0) {
                printf("Running extra iterations to remove overflow...\n");
        }
        
        while (totalOverflow > 0 && i <= overflowIterations) {
                if (THRESH_M > 15) {
                        THRESH_M -= thStep1;
                } else if (THRESH_M >= 2) {
                        THRESH_M -= thStep2;
                } else {
                        THRESH_M = 0;
                }
                if (THRESH_M <= 0) {
                        THRESH_M = 0;
                }

                if (totalOverflow > 2000) {
                        enlarge += ESTEP1;  //ENLARGE+(i-1)*ESTEP;
                        cost_step = CSTEP1;
                        updateCongestionHistory(i, upType);
                } else if (totalOverflow < 500) {
                        cost_step = CSTEP3;
                        enlarge += ESTEP3;
                        ripup_threshold = -1;
                        updateCongestionHistory(i, upType);
                } else {
                        cost_step = CSTEP2;
                        enlarge += ESTEP2;
                        updateCongestionHistory(i, upType);
                }

                if (totalOverflow > 15000 && maxOverflow > 400) {
                        enlarge = std::max(xGrid, yGrid) / 30;
                        slope = BIG_INT;
                        if (i == 5) {
                                VIA = 0;
                                LOGIS_COF = 1.33;
                                ripup_threshold = -1;
                                //	cost_type = 3;

                        } else if (i > 6) {
                                if (i % 2 == 0) {
                                        LOGIS_COF += 0.5;
                                }
                                if (i > 40) {
                                        break;
                                }
                        }
                        if (i > 10) {
                                cost_type = 1;
                                ripup_threshold = 0;
                        }
                }

                enlarge = std::min(enlarge, xGrid / 2);
                costheight += cost_step;
                mazeedge_Threshold = THRESH_M;

                if (upType == 3) {
                        LOGIS_COF = std::max<float>(2.0 / (1 + log(maxOverflow + max_adj)), LOGIS_COF);
                } else {
                        LOGIS_COF = std::max<float>(2.0 / (1 + log(maxOverflow)), LOGIS_COF);
                }

                if (i == 8) {
                        L = 0;
                        upType = 2;
                        InitLastUsage(upType);
                }

                if (maxOverflow == 1) {
                        //L = 0;
                        ripup_threshold = -1;
                        slope = 5;
                }

                if (maxOverflow > 300 && past_cong > 15000) {
                        L = 0;
                }

                printf("[INFO] iteration %d, enlarge %d, costheight %d, threshold %d via cost %d \n"
                        "[INFO] log_coef %f, healingTrigger %d cost_step %d L %d cost_type %d updatetype %d\n",
                        i, enlarge, costheight, mazeedge_Threshold, VIA, LOGIS_COF, healingTrigger, cost_step, L, cost_type, upType);
                mazeRouteMSMD(i, enlarge, costheight, ripup_threshold, mazeedge_Threshold, !(i % 3), cost_type);
                last_cong = past_cong;
                past_cong = getOverflow2Dmaze(&maxOverflow, &tUsage);

                if (minofl > past_cong) {
                        minofl = past_cong;
                        minoflrnd = i;
                }

                if (i == 8) {
                        L = 1;
                }

                i++;

                if (past_cong < 200 && i > 30 && upType == 2 && max_adj <= 20) {
                        upType = 4;
                        stopDEC = TRUE;
                }

                if (maxOverflow < 150) {
                        if (i == 20 && past_cong > 200) {
                                printf("Extra Run for hard benchmark\n");
                                L = 0;
                                upType = 3;
                                stopDEC = TRUE;
                                slope = 5;
                                mazeRouteMSMD(i, enlarge, costheight, ripup_threshold, mazeedge_Threshold, !(i % 3), cost_type);
                                last_cong = past_cong;
                                past_cong = getOverflow2Dmaze(&maxOverflow, &tUsage);

                                str_accu(12);
                                L = 1;
                                stopDEC = FALSE;
                                slope = 3;
                                upType = 2;
                        }
                        if (i == 35 && tUsage > 800000) {
                                str_accu(25);
                        }
                        if (i == 50 && tUsage > 800000) {
                                str_accu(40);
                        }
                }

                if (i > 50) {
                        upType = 4;
                        if (i > 70) {
                                stopDEC = TRUE;
                        }
                }

                if (past_cong > 0.7 * last_cong) {
                        costheight += CSTEP3;
                }

                if (past_cong >= last_cong) {
                        VIA = 0;
                        healingTrigger++;
                }

                if (past_cong < bmfl) {
                        bwcnt = 0;
                        if (i > 140 || (i > 80 && past_cong < 20)) {
                                copyRS();
                                bmfl = past_cong;

                                L = 0;
                                SLOPE = BIG_INT;
                                mazeRouteMSMD(i, enlarge, costheight, ripup_threshold, mazeedge_Threshold, !(i % 3), cost_type);
                                last_cong = past_cong;
                                past_cong = getOverflow2Dmaze(&maxOverflow, &tUsage);
                                if (past_cong < last_cong) {
                                        copyRS();
                                        bmfl = past_cong;
                                }
                                L = 1;
                                SLOPE = 5;
                                if (minofl > past_cong) {
                                        minofl = past_cong;
                                        minoflrnd = i;
                                }
                        }
                } else {
                        bwcnt++;
                }

                if (bmfl > 10) {
                        if (bmfl > 30 && bmfl < 72 && bwcnt > 50) {
                                break;
                        }
                        if (bmfl < 30 && bwcnt > 50) {
                                break;
                        }
                        if (i >= mazeRound) {
                                getOverflow2Dmaze(&maxOverflow, &tUsage);
                                break;
                        }
                }

                if (i >= mazeRound) {
                        getOverflow2Dmaze(&maxOverflow, &tUsage);
                        break;
                }
        }
        
        if (totalOverflow > 0 && !allowOverflow) {
                printf("[ERROR] FastRoute cannot handle very congested design\n");
                std::exit(2);
        }

        if (allowOverflow && totalOverflow > 0) {
                printf("[WARNING] Global routing finished with overflow!");
        }

        if (minofl > 0) {
                printf("\n\n minimal ofl %d, occuring at round %d\n\n", minofl, minoflrnd);
                copyBR();
        }

        freeRR();

        checkUsage();

        if (verbose > 1)
            printf("Maze routing finished\n");

        t4 = clock();
        maze_Time = (float)(t4 - t3) / CLOCKS_PER_SEC;
        
        if (verbose > 1) {
            printf("[INFO] P3 runtime: %f sec\n", maze_Time);

            printf("[INFO] Final 2D results: \n");
        }
        getOverflow2Dmaze(&maxOverflow, &tUsage);

        if (verbose > 1)
                printf("Layer Assignment Begins\n");
        newLA();
        if (verbose > 1)
                printf("Layer assignment finished\n");

        t2 = clock();
        gen_brk_Time = (float)(t2 - t1) / CLOCKS_PER_SEC;
        
        if (verbose > 1)
            printf("[INFO] 2D + Layer Assignment Runtime: %f sec\n", gen_brk_Time);

        costheight = 3;
        viacost = 1;

        if (gen_brk_Time < 60) {
                ripupTH3D = 15;
        } else if (gen_brk_Time < 120) {
                ripupTH3D = 18;
        } else {
                ripupTH3D = 20;
        }

        if (goingLV && past_cong == 0) {
                if (verbose > 1)
                    printf("Post Processing Begins \n");
                mazeRouteMSMDOrder3D(enlarge, 0, ripupTH3D);

                //	mazeRouteMSMDOrder3D(enlarge, 0, 10 );
                if (gen_brk_Time > 120) {
                        mazeRouteMSMDOrder3D(enlarge, 0, 12);
                }
                if (verbose > 1)
                        printf("Post Processsing finished\n Starting via filling\n");
        }

        fillVIA();
        finallength = getOverflow3D();
        numVia = threeDVIA();
        checkRoute3D();

        t4 = clock();
        maze_Time = (float)(t4 - t1) / CLOCKS_PER_SEC;
        printf("[INFO] Final usage          : %d\n", finallength);
        printf("[INFO] Final number of vias : %d\n", numVia);
        printf("[INFO] Final usage 3D       : %d\n", (finallength + 3 * numVia));

        std::cout << "Getting results...\n";
        result = getResults();
        std::cout << "Getting results... Done!\n\n";

        /* TODO:  <11-07-19, this function leads to a segfault, but as the OS
         * frees all memory after the application end (next line) we can omit
         * this function call for now.> */
        /* freeAllMemory(); */
        return (0);
}

void FT::usePdRev(){
        pdRev = true;
}

void FT::setAlpha(float a){
        alpha = a;
}

void FT::setVerbose(int v){
        verbose = v;
}

void FT::setOverflowIterations(int iterations){
        overflowIterations = iterations;
}

void FT::setPDRevForHighFanout(int pdRevHihgFanout){
        pdRevForHighFanout = pdRevHihgFanout;
}

void FT::setAllowOverflow(bool allow) {
        allowOverflow = allow;
}

}  // namespace FastRoute
