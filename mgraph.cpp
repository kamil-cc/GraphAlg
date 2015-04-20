#include "mgraph.hpp"

MGraph::MGraph(){
	#ifdef MYDEBUG
		qDebug() << "Startuje konstruktor MGraph::MGraph()";
	#endif
	currentVertex = NULL;
	currentAdjacent = NULL;
	undirected = NULL;
	transposed = NULL;
	DCounter = 1;
	BCounter = 1;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie konstruktor MGraph::MGraph()";
	#endif
}

void MGraph::addVertex(int number){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::addVertex("<< number<<")";
	#endif
	QVector<int> incidenceList; //Pusty wektor
	if(graph.contains(number)){//sprawdzanie czy jest, nie mo¿na wstawiaæ duplikatów wierzcho³ków
		#ifdef MYDEBUG
			qDebug() << "BLAD: Graf nie moze zawierac dwoch wierzcholkow o numerze:"<< number<<")";
		#endif
	}else{
		graph.insert(number, incidenceList);
	}
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::addVertex("<< number<<")";
	#endif
}

void MGraph::addEdge(int from, int to){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::addEdge("<<from<<","<<to<<")";
	#endif
	QVector<int> incidenceList;
	if(!graph.contains(from)){
		#ifdef MYDEBUG
			qDebug() << "BLAD: Graf nie zawiera wierzcholka:"<<from;
		#endif
		return;
	}
	if(!graph.contains(to)){
		#ifdef MYDEBUG
			qDebug() << "BLAD: Graf nie zawiera wierzcholka:"<<to;
		#endif
		return;
	}
	incidenceList = graph.take(from);
	if(incidenceList.contains(to)){
		#ifdef MYDEBUG
			qDebug() << "BLAD: Dublowanie wierzcholka w liscie sasiedztwa:"<<to;
		#endif
		graph.insert(from, incidenceList);
		return;
	}
	incidenceList.push_back(to);//Contains?
	qSort(incidenceList.begin(), incidenceList.end());//tego nie musi byæ, ale u³atwia DFS
	graph.insert(from, incidenceList);
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::addEdge("<<from<<","<<to<<")";
	#endif
}

QString MGraph::graphToString(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::graphToString()";
	#endif
	QString result;
	QString num;
	QVector<int> list;
	QVector<int>::iterator listIterator;
	QMap <int, QVector<int> >::iterator i = graph.begin(); 
	while (i != graph.end()) {
		num.setNum(i.key());
		result += num + " -> ";
		list = i.value();
		listIterator = list.begin();
		while(listIterator != list.end()){
			num.setNum(*listIterator);
			result += num;
			++listIterator;
			if(listIterator != list.end()){
				result += ", ";
			}
		}
		result += "\n";
		++i;
	}
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::graphToString()";
	#endif
	return result;
}

bool MGraph::contains(int vertex){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::contains("<< vertex <<")";
	#endif
	bool tmp = graph.contains(vertex);
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::contains("<< vertex <<")"<<" z rezultatem:"<<tmp;
	#endif
	return tmp;
}

void MGraph::reset(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::reset()";
	#endif
	currentVertex = NULL;//graph.begin();
	currentAdjacent = NULL;
	undirected = NULL;
	transposed = NULL;
	BFSData.clear();
	DFSDataStart.clear();
	DFSDataEnd.clear();
	graphColored.clear();
	BFSTree.clear();
	DFSTree.clear();
	DCounter = 1;
	BCounter = 1;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::reset()";
	#endif
}

//////////////////////////////////////////////////////////

QString MGraph::getBFSResult(){
	#ifdef MYDEBUG
		qDebug() << "Startuje: MGraph::BFSResult()";
	#endif
	QString result;
	if(!BFSData.isEmpty()){
		result = "BFS:\n";
	}
	QMap<int, int>::const_iterator i = BFSData.begin();
	while (i != BFSData.end()) {
		result += tr("WIERZCHOLEK: ") + QString::number(i.key()) + tr(", KOLEJNOSC: ") + QString::number(i.value()) + "\n";
		++i;
	}	
	#ifdef MYDEBUG
		qDebug() << "Konczy sie: MGraph::BFSResult()";
	#endif
	return result;
}

QString MGraph::getDFSResult(){
	#ifdef MYDEBUG
		qDebug() << "Startuje: MGraph::DFSResult()";
	#endif
	QString result;
	if(!DFSDataStart.isEmpty()){
		result = "DFS:\n";
	}
	QMap<int, int>::const_iterator i = DFSDataStart.begin();
	QMap<int, int>::const_iterator j = DFSDataEnd.constBegin();
	while (i != DFSDataStart.end()) {
		result += tr("WIERZCHOLEK: ") + QString::number(i.key()) + tr(", KOLEJNOSC: (") + QString::number(i.value()) + tr(" / ") + QString::number(j.value()) + ")\n";
		++i;
		++j;
	}	
	#ifdef MYDEBUG
		qDebug() << "Konczy sie: MGraph::DFSResult()";
	#endif
	return result;
}

void MGraph::BFSMark(int v){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::BFSMark("<< v <<")";
	#endif
	BFSData.insertMulti(v, BCounter); //Insert multi w celu wykrycia ew. b³êdów. Normalnie mo¿e byæ insert()
	BCounter++;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::BFSMark("<< v <<")";
	#endif
}

void MGraph::BFSMark(int e, int v){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::BFSMark("<< v <<")";
	#endif
	BFSData.insertMulti(v, BCounter); //Insert multi w celu wykrycia ew. b³êdów. Normalnie mo¿e byæ insert()
	BCounter++;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::BFSMark("<< v <<")";
	#endif
	qDebug()<< v <<"VVVVVV: "<< e <<" ADJACENT: ";
	BFSTree.insertMulti(e, v); //Dodaje krawedz do drzewa
}

void MGraph::DFSMarkStart(int v){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::DFSMarkStart("<< v <<")";
	#endif
	DFSDataStart.insertMulti(v, DCounter); //Insert multi w celu wykrycia ew. b³êdów. Normalnie mo¿e byæ insert()
	DCounter++;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::DFSMarkStart("<< v <<")";
	#endif
}

void MGraph::DFSMarkEnd(int v){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::DFSMarkStart("<< v <<")";
	#endif
	DFSDataEnd.insertMulti(v, DCounter); //Insert multi w celu wykrycia ew. b³êdów. Normalnie mo¿e byæ insert()
	DCounter++;
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::DFSMarkStart("<< v <<")";
	#endif
}

void MGraph::setCurrentVertex(int v){
	#ifdef MYDEBUG
		qDebug() << "Startuje setCurrentVertex("<< v <<")";
	#endif
	currentVertex = graph.find(v);
	currentAdjacent = currentVertex.value().begin();
	#ifdef MYDEBUG
		qDebug() << "Konczy sie setCurrentVertex("<< v <<")";
	#endif
}

int MGraph::nextUnmarkedAdjacentB(){
	#ifdef MYDEBUG
		qDebug()<<"Startuje MGraph::nextUnmarkedAdjacentB()";
	#endif
	int adjacent;
	while((adjacent=nextAdjacent()) != INT_MAX){
		if(!BFSData.contains(adjacent)){
			#ifdef MYDEBUG
				qDebug()<<"Konczy sie MGraph::nextUnmarkedAdjacentB() ADJACENT: "<<adjacent;
			#endif
			return adjacent;
		}
	}
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie MGraph::nextUnmarkedAdjacentB() ADJACENT: INT_MAX";
	#endif
	return INT_MAX;
}

int MGraph::nextUnmarkedAdjacentD(){
	#ifdef MYDEBUG
		qDebug()<<"Startuje MGraph::nextUnmarkedAdjacentD()";
	#endif
	int adjacent;
	while((adjacent=nextAdjacent()) != INT_MAX){
		if(!DFSDataStart.contains(adjacent)){
			#ifdef MYDEBUG
				qDebug()<<"Konczy sie MGraph::nextUnmarkedAdjacentD() ADJACENT: "<<adjacent;
			#endif
			return adjacent;
		}
	}
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie MGraph::nextUnmarkedAdjacentD() ADJACENT: INT_MAX";
	#endif
	return INT_MAX;
}

int MGraph::nextAdjacent(){ //Nastêpny s¹siad bie¿¹cego wierzcho³ka
	int adjacent;
	if(currentAdjacent != currentVertex.value().end()){
		adjacent = *currentAdjacent;
		++currentAdjacent;
		return adjacent;
	}
	return INT_MAX;
}

void MGraph::clearAfterAlgorithm(){
	currentVertex = NULL;
	currentAdjacent = NULL;
	DCounter = 1;
	BCounter = 1;
}

void MGraph::clearBFSResult(){
	BFSData.clear();
}

void MGraph::clearDFSResult(){
	DFSDataStart.clear();
	DFSDataEnd.clear();
}

void MGraph::setFirstValues(){
	currentVertex = graph.begin();
	currentAdjacent = currentVertex.value().begin();
}

int MGraph::nextVertex(){
	int val;
	if(currentVertex == graph.constEnd()){
		#ifdef MYDEBUG
			qDebug()<<"Iterator ustawiony na begin w funkcji MGraph::nextVertex";
		#endif
		currentVertex = NULL; //Zastanowiæ siê nad tym przypisaniem 
		return INT_MAX;
	}
	val = currentVertex.key();
	currentAdjacent = currentVertex.value().begin();
	adjEnd = currentVertex.value().end();
	++currentVertex;
	return val;
}

int MGraph::getNextEdge(){
	int adjacent;
	if(currentAdjacent != adjEnd){
		adjacent = *currentAdjacent;
		++currentAdjacent;
		return adjacent;
	}
	return INT_MAX;
}

QLinkedList<int>& MGraph::getVertexList(){
	#ifdef MYDEBUG
		qDebug() << "Startuje MGraph::getVertexList()";
	#endif
	QLinkedList<int> *list = new QLinkedList<int>();
	if(graph.isEmpty())
		qDebug() << "empty QMAP object!!!!!!!!!!!";
	QMap < int, QVector< int > >::const_iterator i = graph.constBegin();
	qDebug()<<"Iterator ustawiony na begin";
	while (i != graph.constEnd()) {
		#ifdef MYDEBUG
			qDebug() << i.key() << ": " << i.value();
		#endif
		list->append(i.key());
		++i;
	}
	#ifdef MYDEBUG
		qDebug() << "Konczy sie MGraph::getVertexList()";
	#endif
	return *list;
}

bool MGraph::isBFSMarked(int v){
	if(BFSData.contains(v))
		return true;
	return false;
}

bool MGraph::isDFSMarked(int v){
	if(DFSDataStart.contains(v))
		return true;
	return false;
}

void MGraph::setTransposed(MGraph *t){
	transposed = t;
}

void MGraph::setUndirected(MGraph *u){
	undirected = u;
}

MGraph *MGraph::getTransposed(){
	return transposed;
}

MGraph *MGraph::getUndirected(){
	return undirected;
}

QMap<int,int> &MGraph::getDFSReference(){
	return DFSDataEnd;
}

void MGraph::setVertexColor(int vertexToColored, int color){
	graphColored.insertMulti(vertexToColored, color);
}

QMap<int, int> &MGraph::getColorList(){
	return graphColored;
}

int MGraph::getColorVertex(int v){
	QMap < int, int >::iterator i = graphColored.find(v);
	if(i == graphColored.end())
		return INT_MAX;
	return i.value();
}

void MGraph::setColorList(QMap<int,int>& li){
	graphColored.clear();
	QMap<int,int>::const_iterator j = li.constBegin();
	while (j != li.constEnd()) {
		graphColored.insert(j.key(), j.value());
		++j;
		qDebug()<<"Seting color list";
	}
}

QString MGraph::getColoredResult(){
	QString result;
	if(graphColored.isEmpty()){
		return result;
	}
	QMap<int,int>::iterator j = graphColored.begin();
	while (j != graphColored.end()) {
		result += tr("Wierzcholek: ") + QString::number(j.key()) + tr(";   Kolor: ") + QString::number(j.value()) + tr("\n");
		++j;
	}
	return result;
}

QMap<int,int> & MGraph::returnBFS(){
	return BFSData;
}

QMap<int,int> & MGraph::returnDFSS(){
	return DFSDataStart;
}

QMap<int,int> & MGraph::returnDFSE(){
	return DFSDataEnd;
}

QMap<int,int> & MGraph::returnBFSTree(){
	return BFSTree;
}

void MGraph::addDFSTree(int f, int t){
	//qDebug()<<"ffff: "<<f<<"   TTTTT: "<<t;
	DFSTree.insertMulti(f,t);
}

QMap<int,int> & MGraph::returnDFSTree(){
	return DFSTree;
}