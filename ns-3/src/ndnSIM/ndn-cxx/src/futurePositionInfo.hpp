/*
 * futurePosition.hpp
 *
 *  Created on: Nov 5, 2017
 *      Author: domenico
 */

#ifndef NS_3_FUTUREPOSITIONINFO_HPP_
#define NS_3_FUTUREPOSITIONINFO_HPP_

#include "encoding/block.hpp"
#include "../../../../../ns-3/src/core/model/vector.h"
#include "util/time.hpp"
#include "common.hpp"
#include "encoding/block.hpp"
#include "encoding/encoding-buffer.hpp"
#include "util/time.hpp"
#include "name-component.hpp"
#include <list>



namespace ndn {

class FuturePositionInfo : public enable_shared_from_this<FuturePositionInfo>
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

  const std::list<Block>&
  getAppFuturePositionInfo() const ;

  FuturePositionInfo&
  setAppFuturePositionInfo(const std::list<Block>& info);

  bool
  removeAppFuturePositionInfo(uint32_t tlvType);


  FuturePositionInfo&
  setFuturePositionVector(ns3::Vector position);

  double
  getTime();

  FuturePositionInfo&
  setTime(double time);

  ns3::Vector
  getFuturePositionVector();

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
  	  ns3::Vector m_futurePositionVector;


private:
  	  std::list<Block> m_appFuturePositionInfo;
  	  mutable Block m_mWire_futurePositionInfo;
};

//inline bool
//FuturePositionInfo::hasWire() const
//{
//  return m_mWire_futurePositionInfo.hasWire();
//}

} //end Namespace ndn

#endif /* NS_3_FUTUREPOSITION_HPP_ */
