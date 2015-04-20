#ifndef _IOACTIONS_HPP
#define _IOACTIONS_HPP
#include <QtGui>
#undef MYDEBUG

class IOActions : public QObject{

	Q_OBJECT
	
	public:
		IOActions();
		bool isLocked(); //To nie musi by� slot, to mo�e by� zwyk�a funkcja
		void unlockModifications();
		void lockModifications();
		void saveSilent(QString); //Zapisuje wynik
	public slots:
		void modify();	
	public:
		int i;
	private:
		bool modified;
		bool lockedModifications;
};

#endif