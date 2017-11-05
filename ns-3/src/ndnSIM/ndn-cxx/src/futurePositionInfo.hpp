/*
 * futurePosition.hpp
 *
 *  Created on: Nov 5, 2017
 *      Author: domenico
 */

#ifndef NS_3_FUTUREPOSITION_HPP_
#define NS_3_FUTUREPOSITION_HPP_

#include "encoding/block.hpp"
#include "../../../../../ns-3/src/core/model/vector.h"
#include "util/time.hpp"



namespace ndn {

class FuturePositionInfo : public enable_shared_from_this<Name>
{

public:
  class Error : public tlv::Error
  {
  public:
    explicit
    Error(const std::string& what)
      : tlv::Error(what)
    {
    }
  };

  FuturePositionInfo();

  /**
   * @brief Create from wire encoding
   */
  explicit
  FuturePositionInfo(const Block& block);

  explicit
  FuturePositionInfo(const ns3::Vector futurePositionVector, double timeAtFuturePosition);

  template<encoding::Tag TAG>
  size_t
  wireEncode(EncodingImpl<TAG>& encoder) const;

  const Block&
  wireEncode() const;

  void
  wireDecode(const Block& wire);

  public:

  void
  setFutureLocation(ns3::Vector position);

  double
  getTime();

  ns3::Vector
  getPositionVector();

  double
  getLocation_X();

  double
  getLocation_Y();

  double
  getLocation_Z();


  public:
//  	  ns3::Vector m_futureLocation;
  	  double m_location_X_Coord;
  	  double m_location_Y_Coord;
  	  double m_location_Z_Coord_Velocity;
  	  double m_time;
  	  bool m_bool_position_is_empty;

private:
  	  mutable Block m_futurePositionInfoBlock;
};

//inline bool
//FuturePositionInfo::hasWire() const
//{
//  return m_futurePositionInfoBlock.hasWire();
//}

} //end Namespace ndn

#endif /* NS_3_FUTUREPOSITION_HPP_ */
