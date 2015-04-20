#include "mengine.hpp"

//Do zrobienia: wyrzuciæ parser do zewnêtrznego pliku .cpp ???

MEngine::MEngine(){
	#ifdef MYDEBUG
	qDebug() << "Startuje MEngine::MEngine()";
	#endif
	currentGraph = NULL;
	//graph = NULL;
	#ifdef MYDEBUG
	qDebug() << "Konczy sie MEngine::MEngine()";
	#endif
}

//Dodaæ funkcjê otwierania kilku plików o tej samej nazwie
void MEngine::parseTextStream(QString fileName, QString fileContent){ //To rozbiæ na kilka mniejszych funkcji
	#ifdef MYDEBUG
		qDebug() << "Startuje MEngine::parseTextStream("<<fileName<<"content";//fileContent<<")";
	#endif
	QQueue <int> queue;
	int lineNumber = 0;
	int vertexNumber;
	int index;
	MGraph *newGraph = new MGraph(); //Tworzenie nowego grafu, czy graf jako wierzcho³ki ma litery?
	QString arrow = "->";
	QString vertex;
	bool *conversionSuccess; 
	QStringList lines = fileContent.split( "\n", QString::SkipEmptyParts);
	foreach(QString line, lines) { //Dodawanie wierzcho³ków
		lineNumber++;
		line = line.trimmed();
		//#ifdef MYDEBUG
		//	qDebug()<<"Zawartosc linii"<<lineNumber<<": "<<line;
		//#endif
		index = line.indexOf(arrow);
		vertex = line.left(index);
		vertex = vertex.trimmed();
		if(vertex.isEmpty()){//Prosta obs³uga b³êdów w pliku
			#ifdef MYDEBUG
				qDebug()<<"BLAD W PLIKU WEJSCIOWYM: Brak wierzcholka w linii:"<<lineNumber;
			#endif
			//Wyjœcie z programu lub z funkcji open
			vertex.setNum(INT_MAX); //Tymczasowo, na czas testów parsera
		}
		conversionSuccess = new bool(true); //lub conversionSuccess = NULL;
		vertexNumber = vertex.toInt(conversionSuccess);
		if(!(*conversionSuccess)){
			#ifdef MYDEBUG
				qDebug()<<"BLAD W PLIKU WEJSCIOWYM: Zly numer wierzcholka w linii:"<<lineNumber;
			#endif
			//Wyjœcie z programu lub z funkcji open
			vertexNumber = INT_MAX; //Tymczasowo, na czas testów parsera
		}
		//#ifdef MYDEBUG
		//	qDebug()<<"Wierzcholek:"<<vertexNumber();
		//#endif
		newGraph->addVertex(vertexNumber);
		queue.enqueue(vertexNumber); //Wstawianie do kolejki ¿eby nie powtarzaæ w nastêpnym foreach
	}
	lineNumber = 0;
	arrow = ">";
	QString edges;
	QStringList edgesList;
	int destinationVertexNumber;
	foreach(QString line, lines) {
		lineNumber++;
		line = line.trimmed();
		vertexNumber = queue.dequeue();
		index = line.indexOf(arrow);
		edges = line.right((line.size() - index) - 1);//Prawa czêœæ stringa
		edges = edges.trimmed();
		#ifdef MYDEBUG
			qDebug()<<"Lista krawedzi od "<<index<<" znaku:"<<edges;
		#endif
		edgesList = edges.split( ",", QString::SkipEmptyParts);
		foreach(QString destinationVertex, edgesList) {//Dodawanie krawêdzi
			destinationVertex = destinationVertex.trimmed();
			if(destinationVertex.isEmpty()){
				//#ifdef MYDEBUG
				//	qDebug()<<"BLAD W PLIKU WEJSCIOWYMW: W linii:"<<lineNumber<<"lista sasiedztwa jest pusta";
				//#endif
				continue;
			}
			conversionSuccess = new bool(true); //lub conversionSuccess = NULL;
			destinationVertexNumber = destinationVertex.toInt(conversionSuccess);
			if(!(*conversionSuccess)){
				#ifdef MYDEBUG
					qDebug()<<"BLAD W PLIKU WEJSCIOWYM: W linii:"<<lineNumber<<"w liscie sasiedztwa sa niedozwolone znaki";
				#endif
				continue;
			}
			newGraph -> addEdge(vertexNumber, destinationVertexNumber);
			#ifdef MYDEBUG
				qDebug()<<"Krawedz od:"<<vertexNumber<<" do:"<<destinationVertexNumber;
			#endif
		}
	}
	//Dodawanie listy s¹siedztwa
	//koniec parsowania
	mainMemory.insert(fileName, newGraph);
	currentGraph = mainMemory.find(fileName); //Ustaw iterator na w³aœnie otwarty plik
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy MEngine::parseTextStream(QTextStream *in)";
	#endif
}

QString MEngine::graphToString(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MEngine::graphToString()";
	#endif
	return currentGraph.value()->graphToString();
}

void MEngine::clear(){
	currentGraph.value()->reset();
}

void MEngine::destroyCurrentGraph(){
	#ifdef MYDEBUG
		qDebug() << "MEngine::destroyCurrentGraph()";
	#endif
	if(!mainMemory.isEmpty()){
		mainMemory.erase(currentGraph);
	}
	if(mainMemory.isEmpty()){
		currentGraph = NULL;
	}
}

void MEngine::setCurrentGraph(QString name){
	#ifdef MYDEBUG
		qDebug()<<"Set current graph";
	#endif
	currentGraph = mainMemory.find(name);
	#ifdef MYDEBUG
		if(currentGraph == mainMemory.end()){
			qDebug() << "MEngine::setCurrentGraph("<< name<<") Mogl (ale nie musial) wystapic blad. Iterator ustawiony na end()";
		}
	#endif
}

/////////////////////////////////////////////////////////////////////////

QString MEngine::BFS(int start){
	#ifdef MYDEBUG
		qDebug() << "Startuje MEngine::BFS("<<start<<")";
	#endif
	QString message; //Wiadomoœæ zwracana z funkcji BFS
	graph = currentGraph.value(); //Ustawia wskaŸnik na bie¿¹cy graf
	graph->clearBFSResult(); //Czyœci poprzedni rezultat
	if(!(graph->contains(start))){ //Kontrola czy podany wierzcho³ek znajduje sie w grafie
		message = tr("BFS:\nWierzcholek %1 nie znajduje sie w grafie!").arg(start);
		return message;
	}
	BFSinternal(start);
	//Poni¿ej dla grafów niespójnych
	QLinkedList<int> &list = graph->getVertexList();
	int vertex;
	foreach (vertex, list){
		#ifdef MYDEBUG
			qDebug() << vertex;
		#endif
		if(!graph->isBFSMarked(vertex)){
			BFSinternal(vertex);
			#ifdef MYDEBUG
				qDebug() <<"Vertex: " << vertex <<" is unmarked";
			#endif
		}
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MEngine::BFS("<<start<<")";
	#endif
	graph->clearAfterAlgorithm();
	return message;
}

void MEngine::BFSinternal(int start){
	#ifdef MYDEBUG
		qDebug()<<"Startuje: MEngine::BFSinternal("<<start<<")";
	#endif
	int vertex;
	int adjacent;
	QQueue<int> queue;
	graph->BFSMark(start);
	queue.enqueue(start);
	#ifdef MYDEBUG
		qDebug()<<"Przed pierwszym while";
	#endif
	while(!queue.isEmpty()){
		vertex = queue.dequeue();
		graph->setCurrentVertex(vertex); //Funkcja wewnêtrzna
		#ifdef MYDEBUG
			qDebug()<<"VERTEX: "<<vertex;
		#endif	
		while((adjacent = graph->nextUnmarkedAdjacentB()) != INT_MAX){
			#ifdef MYDEBUG
				qDebug()<<vertex<<"VVVVVV: "<< vertex <<" ADJACENT: "<<adjacent;
			#endif
			graph->BFSMark(vertex, adjacent);
			queue.enqueue(adjacent);
		}
	}
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie MEngine::BFSinternal("<<start<<")";
	#endif
}

QString MEngine::DFS(int start){
	#ifdef MYDEBUG
		qDebug() << "Startuje MEngine::DFS("<<start<<")";
	#endif
	QString message; //Wiadomoœæ zwracana z funkcji BFS
	graph = currentGraph.value(); //Ustawia wskaŸnik na bie¿¹cy graf
	graph->clearDFSResult();//Czyœci poprzedni rezultat
	if(!(graph->contains(start))){ //Kontrola czy podany wierzcho³ek znajduje sie w grafie
		message = tr("DFS:\nWierzcholek %1 nie znajduje sie w grafie!").arg(start);
		return message;
	}
	DFSinternal(start);
	//Poni¿ej dla grafów niespójnych
	QLinkedList<int> &list = graph->getVertexList();
	int vertex;
	foreach (vertex, list){
		#ifdef MYDEBUG
			qDebug() << vertex;
		#endif
		if(!graph->isDFSMarked(vertex)){
			DFSinternal(vertex);
			#ifdef MYDEBUG
				qDebug() <<"Vertex: " << vertex <<" is unmarked";
			#endif
		}
	}
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MEngine::DFS("<<start<<")";
	#endif
	graph->clearAfterAlgorithm();
	return message;
}

void MEngine::DFSinternal(int start){ //Rekurencyjna
	#ifdef MYDEBUG
		qDebug()<<"Startuje DFSInternal("<< start <<")";
	#endif
	int adjacent;
	graph->DFSMarkStart(start);
	graph->setCurrentVertex(start); //
	while((adjacent = graph->nextUnmarkedAdjacentD()) != INT_MAX){
		#ifdef MYDEBUG
			qDebug()<<"DFS internal adjacent: " << adjacent;
		#endif
		graph->addDFSTree(start, adjacent);
		DFSinternal(adjacent);
		graph->setCurrentVertex(start); //Funkcja wewnêtrza
	}
	graph->DFSMarkEnd(start);
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie DFSInternal("<< start <<")";
	#endif
}

QString MEngine::dispResults(){ //Zwraca w postaci stringa wyniki obliczeñ dokonanych na grafie
	QString result; 
	QString BFSResult = currentGraph.value()->getBFSResult();
	if(BFSResult != "") /*£adne ustawianie rezultatów*/
		result = BFSResult; 
	QString DFSResult = currentGraph.value()->getDFSResult();
	if(DFSResult != "")
		if(result != "")
			result += "\n" + DFSResult;
		else
			result = DFSResult;
	MGraph *transposed = currentGraph.value()->getTransposed();
	if(transposed != NULL)
		if(result != "")
			result += "\nTransponowany:\n" + transposed->graphToString();
		else
			result = "Transponowany:\n" + transposed->graphToString();
	MGraph *undirected = currentGraph.value()->getUndirected();
	if(undirected != NULL)
		if(result != "")
			result += "\nNieskierowany:\n" + undirected->graphToString();
		else
			result = "Nieskierowany:\n" + undirected->graphToString();
	QString res = currentGraph.value()->getColoredResult();
	if(res != ""){
		if(result != "")
			result += tr("\nWynik kolorowania wierzcholkowego\n") + res;
		else
			result = tr("Wynik kolorowania wierzcholkowego\n") + res;
	}
	return result;
}

void MEngine::setFirstValues(){
	currentGraph.value()->setFirstValues();
}

int MEngine::getNextVertex(){
	#ifdef MYDEBUG
		qDebug()<<"Startuje MEngine::getNextVertex()";
	#endif
	int val = currentGraph.value()->nextVertex();
	if(val == INT_MAX){
		currentGraph.value()->clearAfterAlgorithm();
	}
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie MEngine::getNextVertex()";
	#endif
	return val;
}

int MEngine::getNextEdge(){
	return currentGraph.value()->getNextEdge();
}

QString MEngine::transposition(){
	#ifdef MYDEBUG
		qDebug() <<"Startuje MEngine::transposition()";
	#endif	
	MGraph *transposed = new MGraph();
	graph = currentGraph.value(); //!!!!!
	QLinkedList<int> &list = graph->getVertexList();
	int vertex;
	foreach (vertex, list){
		#ifdef MYDEBUG
			qDebug() << ":" << vertex;
		#endif
		transposed->addVertex(vertex);
	}
	int newVertex;
	int newAdjacent = INT_MAX;
	foreach (newVertex, list){
		graph->setCurrentVertex(newVertex);
		while((newAdjacent=graph->nextAdjacent()) != INT_MAX){
			#ifdef MYDEBUG
				qDebug()<<newAdjacent;
			#endif
			transposed->addEdge(newAdjacent, newVertex);
		}
		#ifdef MYDEBUG
			qDebug() << ":" << vertex;
		#endif
	}
	transposed->clearAfterAlgorithm();
	#ifdef MYDEBUG
		qDebug() <<"Konczy sie MEngine::transposition()";
	#endif
	graph->setTransposed(transposed);
	QString empty;
	return empty;
}

QString MEngine::toUndirected(){
	#ifdef MYDEBUG
		qDebug() <<"Startuje MEngine::toUndirected()";
	#endif	
	MGraph *undirected = new MGraph();
	graph = currentGraph.value(); //!!!!!
	QLinkedList<int> &list = graph->getVertexList();
	int vertex;
	foreach (vertex, list){
		#ifdef MYDEBUG
			qDebug() << ":" << vertex;
		#endif
		undirected->addVertex(vertex);
	}
	int newVertex;
	int newAdjacent = INT_MAX;
	foreach (newVertex, list){
		graph->setCurrentVertex(newVertex);
		while((newAdjacent=graph->nextAdjacent()) != INT_MAX){
			#ifdef MYDEBUG
				qDebug()<<newAdjacent;
			#endif
			undirected->addEdge(newVertex, newAdjacent);
			undirected->addEdge(newAdjacent, newVertex);
		}
		#ifdef MYDEBUG
			qDebug() << ":" << vertex;
		#endif
	}
	undirected->clearAfterAlgorithm();
	#ifdef MYDEBUG
		qDebug() <<"MEngine::toUndirected()";
	#endif
	graph->setUndirected(undirected);
	QString empty;
	return empty;
}

QString MEngine::stronglyConnected(){
	graph = currentGraph.value(); 
	QLinkedList<int> &list = graph->getVertexList();
	int start = list.first();
	DFS(start);
	graph->clearAfterAlgorithm();
	transposition();
	graph->clearAfterAlgorithm();
	MGraph *transposed = graph->getTransposed();
	QMap <int, int> &order = graph->getDFSReference();
	DFSOrder(order, transposed);
	QString empty;
	return empty;
}

void MEngine::DFSinternalOrder(QMap<int, int>& ll, int ii){
	#ifdef MYDEBUG
		qDebug() <<"DSFinternalORDER oznacza: "<<ii;
		qDebug() <<"nextUnmarkedAdjacentD(): ";
	#endif
	int adjacent;
	graph->DFSMarkStart(ii);
	while((adjacent = graph->nextUnmarkedAdjacentD()) != INT_MAX){
		qDebug() << "AAAAAAAdjacent: " << adjacent;
	}
}

QString MEngine::DFSOrder(QMap<int, int>&l, MGraph *g){
	QMap<int, int> reverse;
	QMap<int, int>::const_iterator i = l.constBegin();
	while(i != l.constEnd()){
		//#ifdef MYDEBUG
		//	qDebug() <<"DSFORDER: key = "<<i.key()<<"; value = "<<i.value();
		//#endif
		reverse.insert(i.value(), i.key());
		++i;
	}
	QMap<int, int>::const_iterator j = reverse.constEnd();
	do{
		--j;
		//#ifdef MYDEBUG
		//	qDebug() <<"RIGHT DSFORDER: key = "<<j.key()<<"; value = "<<j.value();
		//#endif
	}while(j != reverse.constBegin());
	//
	MGraph *buffer = graph;
	#ifdef MYDEBUG
		qDebug() << "Startuje MEngine::DFSOrder";
	#endif
	//QString message; //Wiadomoœæ zwracana z funkcji BFS
	graph = g; 
	graph->clearDFSResult();//Czyœci poprzedni rezultat
	QMap<int, int>::iterator tmp = reverse.end();
	--tmp;
	int start = tmp.value();
	if(!(graph->contains(start))){ //Kontrola czy podany wierzcho³ek znajduje sie w grafie
		QString message = tr("DFS:\nWierzcholek %1 nie znajduje sie w grafie!").arg(start);
		return message;
	}
	j = reverse.constEnd();
	//DFSinternalOrder(l, start);
	graph->setCurrentVertex(start);
	do{
		--j;
		//#ifdef MYDEBUG
		//	qDebug() <<"RIGHT DSFORDER: key = "<<j.key()<<"; value = "<<j.value();
		//#endif
		if(!graph->isDFSMarked(j.value())){
			DFSinternalOrder(l, j.value());
			//#ifdef MYDEBUG
			//	qDebug() <<"Vertex: " << j.value() <<" is unmarked";
			//#endif
		}
	}while(j != reverse.constBegin());
	//Poni¿ej dla grafów niespójnych
	/*QLinkedList<int> &list = graph->getVertexList();
	int vertex;
	foreach (vertex, list){
		#ifdef MYDEBUG
			qDebug() << vertex;
		#endif
		if(!graph->isDFSMarked(vertex)){
			DFSinternal(vertex);
			#ifdef MYDEBUG
				qDebug() <<"Vertex: " << vertex <<" is unmarked";
			#endif
		}
	}*/
	//
	#ifdef MYDEBUG
		qDebug() << "Konczy MEngine::DFSOrder";
	#endif

	graph->clearAfterAlgorithm();
	//
	graph = buffer;
	QString empty;
	return empty;
}

QString MEngine::graphColoring(){/*Nie koloruje niespójnych wierzcho³ków*/
	//Zobojêtniæ graf i wykonywaæ operacje na grafie niezorientowanym
	graph = currentGraph.value();
	MGraph *buffer = graph;
	toUndirected();
	MGraph *undirected = graph->getUndirected();
	graph = undirected;
	//
	int adjacent;
	int tmpColor;
	
	QLinkedList<int> kolory;
	QLinkedList<int> &list = graph->getVertexList();
	QLinkedList<int>::iterator i = list.begin();
	int start = list.first();
	#ifdef MYDEBUG
		qDebug() << "Koloraowanie zaczyna sie od: " << start;
	#endif
	graph->setVertexColor(start, 1);
	while(i != list.end()){
		graph->setCurrentVertex(*i);
		if((tmpColor = graph->getColorVertex(*i)) != INT_MAX){ //Tylko dla pokolorowanych wierzcholków
			kolory.clear();
			kolory.append(tmpColor);
			while((adjacent = graph->nextAdjacent()) != INT_MAX){ //Dopóki ma s¹siadów
				if((tmpColor = graph->getColorVertex(adjacent)) != INT_MAX){ //Dopóki ma pokolorowanych s¹siadów
					kolory.append(tmpColor); //Dodaje kolor do listy zabronionych kolorów
				}
			}
			#ifdef MYDEBUG
				QLinkedList<int>::iterator k;
				k = kolory.begin();
				while(k != kolory.end()){
					qDebug() << "LISTA:" << *k;
					++k;
				}
			#endif
			graph->setCurrentVertex(*i);
			while((adjacent = graph->nextAdjacent()) != INT_MAX){ //Dopóki ma s¹siadów
				if((tmpColor = graph->getColorVertex(adjacent)) == INT_MAX){ //Dopóki ma NIEpokolorowanych s¹siadów
					int l=1; //Koloruje pierwszym wolnym kolorem 
					while(1){
						if(!kolory.contains(l))
							break;
						++l;
					}
					graph->setVertexColor(adjacent, l);//Koloruje wierzcholek
					kolory.append(l);//Dodaje ten kolor do listy zabronionych kolorów
				}
			}
		}
		++i;
	}
	QMap<int,int> &colors = graph->getColorList(); 
	graph->clearAfterAlgorithm();
	graph = buffer;
	graph->setColorList(colors);
	//graph->clearAfterAlgorithm();
	QString empty;
	return empty;
}

QMap<int,int> &MEngine::getVColorList(){
	graph = currentGraph.value();
	return graph->getColorList();
}

QMap<int,int> & MEngine::returnBFSL(){
	graph = currentGraph.value();
	return graph->returnBFS();
}

QMap<int,int> & MEngine::returnDFSSL(){
	graph = currentGraph.value();
	return graph->returnDFSS();
}

QMap<int,int> & MEngine::returnDFSEL(){
	graph = currentGraph.value();
	return graph->returnDFSE();
}

QMap<int,int> & MEngine::returnBFSTreeE(){
	graph = currentGraph.value();
	return graph->returnBFSTree();
}

QMap<int,int> & MEngine::returnDFSTreeL(){
	graph = currentGraph.value();
	return graph->returnDFSTree();
}
