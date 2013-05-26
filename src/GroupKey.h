
#ifndef GROUPKEY_H
#define GROUPKEY_H

#include <QVariant>

class GroupKey : public QVariant
{
	public:
		GroupKey();
		GroupKey( const QVariant & var );

		bool operator<( const GroupKey & other ) const;
};

#endif

