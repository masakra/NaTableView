
#include <QApplication>

#include <QtSql>
#include <QSortFilterProxyModel>

#include "NaTableView.h"

int
main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( "test.db" );
	//QSqlDatabase db = QSqlDatabase::addDatabase( "QPSQL" );
	//db.setHostName("localhost");
	//db.setDatabaseName("nordavia");

	db.open();

	NaTableView ntv;
	ntv.resize( 1024, 550 );
	QSqlQueryModel model;

	QSortFilterProxyModel proxy;
	proxy.setSourceModel( &model );
	proxy.sort( 0 );
	proxy.setDynamicSortFilter( true );

	model.setQuery( "SELECT * FROM test" );
	//model.setQuery( "SELECT * FROM arinc.pa" );

	ntv.setModel( &proxy );
	ntv.show();


	return app.exec();
}

