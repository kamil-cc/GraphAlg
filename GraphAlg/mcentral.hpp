#ifndef _MCENTRAL_HPP
#define _MCENTRAL_HPP
#include <QtGui>
#include "mengine.hpp"
#include "ioactions.hpp"
#undef MYDEBUG

extern MEngine *engine;

class MCentral : public QWidget{

	Q_OBJECT
	
	public:
		MCentral();
		void tmpDisp(QString message); //Tymczasowe wy�wietlanie na tymczasowym elemencie
		void clearTmpDisp(); //Czy�ci tymczasowe wyj�cie
		void writeMessage(QString message); //W�witela info na dolnym komponencie
		QString readMessage(); //Pobiera zawarto�� z elementu textEdit
		void clearMessage(); //Czy�ci komunikaty z dolnego komponentu
		void addToList(QString name); //Dodaje do comboboxa nazw� i index grafu
		void closeCurrent(); //Usuwa z pami�ci bie��cy graf
		bool isAlreadyOpened(QString fileName); //
		IOActions *ioActions;
	private slots:
		void repaintGraph(QString name); //Rysuje i od�wie�a graf o wskazanej nazwie
		void repaintGraph(); //Od�wie�a bie��cy graf
		void enableSaveRestore();//Uaktywnia przyciski
		void disableSaveRestore(); //Dezaktywuje przyciski Save i Restore 
		void restoreClicked();
		void saveClicked();
	private:
		QTextEdit *mainTextEdit; //Tymczasowy ZAST�PI� JAKIM� KOMPONENTEM DO RYSOWANIA - lub setReadOnly(true);	
		QTextEdit *textEdit; //TU MO�E JAKI� INNY WIDGET DO WYPISYWANIA INFO NP: Graf jest acykliczny,...,znaleziono cykl hamiltona itp.
		QPushButton *refreshButton; //Od�wie�a graf, kasuje rezultaty oblicze� na grafie
		QPushButton *saveButton; //Od�wie�a graf, kasuje rezultaty oblicze� na grafie
		QPushButton *restoreButton; //Od�wie�a graf, kasuje rezultaty oblicze� na grafie
		QComboBox *combo; //Element do wyboru bie��cego grafu (jednego z kilku w pami�ci)
		QVBoxLayout *layout; //Layout dla Widgetu MCentral
		QGroupBox *horizontalGroupBox;
};

#endif