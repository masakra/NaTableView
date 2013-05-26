
#include "GroupPointer.h"

GroupPointer::GroupPointer()
	: QVector< GroupKey>()
{
}

GroupPointer::GroupPointer( const QVector< GroupKey > & other )
	: QVector< GroupKey >( other )
{
}

GroupPointer
GroupPointer::shifted( int pos ) const
{
	return mid( pos );
}

bool
GroupPointer::isSingle() const
{
	return size() == 1;
}

const GroupKey &
GroupPointer::head() const
{
	return first();
}

