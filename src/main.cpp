
#include <QApplication>

#include <QtSql>

#include "NaTableView.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( "test.db" );

	db.open();


	NaTableView ntv;
	ntv.resize( 1024, 550 );
	QSqlQueryModel model;

	model.setQuery( "SELECT * FROM test" );

	ntv.setModel( &model );
	ntv.show();


	return app.exec();
}

