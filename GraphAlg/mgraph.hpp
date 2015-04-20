#ifndef _MGRAPH_HPP
#define _MGRAPH_HPP
#include <QtGui>
#include <climits>
#undef MYDEBUG

class MGraph : public QObject{
	
	Q_OBJECT
	
	public:
		MGraph();
		void addVertex(int number);
		void addEdge(int from, int to);
		QString graphToString();
		bool contains(int vertex); //Sprawdza czy wierzcho³ek jest w grafie
		void reset(); // Ustawia currentV na maxint oraz na NULL
		QString getBFSResult(); // Zwraca rezultat przeszukiwania BFS
		QString getDFSResult(); //jw dla dfs
		QString getColoredResult();
		void BFSMark(int);
		void DFSMark(int);
		void DFSMarkStart(int);
		void DFSMarkEnd(int);
		void setCurrentVertex(int);
		int nextUnmarkedAdjacentB();
		int nextUnmarkedAdjacentD();
		void clearAfterAlgorithm(); //Sprz¹ta po algorytmach, ustawiaj¹c iteratory na wartoœæ NULL, NIE MUSI TEGO BYÆ
		void clearBFSResult(); //Funkcje czyszcz¹ce rezultaty
		void clearDFSResult();
		void setFirstValues(); //Ustawia pierwsze 
		int nextAdjacent(); //Nastêpny s¹siad z listy
		int nextVertex(); //Nastêpny wierzcho³ek
		int getNextEdge(); //Nastêpny s¹siad z listy
		bool isBFSMarked(int); //Czy wierzcholek byl odwiedzony
		bool isDFSMarked(int); //Czy wierzcholek byl odwiedzony
		QLinkedList<int> &getVertexList(); //Zwraca listê wierzcho³ków 
		void setTransposed(MGraph *);
		void setUndirected(MGraph *);
		MGraph *getTransposed();
		MGraph *getUndirected();
		QMap<int,int> &getDFSReference();
		void setVertexColor(int v, int c);
		QMap<int,int> &getColorList(); 
		void setColorList(QMap<int,int>&);
		int getColorVertex(int);
		QMap<int,int> & returnBFS();
		QMap<int,int> & returnBFSTree();
		QMap<int,int> & returnDFSTree();
		QMap<int,int> & returnDFSS();
		QMap<int,int> & returnDFSE();
		void BFSMark(int e, int v);
		void addDFSTree(int, int);
	private:
		QMap < int, QVector< int > > graph;
		QMap < int, QVector< int > >::iterator currentVertex;
		QMap < int, int > BFSData;
		QMap < int, int > BFSTree;
		QMap < int, int > DFSTree;
		QMap < int, int > DFSDataStart;
		QMap < int, int > DFSDataEnd;
		QMap < int, int > graphColored;
		QVector< int >::iterator currentAdjacent; //Iterator do BFS i DFS
		QVector< int >::iterator adjEnd; //U¿yty tylko raz
		int DCounter;
		int BCounter;
		MGraph *undirected;
		MGraph *transposed;
};

#endif