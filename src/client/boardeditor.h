/*
   DrawPile - a collaborative drawing program.

   Copyright (C) 2006 Calle Laakkonen

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifndef BOARDEDITOR_H
#define BOARDEDITOR_H

#include <QColor>

namespace drawingboard {

class Board;
class User;
class Brush;

//! A delegate for accessing the board contents
/**
 * The board editor class provides an interface for local tools to
 * access the board directly and over the network.
 *
 * Read only methods always access the board directly, whereas those that
 * modify the board will either access directly or construct and dispatch
 * network packets.
 */
class BoardEditor {
	public:

		BoardEditor(Board *board, User *user) : board_(board), user_(user) {}
		virtual ~BoardEditor() {}

		//! Get the brush currently in use
		const Brush& currentBrush();

		//! Get color from the board at the specified coordinates
		QColor colorAt(int x,int y);

		//! Set the tool used for drawing
		virtual void setTool(const Brush& brush) = 0;

		//! Add a new point to a stroke.
		virtual void addStroke(int x, int y, qreal pressure) = 0;

		//! End current stroke. Next addStroke will begin a new one.
		virtual void endStroke() = 0;

	protected:
		Board *board_;
		User *user_;
};

//! Board editor that modifies the local board
class LocalBoardEditor : public BoardEditor {
	public:
		LocalBoardEditor(Board *board, User *user) : BoardEditor(board,user) {}

		void setTool(const Brush& brush);
		void addStroke(int x, int y, qreal pressure);
		void endStroke();
};

}

#endif
