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
class FuturePositionInfo;
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

  /*const std::list<Block>&
  getAppFuturePositionInfo() const ;

  FuturePositionInfo&
  setAppFuturePositionInfo(const std::list<Block>& info);

  FuturePositionInfo&
  addAppFuturePositionInfo(const Block& block);

  bool
  removeAppFuturePositionInfo(uint32_t tlvType);

  const Block*
  findAppFuturePositionInfo(uint32_t tlvType) const;
  */

  FuturePositionInfo&
  setFuturePositionVector(ns3::Vector position);

  ns3::Vector
  getFuturePositionVector();
  bool
  Getemptyposition() const;

  FuturePositionInfo&
  setFutureLocationX(double futurelocation_X);

  FuturePositionInfo&
  setFutureLocationY(double futureLocation_Y);

  FuturePositionInfo&
  setFutureLocationZ(double futureLocation_Z);

  double
  getLocation_X() const;

  double
  getLocation_Y() const;

  double
  getLocation_Z() const;

  double
  getTimeAtFutureLocation() const;

  FuturePositionInfo&
  setTimeAtFutureLocation(double time);



  public:
  	  double m_location_X_Coord;
  	  double m_location_Y_Coord;
  	  double m_location_Z_Coord_Velocity;
  	  double m_timeAtFuturePosition;
  	  bool m_bool_position_is_empty;
  	  ns3::Vector m_futurePositionVector;


  public: // EqualityComparable concept
    bool
    operator==(const FuturePositionInfo& other) const;

    bool
    operator!=(const FuturePositionInfo& other) const;

private:
  	  std::list<Block> m_appFuturePositionInfo;
  	  mutable Block m_mWire_futurePositionInfo;
};

std::ostream&
operator<<(std::ostream& os, const FuturePositionInfo& info);

inline double
FuturePositionInfo::getLocation_X() const
{
	return m_location_X_Coord;
}
inline bool
FuturePositionInfo::Getemptyposition() const
{
	return m_bool_position_is_empty;
}
inline double
FuturePositionInfo::getLocation_Y() const
{
	return m_location_Y_Coord;
}

inline double
FuturePositionInfo::getLocation_Z() const
{
	return m_location_Z_Coord_Velocity;
}

inline double
FuturePositionInfo::getTimeAtFutureLocation() const
{
	return m_timeAtFuturePosition;
}
//inline bool
//FuturePositionInfo::hasWire() const
//{
//  return m_mWire_futurePositionInfo.hasWire();
//}

} //end Namespace ndn

#endif /* NS_3_FUTUREPOSITION_HPP_ */
