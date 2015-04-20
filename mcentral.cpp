#include "mcentral.hpp"

extern QString filePath;

MCentral::MCentral(){
	//
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::MCentral()";
	#endif
	//
	ioActions = new IOActions();
	combo = new QComboBox; //
	combo-> setEnabled(false);
	textEdit = new QTextEdit(tr("Tu wyswietlaja sie rezultaty"));
	mainTextEdit = new QTextEdit(tr("Tu wyswietla sie graf"));
	refreshButton = new QPushButton(tr("Wyczysc rezultaty"));
	refreshButton->setEnabled(false);
	saveButton = new QPushButton(tr("Zapisz zmiany"));
	saveButton->setEnabled(false);
	restoreButton = new QPushButton(tr("Anuluj zmiany"));
	restoreButton->setEnabled(false);
	//
	connect(refreshButton, SIGNAL(clicked()), this, SLOT(repaintGraph())); //Obs³uga przycisku odœwie¿
	connect(combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(repaintGraph(QString))); //Obs³uga comboboxa
	connect(combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(disableSaveRestore())); //
	connect(mainTextEdit, SIGNAL(textChanged()), ioActions,  SLOT(modify()));
	connect(mainTextEdit, SIGNAL(textChanged()), this,  SLOT(enableSaveRestore()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(disableSaveRestore()));
	connect(restoreButton, SIGNAL(clicked()), this, SLOT(restoreClicked()));
	connect(restoreButton, SIGNAL(clicked()), this, SLOT(disableSaveRestore()));
	//
	horizontalGroupBox = new QGroupBox(tr("Opcje edycji:"));
	QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(saveButton);
    horizontalLayout->addWidget(restoreButton);
	horizontalLayout->addWidget(refreshButton);
    horizontalGroupBox->setLayout(horizontalLayout);
	//
	layout = new QVBoxLayout; 
	layout->addWidget(combo);
	layout->addWidget(mainTextEdit);
	layout->addWidget(horizontalGroupBox);
	layout->addWidget(textEdit);
	//layout->addStretch(); //to nie daje ¿adnych zmian
	setLayout(layout); 
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::MCentral()";
	#endif
}

void MCentral::tmpDisp(QString message){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::tmpDisp(" << message <<")";
	#endif
	mainTextEdit->setText(message);
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::tmpDisp(" << message <<")";
	#endif
}

void MCentral::clearTmpDisp(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::clearTmpDisp()";
	#endif
	mainTextEdit->clear();
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::clearTmpDisp()";
	#endif
}

void MCentral::writeMessage(QString message){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::writeMessage(" << message <<")";
	#endif
	textEdit->setText(message);
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::writeMessage(" << message <<")";
	#endif
}

QString  MCentral::readMessage(){
	return textEdit->toPlainText();
}

void MCentral::clearMessage(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::clearMessage()";
	#endif
	textEdit->clear();
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::clearMessage()";
	#endif
}

void MCentral::addToList(QString name){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::addToList(" << name <<")";
	#endif
	if(!combo->isEnabled()){
		combo->setEnabled(true);
	}
	if(!refreshButton->isEnabled()){
		refreshButton->setEnabled(true);
	}
	combo->addItem(name);
	int index = combo->findText(name); //Ustawianie bie¿¹cego elementu na w³aœnie otwarty plik
	combo->setCurrentIndex(index);
	ioActions->unlockModifications();
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::addToList(" << name <<")";
	#endif
}

void MCentral::repaintGraph(QString name){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::repaintGraph("<<name<<")";
	#endif
	clearMessage(); 
	clearTmpDisp();
	if(name != ""){ //Nie wchodzi do ifa gdy zamykamy ostatni graf
		engine->setCurrentGraph(name); 
		tmpDisp(engine->graphToString());
		writeMessage(engine->dispResults());
	}
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::repaintGraph("<<name<<")";
	#endif
}

void MCentral::repaintGraph(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::repaintGraph()";
	#endif
	clearMessage(); 
	//clearTmpDisp(); //To wykomentowac jezeli przycisk "wyczysc rezultat" ma nie czyscic ostatnich edycji
	engine->clear(); //Uwaga!!! kasuje niezapisane rezultaty
	//tmpDisp(engine->graphToString()); //To wykomentowac jezeli przycisk "wyczysc rezultat" ma nie czyscic ostatnich edycji
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::repaintGraph()";
	#endif
}

void MCentral::closeCurrent(){
	#ifdef MYDEBUG
		qDebug()<<"MCentral::closeCurrent()";
	#endif
	if(combo->count() > 1){
		combo->removeItem(combo->currentIndex()); //Usuwa graf z combobox
	}else{ //Usuwamy ostatni element
		combo->clear();
		combo->setEnabled(false);
		refreshButton->setEnabled(false);
	}
	#ifdef MYDEBUG
		qDebug()<<"Po usunieciu currentIndex";
	#endif
}

bool MCentral::isAlreadyOpened(QString fileName){
	int tmp = -1;
	QString name = fileName.trimmed();
	if(combo->isEnabled()){
		tmp = combo->findText(name, Qt::MatchExactly);
	}
	if(tmp == -1){
		return false;
	}
	return true;
}

void MCentral::enableSaveRestore(){
	#ifdef MYDEBUG
		qDebug()<<"enableSaveRestore()";
	#endif
	if(!(ioActions->isLocked())){
		saveButton->setEnabled(true);
		restoreButton->setEnabled(true);
	}else{
		saveButton->setEnabled(false);
		restoreButton->setEnabled(false);
	}
}

void MCentral::disableSaveRestore(){
	#ifdef MYDEBUG
		qDebug()<<"disableSaveRestore()";
	#endif
	saveButton->setEnabled(false);
	restoreButton->setEnabled(false);
	if(combo->count() == 0){
		#ifdef MYDEBUG
			qDebug()<<"Locking modifications: " << combo->count();
		#endif
		ioActions->lockModifications();
	}
}

void MCentral::restoreClicked(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::repaintGraph()";
	#endif 
	clearTmpDisp(); 
	tmpDisp(engine->graphToString()); 
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MCentral::repaintGraph()";
	#endif
}

void MCentral::saveClicked(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MCentral::saveClicked()";
	#endif
	disableSaveRestore();
	QString fileContent = 	mainTextEdit->toPlainText();
	#ifdef MYDEBUG
		qDebug() << "Nowa tresc: "<< fileContent;
	#endif
	#ifdef MYDEBUG
		qDebug()<<"MCentral::saveClicked() Niszczy stary graf";
	#endif
	engine->destroyCurrentGraph();
	#ifdef MYDEBUG
		qDebug()<<"MCentral::saveClicked() Zamyka biezacy graf";
	#endif
	closeCurrent();
	#ifdef MYDEBUG
		qDebug()<<"MCentral::saveClicked() Zamknal graf";
	#endif
	#ifdef MYDEBUG
		qDebug() << "Zapisuje nowa tresc";
	#endif
	ioActions->saveSilent(fileContent);
	//
	#ifdef MYDEBUG
		qDebug() << "MCentral::saveClicked() Otwiera ponownie plik";
	#endif
	if (filePath != "") {
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) { 
			#ifdef MYDEBUG
				qDebug() << "Nie mogê otworzyæ pliku!";
			#endif
			return;
		}
		fileContent.clear();
		QTextStream in(&file);//Czytanie do strumienia tekstowego
        fileContent = in.readAll();
        file.close();
		QString slash = "/";
		int position = filePath.lastIndexOf(slash);
		QString fileName = filePath.right((filePath.size() - position) - 1); //Wyci¹gniêcie nazwy pliku ze œcie¿ki
		if(isAlreadyOpened(fileName)){
			#ifdef MYDEBUG
				qDebug() <<"Plik zostal juz otwarty wczesniej";
			#endif
			return;
		}
		engine->parseTextStream(fileName, fileContent); //Parsowanie pliku i dodawanie do pamiêci
		addToList(fileName); //Ustawianie odpowiedniego elementu w combobox
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MWindow::open()";
	#endif
}
