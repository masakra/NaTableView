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

 /** \class Section
  *
  * \brief 
  */

#ifndef SECTION_H
#define SECTION_H

#define DEFAULT_SECTION_SIZE 128
#define MIN_SECTION_SIZE 20

#define COLOR_BORDER QColor( 115, 147, 195 )
#define COLOR_GRAD_BEGIN QColor( 72, 113, 174 )
#define COLOR_GRAD_END QColor( 5, 5, 27 )

#define HANDLE_WIDTH 4

//#include
class QPainter;
class QString;

class Section
{
	public:
		Section();
		Section( int i );

		int size;
		int logical;

		enum HandleMode {
			WithHandle,
			WithoutHandle
		};

		void draw( QPainter & painter, int x, int y, int height, const QString & text,
				HandleMode handleMode = WithHandle ) const;

		static void draw( QPainter & painter, int x, int y, int width, int height,
				const QString & text, HandleMode handleMode );
};

#endif


