#ifndef _MWINDOW_HPP
#define _MWINDOW_HPP
#include <QtGui> //PODAÆ SZCZEGÓ£OWE NAG£ÓWKI?
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
	void open(); //Formaty plików: .agi - incydencja, .ags - s¹siedztwa, .agr - format binarny pliku agr (wiele grafów, serializacja itp, komresja etc..)
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
	//QAction *countOrderAction; //Liczy rz¹d grafu
	//QAction *checkAdjacencyAction; //Sprawdza czy pmiêdzy podanymi dwoma wierzcho³kami zachodzi relacja s¹siedztwa
	//QAction *isSubgraphAction; //Czy dany graf jest podgrafem innego grafu
	//Liczy stopieñ wskazanego wierzcho³ka
	//Licz stopieñ grafu - maksymalny stopieñ wierzcho³ka
	//Liczy iloœæ krawêdzi
	//SprawdŸ czy graf jest pe³ny
	//SprawdŸ czy graf jest regularny
	//Czy ci¹g jest graficzny
	//
	QAction *displayHelpAction; 
	QAction *displayAuthorsAction; 
	QAction *displayAboutQtAction; //Wyœwietla okienko o Qt
	//
	QAction *newWindowAction;
	//
	QMenu *fileMenu;
	QMenu *algorithmMenu;
	QMenu *aboutMenu;
	QStatusBar *statusBarPtr;
	MCentral *centralWidgetPtr; //Centralny wid¿et
	QMessageBox msgBox; //Info o autorach
	//
	QString fileName; //Nazwa otwartego pliku
	QString applicationName; //Nazwa aplikacji
	QString title; //Tytu³ okna
	QString fileContent; //Zawartoœæ pliku odczytywanego
	QString contentToSave; //Zawartoœæ pliku, do którego piszemy
	MPaint *secondWindow; //MDialog
	
	int *options; //Opcje do rysowania layoutu
	
private:
	int opened;//Iloœæ otwartych plików
	void enableActions();//Uaktywnia menu po otwarciu jakiegoœ pliku
	void disableActions();//Blokuje niektóre menu po zamkniêciu wszystkich plików
	void closeEvent(QCloseEvent *event); //Zamykanie okna z potwierdzeniem
};

#endif
