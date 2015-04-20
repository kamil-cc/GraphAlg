#include "mwindow.hpp"

MEngine *engine; //DEKLARACJA CHYBA POWINNA BYÆ W PLIKU NAG£ÓWKOWYM, ALE WTEDY SIÊ NIE KOMPILUJE 
//ZAMIENIÆ SAVE NA SAVE RESULTS ORAZ SAVE GRAPH

QString filePath; //Œcie¿ka do pliku

//Do zrobienia(?): przerzuciæ obs³ugê slotów (open, close) do engine'u?
MWindow::MWindow(){
	#ifdef MYDEBUG
		qDebug() << "Startuje konstruktor MWindow()";
	#endif
	//Tworzenie silnika
	engine = new MEngine;
	//Akcje do menu File
	openAction = new QAction(tr("&Otworz graf"), this);
	closeAction = new QAction(tr("&Zamknij graf"), this);
	saveOneAction = new QAction(tr("&Zapisz rezultat"), this);
	//saveAllAction = new QAction(tr("Sa&ve all"), this);
	newWindowAction = new QAction(tr("&Rysuj!"), this); //Tworzy nowe okienko
	exitAction = new QAction(tr("Wyjscie"), this);
	//Akcje do menu Algorithm
	BFSAction = new QAction(tr("BFS"), this);
	DFSAction = new QAction(tr("DFS"), this);
	transposeAction = new QAction(tr("Transponuj"), this);
	toUdirectedAction = new QAction(tr("Symetryzuj"), this);
	stronglyConnectedAction = new QAction(tr("Silnie spojne skladowe"), this);
	graphColoringAction = new QAction(tr("Kolorowanie wierzcholkowe"), this);
	//Akcje do menu About
	displayHelpAction = new QAction(tr("&Pomoc"), this);
	displayAuthorsAction = new QAction(tr("Autorzy"), this);
	displayAboutQtAction = new QAction(tr("Info o Qt"), this);
	//
	closeAction->setEnabled(false);
	saveOneAction->setEnabled(false);
	//saveAllAction->setEnabled(false);
	newWindowAction->setEnabled(false);
	BFSAction->setEnabled(false);
	DFSAction->setEnabled(false);
	transposeAction->setEnabled(false);
	toUdirectedAction->setEnabled(false);
	displayHelpAction->setEnabled(false);
	stronglyConnectedAction->setEnabled(false);
	graphColoringAction->setEnabled(false);
	//
	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(closeAction, SIGNAL(triggered()), this , SLOT(closeGraph()));
	connect(saveOneAction, SIGNAL(triggered()), this, SLOT(save()));
	connect(newWindowAction, SIGNAL(triggered()), this, SLOT(openNewWindow()));
	connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));
	//
	connect(BFSAction, SIGNAL(triggered()), this, SLOT(BFS()));
	connect(DFSAction, SIGNAL(triggered()), this, SLOT(DFS()));
	connect(transposeAction, SIGNAL(triggered()), this, SLOT(transpose()));
	connect(toUdirectedAction, SIGNAL(triggered()), this, SLOT(toUndirected()));
	connect(stronglyConnectedAction, SIGNAL(triggered()), this, SLOT(stronglyConnected()));
	connect(graphColoringAction, SIGNAL(triggered()), this, SLOT(graphColoring()));
	//countOrderAction = new QAction(tr("Count order"), this);
	//checkAdjacencyAction = new QAction(tr("Check adjacency"), this);
	//isSubgraphAction = new QAction(tr("Is subgraph"), this);
	connect(displayAuthorsAction, SIGNAL(triggered()), this, SLOT(authors()));
	connect(displayAboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	//
	//Dodawanie akcji do MenuBar
	fileMenu = menuBar()->addMenu(tr("&Plik")); 
	fileMenu->addAction(openAction);
	fileMenu->addAction(saveOneAction);
	fileMenu->addAction(newWindowAction);
	fileMenu->addAction(closeAction);
	//fileMenu->addAction(saveAllAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAction);
	algorithmMenu = menuBar()->addMenu(tr("Algorytmy"));
	//algorithmMenu->addAction(countOrderAction);
	algorithmMenu->addAction(BFSAction);
	algorithmMenu->addAction(DFSAction);
	algorithmMenu->addSeparator();
	algorithmMenu->addAction(transposeAction);
	algorithmMenu->addAction(toUdirectedAction);
	//algorithmMenu->addAction(stronglyConnectedAction);
	algorithmMenu->addSeparator();
	algorithmMenu->addAction(graphColoringAction);
	//algorithmMenu->addAction(checkAdjacencyAction);
	//algorithmMenu->addAction(isSubgraphAction);
	aboutMenu = menuBar()->addMenu(tr("O programie"));
	aboutMenu->addAction(displayHelpAction);
	aboutMenu->addAction(displayAuthorsAction);
	aboutMenu->addAction(displayAboutQtAction);
	//
	statusBarPtr = statusBar();// Ustawia pasek stanu
	statusBarPtr->showMessage(tr("Gotowy")); //POPRACOWAÆ NAD TYM NP DODAÆ WID¯ET QPROGRESSBAR LUB QLABEL
	//
	centralWidgetPtr = new MCentral;
	setCentralWidget(centralWidgetPtr);	
	//
	secondWindow = NULL; //Inicjalizacja wskaŸnika na nowe okno
	//
	msgBox.setWindowTitle("Autor");
	msgBox.setText(tr("Autor: \nKamil Burzynski.\nAlgorytmy na Grafach 2012\nUKSW"));
	//
	applicationName = tr("AGR v 0.4");
	setWindowTitle(applicationName);
	setMinimumSize(400, 400);
	//
	opened = 0;
	//
	options = NULL;
	#ifdef MYDEBUG
		qDebug() << "Konczy konstruktor MWindow()";
	#endif
}

void MWindow::quit(){
	/*#ifdef MYDEBUG
		qDebug() << "Startuje MWindow::quit()";
	#endif
	//
	if( QMessageBox::question(
				this,
				tr("Wyjscie?"),
				tr("Czy na pewno chcesz wyjsc z programu ") + applicationName,
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::No
				) == QMessageBox::Yes
			)
	{
		#ifdef MYDEBUG
			qDebug() << "Konczy sie MWindow::quit()";
		#endif*/
		qApp->quit();
	//}
}

void MWindow::open(){ //Nie mo¿na otwieraæ wielu plików o tej samej nazwie
	#ifdef MYDEBUG
		qDebug() << "Startuje MWindow::open()";
	#endif
	//
	filePath = QFileDialog::getOpenFileName(this, tr("Otworz plik"), "",
	tr("Lista incydencji (*.agi)"));//Daæ w³aœciwe nazwy!!! przet³umaczyæ w³aœciwie //;;Adjacency matrix (*.ags);;Multi graph (binary) (*.agb)
	//
	if (filePath != "") {
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly)) { 
			#ifdef MYDEBUG
				qDebug() << "Nie mogê otworzyæ pliku!";
			#endif
			QMessageBox::critical(this, tr("Blad"), tr("Nie moge otworzyc pliku"));
			return;
		}
		fileContent.clear();
		QTextStream in(&file);//Czytanie do strumienia tekstowego
        fileContent = in.readAll();
        file.close();
		QString slash = "/";
		int position = filePath.lastIndexOf(slash);
		fileName = filePath.right((filePath.size() - position) - 1); //Wyci¹gniêcie nazwy pliku ze œcie¿ki
		if(centralWidgetPtr->isAlreadyOpened(fileName)){
			QMessageBox::warning(this, tr("Uwaga"), tr("Plik zostal juz otwarty wczesniej"));
			return;
		}
		engine->parseTextStream(fileName, fileContent); //Parsowanie pliku i dodawanie do pamiêci
		centralWidgetPtr->addToList(fileName); //Ustawianie odpowiedniego elementu w combobox
		++opened;
		enableActions();
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MWindow::open()";
	#endif
}


void MWindow::save(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MWindow::save()";
	#endif
	//
	QString filePath = QFileDialog::getSaveFileName(this, tr("Zapisz plik"), "",
	tr("Plik tekstowy (*.txt)")); //Uwagi takie jak do f-kcji open, oraz dodaæ domyœln¹ nazwê zapisywanego pliku
	//
	if (filePath != "") {
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly)) {
			#ifdef MYDEBUG
				qDebug() << "Nie mogê zapisaæ do pliku!";
			#endif
			QMessageBox::critical(this, tr("Blad!"), tr("Nie moge zapisac pliku")); //Tu daæ jak¹œ inn¹ wiadomoœæ (np na statusBar)
			return;
		} else {
			contentToSave.clear();
			QTextStream stream(&file);
			contentToSave = engine->graphToString(); //Przepisywanie grafu i wyników do stringa
			contentToSave += centralWidgetPtr->readMessage();
			stream << contentToSave; //Zapisywanie
			stream.flush();
			file.close();
		}
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy MWindow::save()";
	#endif
}

void  MWindow::closeGraph(){
	#ifdef MYDEBUG
		qDebug()<<"Startuje MWindow::closeGraph()";
	#endif
	if( QMessageBox::question(
				this,
				tr("Potwierdz"),
				tr("Czy na pewno zamknac biezacy graf?\n Wszystkie niezapisane dane zostana utracone."),
				QMessageBox::Yes | QMessageBox::No,
				QMessageBox::No
				) == QMessageBox::Yes
			)
	{
		engine->destroyCurrentGraph();
		#ifdef MYDEBUG
			qDebug()<<"MWindow::closeGraph(): zniszcono biezacy graf";
		#endif
		centralWidgetPtr->closeCurrent();
		#ifdef MYDEBUG
			qDebug()<<"MWindow::closeGraph(): usunieto element z combobox";
		#endif
		if(--opened == 0){
			disableActions();
		}
	}
}

void MWindow::DFS(){
	bool ok;
    int start = QInputDialog::getInteger(this, tr("Podaj liczbe"),
                                          tr("Prosze podac wierzcholek startowy:"), 1, 0, (INT_MAX - 1), 1, &ok);//Poprawiæ ograniczenia min/max
	if(ok){
		QString result = engine->DFS(start);
		if(result == ""){
			centralWidgetPtr->writeMessage(engine->dispResults());
		}else{
			centralWidgetPtr->writeMessage(result);
		}
	}
}

void MWindow::BFS(){
	bool ok;
    int start = QInputDialog::getInteger(this, tr("Podaj liczbe"),
                                          tr("Prosze podac wierzcholek startowy:"), 1, 0, (INT_MAX - 1), 1, &ok);//Poprawiæ ograniczenia min/max
	if(ok){
		QString result = engine->BFS(start);
		if(result == ""){
			centralWidgetPtr->writeMessage(engine->dispResults());
		}else{
			centralWidgetPtr->writeMessage(result);
		}
	}
}

void  MWindow::transpose(){/*Poprawiæ wyœwietlanie wyników*/
	QString result = engine->transposition();
	if(result == ""){
		centralWidgetPtr->writeMessage(engine->dispResults());
	}else{
		centralWidgetPtr->writeMessage(result);
	}
}

void MWindow::toUndirected(){
	QString result = engine->toUndirected();
	if(result == ""){
		centralWidgetPtr->writeMessage(engine->dispResults());
	}else{
		centralWidgetPtr->writeMessage(result);
	}
}

void MWindow::graphColoring(){
	QString result = engine->graphColoring();
	if(result == ""){
		centralWidgetPtr->writeMessage(engine->dispResults());
	}else{
		centralWidgetPtr->writeMessage(result);
	}
}

void MWindow::stronglyConnected(){
	QString result = engine->stronglyConnected();
	if(result == ""){
		centralWidgetPtr->writeMessage(engine->dispResults());
	}else{
		centralWidgetPtr->writeMessage(result);
	}
}

void MWindow::authors(){
	msgBox.exec();
}

void MWindow::enableActions(){
	closeAction->setEnabled(true);
	saveOneAction->setEnabled(true);
	//saveAllAction->setEnabled(true);
	newWindowAction->setEnabled(true);
	BFSAction->setEnabled(true);
	DFSAction->setEnabled(true);
	transposeAction->setEnabled(true);
	toUdirectedAction->setEnabled(true);
	stronglyConnectedAction->setEnabled(true);
	graphColoringAction->setEnabled(true);
}

void MWindow::disableActions(){
	closeAction->setEnabled(false);
	saveOneAction->setEnabled(false);
	//saveAllAction->setEnabled(false);
	newWindowAction->setEnabled(false);
	BFSAction->setEnabled(false);
	DFSAction->setEnabled(false);
	transposeAction->setEnabled(false);
	toUdirectedAction->setEnabled(false);
	stronglyConnectedAction->setEnabled(false);
	graphColoringAction->setEnabled(false);
}

void MWindow::closeEvent(QCloseEvent *event){ //Poprawiæ
	event->ignore();
	quit();
	qDebug()<<"Tego nie powinno byc widac";
    //QMainWindow::closeEvent(event);
}
 
 void MWindow::openNewWindow(){ //Poprawiæ to i przenieœæ do mcentral
	#ifdef MYDEBUG
		qDebug()<<"Startuje MWindow::openNewWindow()";
	#endif
	//zbiera opcje
	options = new int[10];
	//
	QStringList items; //Lista layoutu
    items << tr("TreeLayout") << tr("OrthoLayout") << tr("FastHierarchyLayout") << tr("EnergyLayout")<<tr("PlanarStraightLayout");
	//
    bool ok;
    QString item = QInputDialog::getItem(this, tr("Prosze wybrac wyglad grafu:"),
                                         tr("Wyglad:"), items, 1, false, &ok);
	QString result;
    if (ok && !item.isEmpty()){
		if(item == "TreeLayout")
				options[0] = 1;
		else if(item == "OrthoLayout")
				options[0] = 2;
		else if(item == "FastHierarchyLayout")
				options[0] = 3;
		else if(item == "EnergyLayout")
				options[0] = 4;
		else if(item == "PlanarStraightLayout")
				options[0] = 5;
	}else{
		return;
	}
	//
	int vertex;
	int adjacent;
	if(secondWindow != NULL){ //Czyœci poprzednie dane
		secondWindow->close();
		secondWindow=NULL;
	}
	secondWindow = new MPaint(this, options);
	/////////////////////////////////Przekazuje parametry
	engine->setFirstValues();
	while((vertex = engine-> getNextVertex()) != INT_MAX){
		#ifdef MYDEBUG
			qDebug()<<"V: "<<vertex;
		#endif
		secondWindow->addVertex(vertex);
	}
	engine->setFirstValues();
	while((vertex = engine-> getNextVertex()) != INT_MAX){
		#ifdef MYDEBUG
			qDebug()<<"V: "<<vertex;
		#endif
		while((adjacent = engine-> getNextEdge()) != INT_MAX){
			#ifdef MYDEBUG
				qDebug()<<"E: "<<adjacent;
			#endif
			secondWindow->addEdge(vertex, adjacent);
		}
	}
	//
	secondWindow->assignColors(engine->getVColorList());
	secondWindow->assignBFS(engine->returnBFSL());
	secondWindow->assignDFS(engine->returnDFSSL() , engine->returnDFSEL());
	secondWindow->assignBFSTree(engine->returnBFSTreeE());
	secondWindow->assignDFSTree(engine->returnDFSTreeL());
	/////////////////////////////////
	secondWindow->drawIt(); //Rysuje graf
	secondWindow->show(); //Pokazuje okienko
	secondWindow->activateWindow(); //Aktywuje okno
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie MWindow::openNewWindow()";
	#endif
 }
 