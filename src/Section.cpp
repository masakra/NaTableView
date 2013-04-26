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

#include "Section.h"

#include <QtGui>

Section::Section()
	: size( DEFAULT_SECTION_SIZE ), logical( -1 )
{
}

Section::Section( int i )
	: size( DEFAULT_SECTION_SIZE ), logical( i )
{
}

void
Section::draw( QPainter & painter, int x, int y, int height, const QString & text,
		HandleMode handleMode ) const
{
	draw( painter, x, y, size, height, text, handleMode );
}

void
Section::draw( QPainter & painter, int x, int y, int width, int height, const QString & text,
		HandleMode handleMode )
{
	painter.save();

	//painter.setRenderHint( QPainter::Antialiasing );

	const QRect rect( x, y, width - 1, height - 1 );

	QPen pen( COLOR_BORDER );
	pen.setWidth( 0 );

	painter.setPen( pen );

	QLinearGradient bgGradient( 0, 1, 0, rect.bottom() - 1 );
	bgGradient.setColorAt( 0, COLOR_GRAD_BEGIN );
	//bgGradient.setColorAt( 1, QColor( 29, 82, 165 ) );
	bgGradient.setColorAt( 1, COLOR_GRAD_END );

	//QBrush bg( bgGradient );

	painter.fillRect( rect, QBrush( bgGradient ) );

	painter.drawRoundedRect( rect, 3., 5. );

	if ( handleMode == WithHandle ) {

		const qreal vStep = rect.height() / 6.0;

		qreal vPos = rect.top() + vStep * 2.0,
			  hPos = rect.right() - HANDLE_WIDTH;

		painter.setPen( QPen( COLOR_GRAD_BEGIN ) );

		for ( int i = 0; i < 3; ++i ) {
			painter.drawPoint( QPointF( hPos, vPos ) );
			vPos += qRound( vStep );
		}

		vPos = rect.top() + vStep * 2.0 + 1;
		hPos = rect.right() - HANDLE_WIDTH + 1;

		painter.setPen( QPen( COLOR_GRAD_END ) );

		for ( int i = 0; i < 3; ++i ) {
			painter.drawPoint( QPointF( hPos, vPos ) );
			vPos += qRound( vStep );
		}
	}

	/*
	\\ text
	*/

	painter.setPen( QPen( Qt::white ) );

	painter.drawText( rect, Qt::AlignCenter | Qt::TextSingleLine,
			text );

	painter.restore();
}
