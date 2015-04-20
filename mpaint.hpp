#ifndef _MPAINT_HPP
#define _MPAINT_HPP
//
#include <QtGui>
#include <cmath>
//OGDF
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
//
#include <ogdf/tree/TreeLayout.h> //Tree
//
#include <ogdf/planarity/PlanarizationLayout.h> //Orthogonal
#include <ogdf/planarity/VariableEmbeddingInserter.h>
#include <ogdf/planarity/FastPlanarSubgraph.h>
#include <ogdf/orthogonal/OrthoLayout.h>
#include <ogdf/planarity/EmbedderMinDepthMaxFaceLayers.h>
//
#include <ogdf/layered/SugiyamaLayout.h> //(Fast) Herarchy
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/FastHierarchyLayout.h>
//
#include <ogdf/energybased/FMMMLayout.h>//Energy 1
//
//#include <ogdf/energybased/FastMultipoleEmbedder.h> //Energy 2 NIE U¯YWAÆ 2
//
#include <ogdf/planarlayout/PlanarStraightLayout.h>
//
#undef MYDEBUG

using namespace ogdf;

class MPaint : public QDialog{
	
	Q_OBJECT
	
	public:
		MPaint(QWidget *parent, int *options);
		//
		void addVertex(int v);
		void addEdge(int from, int to);
		//
		void drawIt();
		//
		void assignColors(QMap<int,int> &);
		void assignBFS(QMap<int,int> &);
		void assignBFSTree(QMap<int,int> &);
		void assignDFSTree(QMap<int,int> &);
		void assignDFS(QMap<int,int> & , QMap<int,int> &);
		//
	private:
		//QT
		QGraphicsScene* scene;  
		QGraphicsView* view;  
		QVBoxLayout* layout; 
		//inne deklaracje
		int nodeCounter;
		int len;
		//OGDF
		Graph G;
		GraphAttributes *GA;
		node left;
		node bottom;
		//node _1st;
		//node _2nd;
		QMap<int, node > cache;
		int *opt;
		QMap < int, int > BFSData;
		QMap < int, int > graphColored;
		QMap < int, int > DFSDataStart;
		QMap < int, int > DFSDataEnd;
		QMap < int, int > BFSTree;
		QMap < int, int > DFSTree;
};

#endif