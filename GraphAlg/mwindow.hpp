#ifndef _MWINDOW_HPP
#define _MWINDOW_HPP
#include <QtGui> //PODA� SZCZEGӣOWE NAG��WKI?
#include "mcentral.hpp"
#include "mengine.hpp"
#include "mpaint.hpp"
#include <climits> //Tymczasowo
#undef MYDEBUG

class MWindow : public QMainWindow{

	Q_OBJECT

public:
	MWindow();
	
private slots: 
	void open(); //Formaty plik�w: .agi - incydencja, .ags - s�siedztwa, .agr - format binarny pliku agr (wiele graf�w, serializacja itp, komresja etc..)
	void save();
	void closeGraph();
	void quit();
	void DFS();
	void BFS();
	void transpose();
	void toUndirected();
	void stronglyConnected();
	void graphColoring();
	void authors();
	void openNewWindow(); //Otwiera nowe okno
	
private:
	QAction *openAction;//Zaimplementowana
	QAction *closeAction;//Zaimplementowana
	QAction *saveOneAction;//Zaimplementowana
	//QAction *saveAllAction;
	QAction *exitAction;//Zaimplementowana
	//
	QAction *DFSAction;
	QAction *BFSAction;
	QAction *transposeAction;
	QAction *toUdirectedAction;
	QAction *stronglyConnectedAction;
	QAction *graphColoringAction;
	//QAction *countOrderAction; //Liczy rz�d grafu
	//QAction *checkAdjacencyAction; //Sprawdza czy pmi�dzy podanymi dwoma wierzcho�kami zachodzi relacja s�siedztwa
	//QAction *isSubgraphAction; //Czy dany graf jest podgrafem innego grafu
	//Liczy stopie� wskazanego wierzcho�ka
	//Licz stopie� grafu - maksymalny stopie� wierzcho�ka
	//Liczy ilo�� kraw�dzi
	//Sprawd� czy graf jest pe�ny
	//Sprawd� czy graf jest regularny
	//Czy ci�g jest graficzny
	//
	QAction *displayHelpAction; 
	QAction *displayAuthorsAction; 
	QAction *displayAboutQtAction; //Wy�wietla okienko o Qt
	//
	QAction *newWindowAction;
	//
	QMenu *fileMenu;
	QMenu *algorithmMenu;
	QMenu *aboutMenu;
	QStatusBar *statusBarPtr;
	MCentral *centralWidgetPtr; //Centralny wid�et
	QMessageBox msgBox; //Info o autorach
	//
	QString fileName; //Nazwa otwartego pliku
	QString applicationName; //Nazwa aplikacji
	QString title; //Tytu� okna
	QString fileContent; //Zawarto�� pliku odczytywanego
	QString contentToSave; //Zawarto�� pliku, do kt�rego piszemy
	MPaint *secondWindow; //MDialog
	
	int *options; //Opcje do rysowania layoutu
	
private:
	int opened;//Ilo�� otwartych plik�w
	void enableActions();//Uaktywnia menu po otwarciu jakiego� pliku
	void disableActions();//Blokuje niekt�re menu po zamkni�ciu wszystkich plik�w
	void closeEvent(QCloseEvent *event); //Zamykanie okna z potwierdzeniem
};

#endif
