#ifndef _MENGINE_HPP
#define _MENGINE_HPP
#include <QtGui>
#include "mgraph.hpp"
#include <climits> //Tymczasowo - na czas testów parsera
#undef MYDEBUG

class MEngine : public QObject{

	Q_OBJECT
	
	public:
		MEngine(); 
		void parseTextStream(QString fileName, QString fileContent); //Parsuje string z plikiem wejœciowym i alokuje w pamiêci
		QString graphToString(); //Przekszta³ca graf na String
		QString BFS(int start);//Algorytmy
		QString DFS(int start);
		QString transposition();
		QString toUndirected();
		QString stronglyConnected(); //Znajdowanie silnie spojnych skladowych	
		QString graphColoring();
		void clear(); //Czyœci wszystkie wyniki dla danego grafu. Zostawia tylko dane wejœciowe
		void destroyCurrentGraph(); //Kasuje z pamiêci bie¿¹cy graf
		void setCurrentGraph(QString name); //Ustawia iterator na bie¿¹cy graf
		QString dispResults();
		void setFirstValues(); //Ustawia iteratory na pocz¹tek
		int getNextVertex();//Funkcje potrzebne do Rysowania grafu
		int getNextEdge();
		QMap<int,int> &getVColorList(); 
		QMap<int,int> & returnBFSL();
		QMap<int,int> & returnDFSSL();
		QMap<int,int> & returnDFSEL();
		QMap<int,int> & returnBFSTreeE();
		QMap<int,int> & returnDFSTreeL();
	private:
		void BFSinternal(int start);
		void DFSinternal(int start);
		QMap < QString, MGraph* > mainMemory; //Tablica hashuj¹ca z grafami
		QMap < QString, MGraph* >::iterator currentGraph; //Iterator wskazuj¹cy na bie¿¹cy graf
		MGraph *graph; //U¿ywany jako wskaŸnik pomocniczy
		QString DFSOrder(QMap<int, int>&, MGraph*);
		void DFSinternalOrder(QMap<int, int>&, int);
};


#endif
