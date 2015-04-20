#include "ioactions.hpp"

extern QString filePath;

IOActions::IOActions(){
	#ifdef MYDEBUG	
		qDebug()<<"Startuje konstruktor IOActions::IOActions()";
	#endif
	modified = false;
	lockedModifications = true;
	#ifdef MYDEBUG	
		qDebug()<<"Konczy sie konstruktor IOActions::IOActions()";
		qDebug()<<"Modiified = "<< modified;
		qDebug()<< "lockedModifications = " << lockedModifications;
	#endif
}

void IOActions::saveSilent(QString fileContent){
	#ifdef MYDEBUG	
		qDebug()<<"IOActions::saveSilent()";
		qDebug()<< filePath;
	#endif
	if (filePath != "") {
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly)) {
			#ifdef MYDEBUG
				qDebug() << "Nie mogê zapisaæ do pliku!";
			#endif
			return;
		} else {
			QTextStream stream(&file);
			stream << fileContent; //Zapisywanie
			stream.flush();
			file.close();
		}
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy IOActions::saveSilent()";
	#endif
}

void IOActions::modify(){
	if(!lockedModifications){
		modified = true;
		#ifdef MYDEBUG
			qDebug() << "Modified";
		#endif
	}
}

bool IOActions::isLocked(){
	return lockedModifications;
}

void IOActions::unlockModifications(){
	#ifdef MYDEBUG
		qDebug() << "unlockModifications()";
	#endif
	lockedModifications = false;
}

void IOActions::lockModifications(){
	#ifdef MYDEBUG
		qDebug() << "lockModifications()";
	#endif
	lockedModifications = true;
}
