
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

void
GroupPointer::append( const GroupKey & key )
{
	if ( key.isValid() )
		QVector< GroupKey >::append( key );
}

GroupPointer &
GroupPointer::operator<<( const GroupKey & key )
{
	append( key );

	return *this;
}
