/*
   DrawPile - a collaborative drawing program.

   Copyright (C) 2008 Calle Laakkonen

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

#ifndef DP_PROTO_STROKE_H
#define DP_PROTO_STROKE_H

#include <QVarLengthArray>

#include "protocol.h"

namespace protocol {

/**
 * A stroke coordinate.
 */
struct XYZ {
	XYZ() { }
	XYZ(int X, int Y, qreal Xf, qreal Yf, int Z) : x(X), y(Y), z(Z), xf(Xf), yf(Yf) { }
	int x, y, z;
	qreal xf, yf;
};

/**
 * A point in the stroke path.
 * May store more than one point, to allow the data to be sent
 * more efficiently and atomically.
 */
class StrokePoint : public Packet {
	public:
		/// The coordinate bias
		static const int BIAS = 128;

		/**
		 * Construct a stroke point message. More points can bee added
		 * with addPoint()
		 * @param user user ID. A client must set this to their ID when sending a message.
		 * @param x stroke X coordinate
		 * @param y stroke Y coordinate
		 * @param xfrac stroke X coordinate fraction
		 * @param yfrac stroke Y coordinate fraction
		 * @param z stroke pressure
		 */
		StrokePoint(int user, unsigned int x, unsigned int y, qreal xfrac,
				qreal yfrac, unsigned int z)
			: Packet(STROKE), _user(user) {
				_points.append(XYZ(x, y, xfrac, yfrac, z));
			}

		/**
		 * Deserialize a stroke point message.
		 * Check the actual number of bytes read with the length() method.
		 * @param data input device to read
		 * @param len length of the message
		 * @return new StrokePoint
		 */
		static StrokePoint *deserialize(QIODevice& data, int len);

		/**
		 * Add a new point to the message. A single message may contain
		 * at most (2^16 - 4) / 5 = 13106 points. (Although in actuality,
		 * most programs will run out of buffer space much earlier)
		 * @param x stroke X coordinate
		 * @param y stroke Y coordinate
		 * @param xfrac stroke X coordinate fraction
		 * @param yfrac stroke Y coordinate fraction
		 * @param z stroke pressure
		 */
		void addPoint(int x, int y, qreal xfrac, qreal yfrac, int z) {
			_points.append(XYZ(x, y, xfrac, yfrac, z));
		}

		/**
		 * Get the number of points contained in this message.
		 * Most messages contain only one point.
		 * @return number of points
		 */
		int points() const { return _points.count(); }

		/**
		 * Get the length of the message payload
		 * @return payload length in bytes
		 */
		unsigned int payloadLength() const { return 1 + _points.size() * 5; }

		/**
		 * Get the user from which this message was received
		 * @return user ID
		 */
		int user() const { return _user; }

		/**
		 * Get the coordinates for a stroke point
		 */
		XYZ point(int i) const { return _points[i]; }

	protected:
		void serializeBody(QIODevice& data) const;

	private:
		int _user;
		QVarLengthArray<XYZ, 1> _points;
};

/**
 * A stroke end message. This message is sent when the pen is lifted.
 */
class StrokeEnd : public Packet {
	public:
		StrokeEnd(int user) : Packet(STROKE_END), _user(user) { }

		/**
		 * Deserialize a stroke end message.
		 */
		static StrokeEnd *deserialize(QIODevice& data, int len);

		/**
		 * Get the message payload length
		 */
		unsigned int payloadLength() const { return 1; }

		/**
		 * Get the ID of the user who lifted their pen.
		 */
		int user() const { return _user; }

	protected:
		void serializeBody(QIODevice& data) const;

	private:
		int _user;
};

}

#endif

