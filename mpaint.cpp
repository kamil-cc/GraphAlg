#include "mpaint.hpp"

MPaint::MPaint(QWidget *parent = 0, int *options = NULL):QDialog(parent){ //Narazie dziedziczy po QDialog ale powinno po QWidget
	#ifdef MYDEBUG
		qDebug()<<"Startuje konstruktor MPaint::MPaint(QWidget)";
	#endif
	//
	scene = new QGraphicsScene(this);  
	//
	opt = options;
	switch(opt[0]){
		case 1:
			GA = new GraphAttributes(G,	GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics);  //Tree layout
			break;
		case 2:
			GA = new GraphAttributes(G, //orthogonal layout
			GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
			GraphAttributes::nodeLabel | GraphAttributes::nodeColor | 
			GraphAttributes::edgeColor | GraphAttributes::edgeStyle | 
			GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate);
			break;
		case 3:
			GA = new GraphAttributes(G, //(Fast) Herarchy Layout
			GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics |
			GraphAttributes::nodeLabel | GraphAttributes::nodeColor | 
			GraphAttributes::edgeColor | GraphAttributes::edgeStyle | 
			GraphAttributes::nodeStyle | GraphAttributes::nodeTemplate);
			break;
		case 4:
		case 5:
			GA = new GraphAttributes(G); //EnergyBasedLayout oraz Planar
		break;
	}
	//
	nodeCounter = 0;
	len = 14;
	#ifdef MYDEBUG
		qDebug()<<"Konczy sie konstruktor MPaint::MPaint(QWidget)";
	#endif
}

void MPaint::addVertex(int v){
	if(opt[0] == 5){
		++nodeCounter;
		if(nodeCounter%2){
			#ifdef MYDEBUG
				qDebug()<<"LEWY";
			#endif
			left = G.newNode();
			GA->x(left) = -1*(nodeCounter+1);
			GA->y(left) = -1*nodeCounter;		
			GA->width(left) = 1*(nodeCounter+1);
			GA->height(left) = 1;
			cache.insert(v, left);
		}else{
			#ifdef MYDEBUG
				qDebug()<<"GORNY";
			#endif
			bottom = G.newNode();
			GA->x(bottom) = 1*(len-nodeCounter);		
			GA->y(bottom) = 1*(len+1-nodeCounter);
			GA->width(bottom) = 1;
			GA->height(bottom) = 1*(len+1-nodeCounter);
			cache.insert(v, bottom);
		}
	}else{
		++nodeCounter;
		if(nodeCounter%2){
			#ifdef MYDEBUG
				qDebug()<<"LEWY";
			#endif
			left = G.newNode();
			GA->x(left) = -5*(nodeCounter+1);
			GA->y(left) = -20*nodeCounter;		
			GA->width(left) = 10*(nodeCounter+1);
			GA->height(left) = 15;
			cache.insert(v, left);
		}else{
			#ifdef MYDEBUG
				qDebug()<<"GORNY";
			#endif
			bottom = G.newNode();
			GA->x(bottom) = 20*(len-nodeCounter);		
			GA->y(bottom) = 5*(len+1-nodeCounter);
			GA->width(bottom) = 15;
			GA->height(bottom) = 10*(len+1-nodeCounter);
			cache.insert(v, bottom);
		}
	}
}

void MPaint::addEdge(int from, int to){
	#ifdef MYDEBUG
		qDebug()<<"MPaint::addEdge(from:"<<from<<", to"<<to<<")";
	#endif
	node nodeFrom = cache.find(from).value();
	node nodeTo = cache.find(to).value();
	#ifdef MYDEBUG
		//qDebug()<<"from="<<(int)nodeFrom<<", to="<<(int)nodeTo<<")";
	#endif
	edge e = G.newEdge(nodeFrom, nodeTo);
	DPolyline &p = GA->bends(e);
	++nodeCounter;
	p.pushBack(DPoint(10,-20*nodeCounter));
	p.pushBack(DPoint(20*(len-nodeCounter),-10));
}

void MPaint::drawIt(){
	#ifdef MYDEBUG
		qDebug()<<"MPaint::drawIt()";
	#endif
	int nodeNumber;
	double nodeWidth = 30;
	double nodeHeight = 30;
	int siblingDistance = nodeWidth + nodeHeight;
	switch(opt[0]){ //Wybór layoutu
		case 1:
			{
			TreeLayout treeLayout;
			treeLayout.siblingDistance(siblingDistance);
			
			treeLayout.call(*GA);
			}
			break;
		case 2:
			{
			PlanarizationLayout pl;

			FastPlanarSubgraph *ps = new FastPlanarSubgraph;
			ps->runs(100);
			VariableEmbeddingInserter *ves = new VariableEmbeddingInserter;
			ves->removeReinsert(EdgeInsertionModule::rrAll);
			pl.setSubgraph(ps);
			pl.setInserter(ves);

			//EmbedderMinDepthMaxFaceLayers *emb = new EmbedderMinDepthMaxFaceLayers;
			//pl.setEmbedder(emb);

			OrthoLayout *ol = new OrthoLayout;
			ol->separation(20.0);
			ol->cOverhang(0.4);
			ol->setOptions(2+4);
			pl.setPlanarLayouter(ol);

			pl.call(*GA);
			}
			break;
		case 3:
			{
			SugiyamaLayout SL; //(Fast) Herarchy layout
			SL.setRanking(new OptimalRanking);
			SL.setCrossMin(new MedianHeuristic);

			FastHierarchyLayout *fhl = new FastHierarchyLayout();
			fhl->layerDistance(30.0);
			fhl->nodeDistance(25.0);
			//fhl->weightBalancing(0.8);
			SL.setLayout(fhl);
 
			SL.call(*GA);
			}
			break;
		case 4:
			{
			FMMMLayout fmmm;//Energy Layout
			fmmm.useHighLevelOptions(true);
			fmmm.unitEdgeLength(15.0); 
			fmmm.newInitialPlacement(true);
			fmmm.qualityVersusSpeed(FMMMLayout::qvsGorgeousAndEfficient);
 
			fmmm.call(*GA);
			}
			break;
		case 5:
			{
			PlanarStraightLayout pls;
			pls.baseRatio(10.0);
				
			pls.call(*GA);
			}
			break;
	}
	//
	double width = GA->boundingBox().width(); 
	double height = GA->boundingBox().height();
	if(opt[0] == 1)
		scene->setSceneRect(QRect(-200, -450, width+nodeWidth+100, height+nodeHeight)); //Tree
	else
		scene->setSceneRect(QRect(0, 0, width+nodeWidth+100, height+nodeHeight+100)); //Else layout
	qDebug() << "Wymiary sceny: " << GA->boundingBox().width() << " x " << GA->boundingBox().height() << endl;
	GA->setAllWidth(nodeWidth);
	GA->setAllHeight(nodeHeight);
	#ifdef MYDEBUG
		qDebug()<<"forall_edges";
	#endif
	edge e;
	forall_edges(e,G){
		node source = e->source();
		node target = e->target();
		double x1 = GA->x(source);
		double y1 = GA->y(source);
		double x2 = GA->x(target);
		double y2 = GA->y(target);
		//
		QPainterPath p; //Jeœli zawiera krawêŸ w drug¹ stronê, to nie maluj!!!!
		p.moveTo(x1 + nodeWidth/2.0, y1 + nodeHeight/2.0);
		p.lineTo(x2 + nodeWidth/2.0, y2 + nodeHeight/2.0);
		//
		scene->addPath(p, QPen (QBrush(Qt::black),2));
		QPainterPath arrowhead;
		//Rysowanie strza³ki
		double Pi = 3.1415926536;
		double arrowHeadLength = 10;// dlugosc strzalki
		double lineAngle = std::atan((double)(y2-y1)/(double)(x2-x1));
		//Zmiana x2 i y2
		double radius =  nodeWidth/2.0;
		//
		double endAngle1 = lineAngle + 35.0 * Pi/180.0;
		double endAngle2 = lineAngle - 35.0 * Pi/180.0;
		int x3, x4, y3, y4;
		if(x2 >= x1){
			x2 = x2 - radius * std::cos(lineAngle);
			y2 = y2 - radius * std::sin(lineAngle);
			y3 = y2 - arrowHeadLength * std::sin(endAngle1);
			y4 = y2 - arrowHeadLength * std::sin(endAngle2);
			x3 = x2 - arrowHeadLength * std::cos(endAngle1);
			x4 = x2 - arrowHeadLength * std::cos(endAngle2);
		}else{
			x2 = x2 + radius * std::cos(lineAngle);
			y2 = y2 + radius * std::sin(lineAngle);
			y3 = y2 + arrowHeadLength * std::sin(endAngle1);
			y4 = y2 + arrowHeadLength * std::sin(endAngle2);
			x3 = x2 + arrowHeadLength * std::cos(endAngle1);
			x4 = x2 + arrowHeadLength * std::cos(endAngle2);
		}
		//
		arrowhead.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0); //Dobrze
		arrowhead.lineTo(x3 + (double)nodeWidth/2.0, y3 + (double)nodeHeight/2.0);
		arrowhead.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0);
		arrowhead.lineTo(x4 + (double)nodeWidth/2.0, y4 + (double)nodeHeight/2.0);
		//arrowhead.moveTo(x3 + nodeWidth/2, y3 + nodeHeight/2);
		//arrowhead.lineTo(x4 + nodeWidth/2, y4 + nodeHeight/2);
		scene->addPath(arrowhead, QPen(QBrush(Qt::black),2));
		//
		//Rysowanie wyników DFS i BFS
		QPainterPath DFS;
		QPainterPath BFS;
		int s = cache.key(source);
		int t = cache.key(target);
		//qDebug()<<"S: "<<" T:"<<t;
		QMap<int, int>::iterator i = BFSTree.begin();
		while(i != BFSTree.end()){
			if(i.key() == s){
				if(i.value() == t){
					BFS.moveTo(x1 + nodeWidth/2.0, y1 + nodeHeight/2.0);
					BFS.lineTo(x2 + nodeWidth/2.0, y2 + nodeHeight/2.0);
					BFS.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0); //Dobrze
					BFS.lineTo(x3 + (double)nodeWidth/2.0, y3 + (double)nodeHeight/2.0);
					BFS.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0);
					BFS.lineTo(x4 + (double)nodeWidth/2.0, y4 + (double)nodeHeight/2.0);
					scene->addPath(BFS, QPen(QBrush(Qt::red),2));
				}
			}
			++i;
		}
		//
		QMap<int, int>::iterator j = DFSTree.begin();
		while(j != DFSTree.end()){
			if(j.key() == s){
				if(j.value() == t){
					DFS.moveTo(x1 + nodeWidth/2.0, y1 + nodeHeight/2.0);
					DFS.lineTo(x2 + nodeWidth/2.0, y2 + nodeHeight/2.0);
					DFS.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0); //Dobrze
					DFS.lineTo(x3 + (double)nodeWidth/2.0, y3 + (double)nodeHeight/2.0);
					DFS.moveTo(x2 + (double)nodeWidth/2.0, y2 + (double)nodeHeight/2.0);
					DFS.lineTo(x4 + (double)nodeWidth/2.0, y4 + (double)nodeHeight/2.0);
					scene->addPath(DFS, QPen(QBrush(Qt::green),2));
				}
			}
			//qDebug()<<"kkkkkk: "<< j.key()<<" vvvvvvvv:"<<j.value();
			++j;
		}
		//
		/*QPolygonF triangle;
		triangle.append(QPointF(x3 + nodeWidth/2, y3 + nodeHeight/2));
		triangle.append(QPointF(x4 + nodeWidth/2, y4 + nodeHeight/2));
		triangle.append(QPointF(x2 + nodeWidth/2, y2 + nodeHeight/2));
		QGraphicsPolygonItem* pTriangleItem = scene->addPolygon(triangle);*/
	//
	}
	#ifdef MYDEBUG
		qDebug()<<"forall_nodes";
	#endif
	node n;
	int color =0;
	forall_nodes(n, G) {
		double x = GA->x(n);
		double y = GA->y(n);
		double w = GA->width(n);
		double h = GA->height(n);
		QRectF boundingRect(x, y, w, h);
		#ifdef MYDEBUG
			qDebug() << x << " : " << y << " : " << endl;
		#endif
		//
		nodeNumber = cache.key(n);
		color = graphColored.value(nodeNumber);
		//qDebug() << "COLOR: "<< color;
		//
		QRadialGradient radialGradient(boundingRect.center(), boundingRect.width());
		radialGradient.setColorAt(1.0, Qt::lightGray);
		if(graphColored.isEmpty()){
			radialGradient.setColorAt(0.7, Qt::yellow);
		}else{
			switch(color){
				case 1:
					radialGradient.setColorAt(0.7, Qt::red);
				break;
				case 2:
					radialGradient.setColorAt(0.7, Qt::green);
				break;
				case 3:
					radialGradient.setColorAt(0.7, Qt::blue);
				break;
				case 4:
					radialGradient.setColorAt(0.7, Qt::cyan);
				break;
				case 5:
					radialGradient.setColorAt(0.7, Qt::magenta);
				break;
				case 6:
					radialGradient.setColorAt(0.7, Qt::yellow);
				break;
				case 7:
					radialGradient.setColorAt(0.7, Qt::darkRed);
				break;
				case 8:
					radialGradient.setColorAt(0.7, Qt::darkGreen);
				break;
				case 9:
					radialGradient.setColorAt(0.7, Qt::darkBlue);
				break;
				case 10:
					radialGradient.setColorAt(0.7, Qt::darkCyan);
				break;
				case 11:
					radialGradient.setColorAt(0.7, Qt::darkMagenta);
				break;
				case 12:
					radialGradient.setColorAt(0.7, Qt::darkYellow);
				break;
				case 13:
					radialGradient.setColorAt(0.7, Qt::white);
				break;
				case 14:
					radialGradient.setColorAt(0.7, Qt::darkGray);
				break;
				case 15:
					radialGradient.setColorAt(0.7, Qt::gray);
				break;
				case 16:
					radialGradient.setColorAt(0.7, Qt::lightGray);
				break;
			}
		}
		radialGradient.setColorAt(0.0, Qt::white);
		scene->addEllipse(boundingRect, QPen(Qt::black), QBrush(QRadialGradient(radialGradient)));
		//
		#ifdef MYDEBUG
			qDebug() <<  "nodeNumber = " << nodeNumber;
		#endif
		QString nodeNum = QString::number(nodeNumber);
		QGraphicsTextItem *text = scene->addText(nodeNum);
		if(nodeNumber<10) //Wyœrodkowanie tekstu
			text->setPos(x+9, y+5); 
		else
			text->setPos(x+5, y+5); 
			
		//Czy podpisaæ bfsem
		if(!BFSData.isEmpty()){
			int bfsNumber = BFSData.value(nodeNumber);
			QString bfsNum = tr("BFS: ") + QString::number(bfsNumber);
			QGraphicsTextItem *bfsText = scene->addText(bfsNum);
			bfsText->setPos(x+15, y-15);
		}
		if(!DFSDataStart.isEmpty()){
			int dfsNumberS = DFSDataStart.value(nodeNumber);
			int dfsNumberE = DFSDataEnd.value(nodeNumber);
			QString dfsNum = tr("DFS: ") + QString::number(dfsNumberS) + "/" + QString::number(dfsNumberE);
			QGraphicsTextItem *dfsText = scene->addText(dfsNum);
			dfsText->setPos(x+15, y+25);
		}
	}
	G.clear(); //Czyszczenie grafu po wyœwietleniu
	//
	/*QSize areaSize = viewport()->size();
    QSize  widgetSize = widget->size();

    verticalScrollBar()->setPageStep(areaSize.height());
    horizontalScrollBar()->setPageStep(areaSize.width());
    verticalScrollBar()->setRange(0, widgetSize.height() - areaSize.height());
    horizontalScrollBar()->setRange(0, widgetSize.width() - areaSize.width());
    updateWidgetPosition();*/
	//
	view = new QGraphicsView(scene,this);  
	layout = new QVBoxLayout(this); 
	layout->addWidget(view);
	setLayout(layout); 
	//resize(270,200); 
}

void MPaint::assignColors(QMap<int,int> &l){
	#ifdef MYDEBUG
		qDebug()<<"ASSIGN-COLORS";
	#endif
	graphColored = l;
}

void MPaint::assignBFS(QMap<int,int> &l){
	#ifdef MYDEBUG
		qDebug()<<"ASSIGN-BFS";
	#endif
	BFSData = l;
}

void MPaint::assignDFS(QMap<int,int> &s, QMap<int,int> &e){
	#ifdef MYDEBUG
		qDebug()<<"ASSIGN-DFS";
	#endif
	DFSDataStart = s;
	DFSDataEnd = e;
}

void MPaint::assignBFSTree(QMap<int,int> &l){
	BFSTree = l;
}

void MPaint::assignDFSTree(QMap<int,int> &s){
	DFSTree = s;
}
