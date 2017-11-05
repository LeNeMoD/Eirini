/*
 * futurePosition.cpp
 *
 *  Created on: Nov 5, 2017
 *      Author: domenico
 */

#include "encoding/block-helpers.hpp"
#include "encoding/encoding-buffer.hpp"
#include "futurePositionInfo.hpp"
//#include "../../../core/model/nstime.h"

namespace ndn {

FuturePositionInfo::FuturePositionInfo() {
//	m_futureLocation = 0;
	m_location_X_Coord = 0;
	m_location_Y_Coord = 0;
	m_location_Z_Coord_Velocity = 0;
	m_time = 0;
	m_bool_position_is_empty = true;

}

FuturePositionInfo::FuturePositionInfo(ns3::Vector positionVector, double timeAtFutureLocation) {
	m_location_X_Coord = positionVector.x;
	m_location_Y_Coord = positionVector.y;
	m_location_Z_Coord_Velocity = positionVector.z;
	m_time = timeAtFutureLocation;
	m_bool_position_is_empty = false;

}

template<encoding::Tag TAG>
size_t FuturePositionInfo::wireEncode(EncodingImpl<TAG>& encoder) const {
	size_t totalLength = 0;

	// if position are not emptiy encode them
	if (!m_bool_position_is_empty) {
		ns3::Vector futurePositionVector;
		futurePositionVector.x = m_location_X_Coord;
		futurePositionVector.y = m_location_Y_Coord;
		futurePositionVector.z = m_location_Z_Coord_Velocity;

		totalLength += prependNonNegativeIntegerBlock(encoder,
				tlv::FuturePosition, futurePositionVector);
	}

	// time
	if (!m_bool_position_is_empty) {

		totalLength += prependNonNegativeIntegerBlock(encoder,
				tlv::TimeAtFuturePosition, m_time);
	}

	totalLength += encoder.prependVarNumber(totalLength);
	totalLength += encoder.prependVarNumber(tlv::FuturePositionInfo);
	return totalLength;

}

const Block&
FuturePositionInfo::wireEncode() const {
	{
		if (m_wire.hasWire())
			return m_wire;

		EncodingEstimator estimator;
		size_t estimatedSize = wireEncode(estimator);

		EncodingBuffer buffer(estimatedSize, 0);
		wireEncode(buffer);

		m_wire = buffer.block();
		return m_wire;
	}

}

void FuturePositionInfo::wireDecode(const Block& wire) {

	Block::element_const_iterator val = m_wire.elements_begin();

	//Dome
	//FuturePosistion
	if (val != m_wire.elements_end() && val->type() == tlv::FuturePosition) {
		ns3::Vector futurePosition = *val;
		m_location_X_Coord = futurePosition.x;
		m_location_Y_Coord = futurePosition.y;
		m_location_Z_Coord_Velocity = futurePosition.z;
		++val;
	}

	//TimeAtFuturePosition
	if (val != m_wire.elements_end()
			&& val->type() == tlv::TimeAtFuturePosition) {
		m_time = readNonNegativeInteger(*val);
		++val;
	}
}

double FuturePositionInfo::getTime() {
	return m_time;
}

ns3::Vector FuturePositionInfo::getPositionVector() {
	ns3::Vector futurePosition;
	m_location_X_Coord = futurePosition.x;
	m_location_Y_Coord = futurePosition.y;
	m_location_Z_Coord_Velocity = futurePosition.z;
	return futurePosition;
}

double
FuturePositionInfo::getLocation_X() {
	return m_location_X_Coord ;

}

double
FuturePositionInfo::getLocation_Y() {
	return m_location_Y_Coord ;
}

double
FuturePositionInfo::getLocation_Z() {
	return m_location_Z_Coord_Velocity ;

}

}

