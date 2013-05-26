
#include "GroupKey.h"

GroupKey::GroupKey()
	: QVariant()
{
}

GroupKey::GroupKey( const QVariant & var )
	: QVariant( var )
{
}

bool
GroupKey::operator<( const GroupKey & other ) const
{
	// TODO add numeric and datetime types

	return toString() < other.toString();
}
