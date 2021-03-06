/***************************************************************************
 *   Copyright (C) 2013 by Sergey N Chursanov                              *
 *                                                                         *
 *   email: masakra@mail.ru                                                *
 *   jabber: masakra@jabber.ru                                             *
 *                                                                         *
 *   Permission is hereby granted, free of charge, to any person obtaining *
 *   a copy of this software and associated documentation files (the       *
 *   "Software"), to deal in the Software without restriction, including   *
 *   without limitation the rights to use, copy, modify, merge, publish,   *
 *   distribute, sublicense, and/or sell copies of the Software, and to    *
 *   permit persons to whom the Software is furnished to do so, subject to *
 *   the following conditions:                                             *
 *                                                                         *
 *   The above copyright notice and this permission notice shall be        *
 *   included in all copies or substantial portions of the Software.       *
 *                                                                         *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       *
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    *
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. *
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR     *
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, *
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR *
 *   OTHER DEALINGS IN THE SOFTWARE.                                       *
 ***************************************************************************/

#include "Sections.h"

Sections::Sections()
	: QVector< Section >()
{
}

int
Sections::pos( int sec ) const
{
	if ( sec >= size() )
		return -1;

	int p = 0;

	for ( register int i = 0; i < sec; ++i )
		p += at( i ).size;

	return p;
}

int
Sections::visualIndex( int logical ) const
{
	for ( int i = 0; i < size(); ++i )
		if ( at( i ).logical == logical )
			return i;

	return -1;
}

QVector< int >
Sections::logicals() const
{
	QVector< int > vector;

	for ( int i = 0; i < size(); ++i )
		vector << at( i ).logical;

	return vector;
}

int
Sections::length() const
{
	int l = 0;

	for ( register int i = 0; i < size(); ++i )
		l += at( i ).size;

	return l;
}

