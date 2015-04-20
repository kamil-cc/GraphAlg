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
		void tmpDisp(QString message); //Tymczasowe wyœwietlanie na tymczasowym elemencie
		void clearTmpDisp(); //Czyœci tymczasowe wyjœcie
		void writeMessage(QString message); //Wœwitela info na dolnym komponencie
		QString readMessage(); //Pobiera zawartoœæ z elementu textEdit
		void clearMessage(); //Czyœci komunikaty z dolnego komponentu
		void addToList(QString name); //Dodaje do comboboxa nazwê i index grafu
		void closeCurrent(); //Usuwa z pamiêci bie¿¹cy graf
		bool isAlreadyOpened(QString fileName); //
		IOActions *ioActions;
	private slots:
		void repaintGraph(QString name); //Rysuje i odœwie¿a graf o wskazanej nazwie
		void repaintGraph(); //Odœwie¿a bie¿¹cy graf
		void enableSaveRestore();//Uaktywnia przyciski
		void disableSaveRestore(); //Dezaktywuje przyciski Save i Restore 
		void restoreClicked();
		void saveClicked();
	private:
		QTextEdit *mainTextEdit; //Tymczasowy ZAST¥PIÆ JAKIMŒ KOMPONENTEM DO RYSOWANIA - lub setReadOnly(true);	
		QTextEdit *textEdit; //TU MO¯E JAKIŒ INNY WIDGET DO WYPISYWANIA INFO NP: Graf jest acykliczny,...,znaleziono cykl hamiltona itp.
		QPushButton *refreshButton; //Odœwie¿a graf, kasuje rezultaty obliczeñ na grafie
		QPushButton *saveButton; //Odœwie¿a graf, kasuje rezultaty obliczeñ na grafie
		QPushButton *restoreButton; //Odœwie¿a graf, kasuje rezultaty obliczeñ na grafie
		QComboBox *combo; //Element do wyboru bie¿¹cego grafu (jednego z kilku w pamiêci)
		QVBoxLayout *layout; //Layout dla Widgetu MCentral
		QGroupBox *horizontalGroupBox;
};

#endif