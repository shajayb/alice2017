#define _MAIN_
#define _ALG_LIB_

#ifdef _MAIN_

#include "main.h"
#include "MODEL.h"
#include "interpolate.h"

#include <array>
#include <memory>
#include<time.h>
#include<experimental/generator> 
#include<experimental/generator> 
using namespace std;
using namespace std::experimental;

#include "rigidCube.h"
#include "ActiveGraph.h"

//////////////////////////////////////////////////////////////////////////
Graph G;
activeGraph wet_AG, wet_AG1;
rigidCube r1, r2;
vector<activeGraph> taskGraph;

void setup()
{
	int j = 1;
	string file = "";
	file += "data/graph";
	file += "_";
	char s[20];
	itoa(j, s, 10);
	file += s;
	file += ".txt";
	importer imp = *new importer(file, 10000, 1.0);
	imp.readPts_p5();

	
	G.reset();
	for (int i = 0; i < imp.nCnt; i++) G.createVertex(imp.nodes[i].pos + vec(0, 0, 0.1 * float(j)));
	for (int i = 0; i < imp.eCnt; i++) G.createEdge(G.vertices[imp.edges[i].n0], G.vertices[imp.edges[i].n1]);

	// center graph to origin
	vec minV, maxV;
	G.boundingbox(minV, maxV);
	for (int i = 0; i < G.n_v; i++) G.positions[i] -= (minV + maxV) * 0.5;
	//

	wet_AG = *new activeGraph();
	toroidalGraph A;
	A.constructFromGraph(G, 5);
	wet_AG.constructFromToroidalGraph(A);
	wet_AG.populateRigidBodies( 0.25,1.0);
	
	wet_AG1.constructFromToroidalGraph(A);
	for (int i = 0; i < wet_AG1.n_v; i++)wet_AG1.positions[i].z += 1;
	wet_AG1.populateRigidBodies(0.25, 1.0);

	int i = 15;
	r1 = wet_AG1.RCsOnCurve[i];
	r2 = wet_AG.RCsOnCurve[i];

	taskGraph.clear();
	taskGraph.push_back(wet_AG);
	taskGraph.push_back(wet_AG1);

	//for (int j = 0; j < 1; j++)
	//{
	//	activeGraph AG = *new activeGraph();
	//	toroidalGraph A;
	//	A.constructFromGraph(G, 5);
	//	AG.constructFromToroidalGraph(A);
	//	for (int i = 0; i < AG.n_v; i++)AG.positions[i].z -= 1 * j;
	//	AG.populateRigidBodies(0.25, 1.0);
	//	taskGraph.push_back(AG);

	//}
}


void update(int value)
{
	//AG.smoothVertices();

}


void draw()
{

	backGround(0.75);
	drawGrid(20);

	//G.draw();
	//AG.draw();
	/*AG1.display();*/
	//for (auto AG : taskGraph)AG.display();

	
	wet_AG1 = taskGraph[1];
	wet_AG = taskGraph[0];

	int n = MIN(wet_AG1.RCsOnCurve.size(), wet_AG.RCsOnCurve.size());

	for (int i = 0; i < 5; i++)
	{
		r1 = wet_AG1.RCsOnCurve[i];
		r2 = wet_AG.RCsOnCurve[i];
		r1.isFacetoFace(r2, 5, 4, 0.2);
		r1.draw();
		r2.draw();
	}




}

///////////////////////////////////////////////////////////////////////////////////////////////

void mousePress(int b, int state, int x, int y)
{
		
}



void mouseMotion(int x, int y)
{
	
}

void keyPress(unsigned char k, int xm, int ym)
{

	wet_AG1 = taskGraph[1];
	for (int i = 0; i < 2; i++)taskGraph[1].smoothVertices();
	taskGraph[1].populateRigidBodies(0.25, 1.0);

}





#endif // _MAIN_
