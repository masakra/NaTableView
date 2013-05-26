
#ifndef GROUPPOINTER_H
#define GROUPPOINTER_H

#include <QVector>
#include "GroupKey.h"

class GroupPointer : public QVector< GroupKey >
{
	public:
		GroupPointer();

		GroupPointer( const QVector< GroupKey > & other );

		GroupPointer shifted( int pos = 1 ) const;

		bool isSingle() const;

		const GroupKey & head() const;
};

#endif
