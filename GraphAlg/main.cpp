#include <QtGui>
#include "mwindow.hpp"

int main(int argv, char **args){

	QApplication app(argv, args);

	MWindow window; //Okno
	window.show(); //Pokazuje okno

	return app.exec(); //Wykonanie aplikacji
}
