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
static_assert(std::is_base_of<tlv::Error, FuturePositionInfo::Error>::value,
              "FuturePositionInfo::Error must inherit from tlv::Error");

FuturePositionInfo::FuturePositionInfo() {
	m_location_X_Coord = 0;
	m_location_Y_Coord = 0;
	m_location_Z_Coord_Velocity = 0;
	m_timeAtFuturePosition = 0;
	m_bool_position_is_empty = true;

}
/*bool
FuturePositionInfo::empty() const
{
  return m_location_X_Coord <0 &&
		  m_location_Y_Coord = 0 &&
		  m_location_Z_Coord_Velocity = 0 &&
		  m_timeAtFuturePosition = 0;
		 m_bool_position_is_empty = true;;

}*/
FuturePositionInfo::FuturePositionInfo(const Block& block)
{
	wireDecode(block);
}

FuturePositionInfo::FuturePositionInfo(ns3::Vector positionVector, double timeAtFutureLocation) {
	m_location_X_Coord = positionVector.x;
	m_location_Y_Coord = positionVector.y;
	m_location_Z_Coord_Velocity = positionVector.z;
	m_timeAtFuturePosition = timeAtFutureLocation;
	m_bool_position_is_empty = false;

}

template<encoding::Tag TAG>
size_t
FuturePositionInfo::wireEncode(EncodingImpl<TAG>& encoder) const {
	size_t totalLength = 0;

	/*for (std::list<Block>::const_reverse_iterator appFuturePositionInfoItem = m_appFuturePositionInfo.rbegin();
			appFuturePositionInfoItem != m_appFuturePositionInfo.rend(); ++appFuturePositionInfoItem) {
	    totalLength += encoder.prependBlock(*appFuturePositionInfoItem);
	  }*/

	// time
	if (!m_bool_position_is_empty) {
				totalLength += prependNonNegativeIntegerBlock(encoder,
						tlv::FuturePositionZ, m_location_Z_Coord_Velocity);
			}

	// if positions are not emptiy encode them
	if (!m_bool_position_is_empty) {

			totalLength += prependNonNegativeIntegerBlock(encoder,
					tlv::TimeAtFuturePosition, m_timeAtFuturePosition);
		}

	if (!m_bool_position_is_empty) {
				totalLength += prependNonNegativeIntegerBlock(encoder,
						tlv::FuturePositionY, m_location_Y_Coord);
			}

	if (!m_bool_position_is_empty) {
		totalLength += prependNonNegativeIntegerBlock(encoder,
				tlv::FuturePositionX, m_location_X_Coord);
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

template size_t
FuturePositionInfo::wireEncode<encoding::EncoderTag>(EncodingImpl<encoding::EncoderTag>& encoder) const;

template size_t
FuturePositionInfo::wireEncode<encoding::EstimatorTag>(EncodingImpl<encoding::EstimatorTag>& encoder) const;

void
FuturePositionInfo::wireDecode(const Block& wire) {

	m_mWire_futurePositionInfo = wire;
	m_mWire_futurePositionInfo.parse();

	//Block::element_const_iterator val = m_mWire_futurePositionInfo.elements_begin();

	//Dome !!! missnign else statements if decoding fales
	//FuturePosistion
	Block::element_const_iterator val= m_mWire_futurePositionInfo.find(tlv::FuturePositionX);
	if (val != m_mWire_futurePositionInfo.elements_end()) {
		m_location_X_Coord = readNonNegativeInteger(*val);
		++val;
	}
	val= m_mWire_futurePositionInfo.find(tlv::FuturePositionY);
	if (val != m_mWire_futurePositionInfo.elements_end()) {
		m_location_Y_Coord = readNonNegativeInteger(*val);

		++val;
	}
	val= m_mWire_futurePositionInfo.find(tlv::FuturePositionZ);
	if (val != m_mWire_futurePositionInfo.elements_end()) {
		m_location_Z_Coord_Velocity = readNonNegativeInteger(*val);
		++val;
	}
	val= m_mWire_futurePositionInfo.find(tlv::TimeAtFuturePosition);
	//TimeAtFuturePosition
	if (val != m_mWire_futurePositionInfo.elements_end())
		{
		m_timeAtFuturePosition = readNonNegativeInteger(*val);
		++val;
	}

	 // AppFuturePositionInfo (if any)
	 // for (; val != m_mWire_futurePositionInfo.elements().end(); ++val) {
	//    m_appFuturePositionInfo.push_back(*val);
	//  }
}

std::ostream&
operator<<(std::ostream& os, const FuturePositionInfo& info)
{
  // FuturePositionX
  os << "FuturePositionX: " << info.getLocation_X();

  // FuturePositionY
  os << ", FuturePositionY: " << info.getLocation_Y();

  // FuturePositionZ
  os << ", FuturePositionZ: " << info.getLocation_Z();

  // TimeAtFuturePosition
   os << ", TimeAtFuturePosition: " << info.getTimeAtFutureLocation();


  // App-defined MetaInfo items


  return os;
}

bool
FuturePositionInfo::operator ==(const FuturePositionInfo& other) const{
	return wireEncode() == other.wireEncode();
}


/*
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

FuturePositionInfo&
FuturePositionInfo::addAppFuturePositionInfo(const Block& block)
{
  if (!(128 <= block.type() && block.type() <= 252))
    BOOST_THROW_EXCEPTION(Error("AppFuturePositionInfo block has type outside the application range "
                                "[128, 252]"));

  m_mWire_futurePositionInfo.reset();
  m_appFuturePositionInfo.push_back(block);
  return *this;
}

bool
FuturePositionInfo::removeAppFuturePositionInfo(uint32_t tlvType)
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

const Block*
FuturePositionInfo::findAppFuturePositionInfo(uint32_t tlvType) const
{
  for (std::list<Block>::const_iterator iter = m_appFuturePositionInfo.begin();
       iter != m_appFuturePositionInfo.end(); ++iter) {
    if (iter->type() == tlvType) {
      return &*iter;
    }
  }
  return 0;
}
//eigendlich bis hier outcommented
double
FuturePositionInfo::getTimeAtFutureLocation() const{
	return m_timeAtFuturePosition;
}
*/

FuturePositionInfo&
FuturePositionInfo::setTimeAtFutureLocation(double timeAtFutureLocation){
	m_mWire_futurePositionInfo.reset();
	m_timeAtFuturePosition = timeAtFutureLocation;
	return *this;
}

ns3::Vector
FuturePositionInfo::getFuturePositionVector() {
	m_futurePositionVector.x = m_location_X_Coord;
	m_futurePositionVector.y = m_location_Y_Coord;
	m_futurePositionVector.z = m_location_Z_Coord_Velocity;
	return m_futurePositionVector;
}


FuturePositionInfo&
FuturePositionInfo::setFuturePositionVector(ns3::Vector positionVector){
	m_mWire_futurePositionInfo.reset();
	m_futurePositionVector = positionVector;
	return *this;
}

FuturePositionInfo&
FuturePositionInfo::setFutureLocationX(double futureLocation_X){
	m_mWire_futurePositionInfo.reset();
	m_location_X_Coord = futureLocation_X;
	return *this;
}
FuturePositionInfo&
FuturePositionInfo::setFutureLocationY(double futureLocation_Y){
	m_mWire_futurePositionInfo.reset();
	m_location_Y_Coord = futureLocation_Y;
	return *this;
}
FuturePositionInfo&
FuturePositionInfo::setFutureLocationZ(double futureLocation_Z){
	m_mWire_futurePositionInfo.reset();
	m_location_Z_Coord_Velocity = futureLocation_Z;
	return *this;
}
//
//double
//FuturePositionInfo::getLocation_X() {
//	return m_location_X_Coord ;
//
//}
//
//
//double
//FuturePositionInfo::getLocation_Y() {
//	return m_location_Y_Coord ;
//}
//
//double
//FuturePositionInfo::getLocation_Z() {
//	return m_location_Z_Coord_Velocity ;
//
//}
}

