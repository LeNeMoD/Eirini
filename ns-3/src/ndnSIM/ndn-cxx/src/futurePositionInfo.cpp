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

BOOST_CONCEPT_ASSERT((boost::EqualityComparable<FuturePositionInfo>));
BOOST_CONCEPT_ASSERT((WireEncodable<FuturePositionInfo>));
BOOST_CONCEPT_ASSERT((WireEncodableWithEncodingBuffer<FuturePositionInfo>));
BOOST_CONCEPT_ASSERT((WireDecodable<FuturePositionInfo>));

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

const std::list<Block>&
FuturePositionInfo::getAppFuturePositionInfo() const
{
  return m_appFuturePositionInfo;
}

FuturePositionInfo&
FuturePositionInfo::setAppFuturePositionInfo(const std::list<Block>& info)
{
  for (std::list<Block>::const_iterator i = info.begin(); i != info.end(); ++i) {
    if (!(128 <= i->type() && i->type() <= 252))
      BOOST_THROW_EXCEPTION(Error("AppFuturePositionInfo block has type outside the application range "
                                  "[128, 252]"));
  }

  m_mWire_futurePositionInfo.reset();
  m_appFuturePositionInfo = info;
  return *this;
}

bool
FuturePositionInfo::removeAppMetaInfo(uint32_t tlvType)
{
  for (std::list<Block>::iterator iter = m_appFuturePositionInfo.begin();
       iter != m_appFuturePositionInfo.end(); ++iter) {
    if (iter->type() == tlvType) {
    	m_mWire_futurePositionInfo.reset();
    	m_appFuturePositionInfo.erase(iter);
      return true;
    }
  }
  return false;
}

template<encoding::Tag TAG>
size_t
FuturePositionInfo::wireEncode(EncodingImpl<TAG>& encoder) const {
	size_t totalLength = 0;

	for (std::list<Block>::const_reverse_iterator appMetaInfoItem = m_appFuturePositionInfo.rbegin();
	       appMetaInfoItem != m_appFuturePositionInfo.rend(); ++appMetaInfoItem) {
	    totalLength += encoder.prependBlock(*appMetaInfoItem);
	  }

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
		if (m_mWire_futurePositionInfo.hasWire())
			return m_mWire_futurePositionInfo;

		EncodingEstimator estimator;
		size_t estimatedSize = wireEncode(estimator);

		EncodingBuffer buffer(estimatedSize, 0);
		wireEncode(buffer);

		m_mWire_futurePositionInfo = buffer.block();
		return m_mWire_futurePositionInfo;
	}

}

void FuturePositionInfo::wireDecode(const Block& wire) {

	m_mWire_futurePositionInfo = wire;
	m_mWire_futurePositionInfo.parse();

	Block::element_const_iterator val = m_mWire_futurePositionInfo.elements_begin();

	//Dome
	//FuturePosistion
	if (val != m_mWire_futurePositionInfo.elements_end() && val->type() == tlv::FuturePosition) {
		ns3::Vector futurePosition = *val;
		m_location_X_Coord = futurePosition.x;
		m_location_Y_Coord = futurePosition.y;
		m_location_Z_Coord_Velocity = futurePosition.z;
		++val;
	}

	//TimeAtFuturePosition
	if (val != m_mWire_futurePositionInfo.elements_end()
			&& val->type() == tlv::TimeAtFuturePosition) {
		m_time = readNonNegativeInteger(*val);
		++val;
	}

	 // AppFuturePositionInfo (if any)
	  for (; val != m_mWire_futurePositionInfo.elements().end(); ++val) {
	    m_appFuturePositionInfo.push_back(*val);
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

FuturePositionInfo&
FuturePositionInfo::setFutureLocation()

}

