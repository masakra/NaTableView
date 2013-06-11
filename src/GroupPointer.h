
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

		/*! \fn void append( const GroupKey & key )
		 *
		 * \brief Append a \key
		 *
		 * Unlike from QVector::append appends only valid value.
		 */
		void append( const GroupKey & key );

		GroupPointer & operator<<( const GroupKey & key );
};

#endif
