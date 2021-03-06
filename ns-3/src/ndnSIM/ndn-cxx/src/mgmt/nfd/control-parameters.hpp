/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2013-2016 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#ifndef NDN_MGMT_NFD_CONTROL_PARAMETERS_HPP
#define NDN_MGMT_NFD_CONTROL_PARAMETERS_HPP

#include "../../encoding/nfd-constants.hpp"
#include "../../name.hpp"
#include "../../util/time.hpp"
#include "../control-parameters.hpp"

namespace ndn {
namespace nfd {

/**
 * \ingroup management
 */
enum ControlParameterField {
	CONTROL_PARAMETER_NAME,
	CONTROL_PARAMETER_FACE_ID,
	CONTROL_PARAMETER_URI,
	CONTROL_PARAMETER_LOCAL_CONTROL_FEATURE,
	CONTROL_PARAMETER_ORIGIN,
	CONTROL_PARAMETER_COST,
	CONTROL_PARAMETER_MAC,
	CONTROL_PARAMETER_POSITION_X,
	CONTROL_PARAMETER_POSITION_Y,
	CONTROL_PARAMETER_POSITION_Z,
	CONTROL_PARAMETER_FUTURE_POSITION_X,
	CONTROL_PARAMETER_FUTURE_POSITION_Y,
	CONTROL_PARAMETER_FUTURE_POSITION_Z,
	CONTROL_PARAMETER_TIME_AT_FUTUREPOSITION,
	CONTROL_PARAMETER_FLAGS,
	CONTROL_PARAMETER_MASK,
	CONTROL_PARAMETER_STRATEGY,
	CONTROL_PARAMETER_EXPIRATION_PERIOD,
	CONTROL_PARAMETER_FACE_PERSISTENCY,
	CONTROL_PARAMETER_UBOUND
};

const std::string CONTROL_PARAMETER_FIELD[CONTROL_PARAMETER_UBOUND] = { "Name",
		"FaceId", "Uri", "LocalControlFeature", "Origin", "Cost", "Mac",
		"PositionX", "PositionY", "PositionZ", "FuturePositionX",
		"FuturePositionY", "FuturePositionZ ", "TimeAtFuturePosition", "Flags", "Mask", "Strategy",
		"ExpirationPeriod", "FacePersistency" };



/**
 * \ingroup management
 * \deprecated use Flags+Mask fields instead
 */
enum LocalControlFeature {
	LOCAL_CONTROL_FEATURE_INCOMING_FACE_ID = 1,
	LOCAL_CONTROL_FEATURE_NEXT_HOP_FACE_ID = 2
};

/**
 * \ingroup management
 * \brief represents parameters in a ControlCommand request or response
 * \sa http://redmine.named-data.net/projects/nfd/wiki/ControlCommand#ControlParameters
 * \details This type is copyable because it's an abstraction of a TLV type.
 */
class ControlParameters: public ndn::mgmt::ControlParameters {
public:
	class Error: public tlv::Error {
	public:
		explicit Error(const std::string& what) :
				tlv::Error(what) {
		}
	};

	ControlParameters();

	explicit
	ControlParameters(const Block& block);

	template<encoding::Tag TAG>
	size_t
	wireEncode(EncodingImpl<TAG>& encoder) const;

	virtual Block
	wireEncode() const final;

	virtual void
	wireDecode(const Block& wire) final;

public:
	// getters & setters
	bool hasName() const {
		return m_hasFields[CONTROL_PARAMETER_NAME];
	}

	const Name&
	getName() const {
		BOOST_ASSERT(this->hasName());
		return m_name;
	}

	ControlParameters&
	setName(const Name& name) {
		m_wire.reset();
		m_name = name;
		m_hasFields[CONTROL_PARAMETER_NAME] = true;
		return *this;
	}

	ControlParameters&
	unsetName() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_NAME] = false;
		return *this;
	}

	bool hasFaceId() const {
		return m_hasFields[CONTROL_PARAMETER_FACE_ID];
	}

	uint64_t getFaceId() const {
		BOOST_ASSERT(this->hasFaceId());
		return m_faceId;
	}

	ControlParameters&
	setFaceId(uint64_t faceId) {
		m_wire.reset();
		m_faceId = faceId;
		m_hasFields[CONTROL_PARAMETER_FACE_ID] = true;
		return *this;
	}

	ControlParameters&
	unsetFaceId() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FACE_ID] = false;
		return *this;
	}

	bool hasUri() const {
		return m_hasFields[CONTROL_PARAMETER_URI];
	}

	const std::string&
	getUri() const {
		BOOST_ASSERT(this->hasUri());
		return m_uri;
	}

	ControlParameters&
	setUri(const std::string& uri) {
		m_wire.reset();
		m_uri = uri;
		m_hasFields[CONTROL_PARAMETER_URI] = true;
		return *this;
	}

	ControlParameters&
	unsetUri() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_URI] = false;
		return *this;
	}

	/**
	 * \deprecated use Flags+Mask fields instead
	 */
	bool hasLocalControlFeature() const {
		return m_hasFields[CONTROL_PARAMETER_LOCAL_CONTROL_FEATURE];
	}

	/**
	 * \deprecated use Flags+Mask fields instead
	 */
	LocalControlFeature getLocalControlFeature() const {
		BOOST_ASSERT(this->hasLocalControlFeature());
		return m_localControlFeature;
	}

	/**
	 * \deprecated use Flags+Mask fields instead
	 */
	ControlParameters&
	setLocalControlFeature(LocalControlFeature localControlFeature) {
		m_wire.reset();
		m_localControlFeature = localControlFeature;
		m_hasFields[CONTROL_PARAMETER_LOCAL_CONTROL_FEATURE] = true;
		return *this;
	}

	/**
	 * \deprecated use Flags+Mask fields instead
	 */
	ControlParameters&
	unsetLocalControlFeature() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_LOCAL_CONTROL_FEATURE] = false;
		return *this;
	}

	bool hasOrigin() const {
		return m_hasFields[CONTROL_PARAMETER_ORIGIN];
	}

	uint64_t getOrigin() const {
		BOOST_ASSERT(this->hasOrigin());
		return m_origin;
	}

	ControlParameters&
	setOrigin(uint64_t origin) {
		m_wire.reset();
		m_origin = origin;
		m_hasFields[CONTROL_PARAMETER_ORIGIN] = true;
		return *this;
	}

	ControlParameters&
	unsetOrigin() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_ORIGIN] = false;
		return *this;
	}

	bool hasCost() const {
		return m_hasFields[CONTROL_PARAMETER_COST];
	}

	uint64_t getCost() const {
		BOOST_ASSERT(this->hasCost());
		return m_cost;
	}

	ControlParameters&
	setCost(uint64_t cost) {
		m_wire.reset();
		m_cost = cost;
		m_hasFields[CONTROL_PARAMETER_COST] = true;
		return *this;
	}

	ControlParameters&
	unsetCost() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_COST] = false;
		return *this;
	}
	bool hasMac() const {
		return m_hasFields[CONTROL_PARAMETER_MAC];
	}

	std::string getMac() const {
		BOOST_ASSERT(this->hasMac());
		return m_mac;
	}

	ControlParameters&
	setMac(std::string mac) {
		m_wire.reset();
		m_mac = mac;
		m_hasFields[CONTROL_PARAMETER_MAC] = true;
		return *this;
	}

	ControlParameters&
	unsetMac() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_MAC] = false;
		return *this;
	}

	bool hasPositionX() const {
		return m_hasFields[CONTROL_PARAMETER_POSITION_X];
	}

	double getPositionX() const {
		BOOST_ASSERT(this->hasPositionX());
		return m_positionX;
	}

	ControlParameters&
	setPositionX(double positionX) {
		m_wire.reset();
		m_positionX = positionX;
		m_hasFields[CONTROL_PARAMETER_POSITION_X] = true;
		return *this;
	}

	ControlParameters&
	unsetPositionX() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_POSITION_X] = false;
		return *this;
	}

	bool hasPositionY() const {
		return m_hasFields[CONTROL_PARAMETER_POSITION_Y];
	}

	double getPositionY() const {
		BOOST_ASSERT(this->hasPositionY());
		return m_positionY;
	}

	ControlParameters&
	setPositionY(double positionY) {
		m_wire.reset();
		m_positionY = positionY;
		m_hasFields[CONTROL_PARAMETER_POSITION_Y] = true;
		return *this;
	}

	ControlParameters&
	unsetPositionY() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_POSITION_Y] = false;
		return *this;
	}

	bool hasPositionZ() const {
		return m_hasFields[CONTROL_PARAMETER_POSITION_Z];
	}

	double getPositionZ() const {
		BOOST_ASSERT(this->hasPositionZ());
		return m_positionZ;
	}

	ControlParameters&
	setPositionZ(double positionZ) {
		m_wire.reset();
		m_positionZ = positionZ;
		m_hasFields[CONTROL_PARAMETER_POSITION_Z] = true;
		return *this;
	}

	ControlParameters&
	unsetPositionZ() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_POSITION_Z] = false;
		return *this;
	}

	bool hasFuturePositionX() const {
		return m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_X];
	}

	double getFuturePositionX() const {
		BOOST_ASSERT(this->hasFuturePositionX());
		return m_futurePositionX;
	}

	ControlParameters&
	setFuturePositionX(double futurePositionX) {
		m_wire.reset();
		m_futurePositionX = futurePositionX;
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_X] = true;
		return *this;
	}

	ControlParameters&
	unsetFuturePositionX() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_X] = false;
		return *this;
	}

	bool hasFuturePositionY() const {
		return m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Y];
	}

	double getFuturePositionY() const {
		BOOST_ASSERT(this->hasFuturePositionY());
		return m_futurePositionY;
	}

	ControlParameters&
	setFuturePositionY(double futurePositionY) {
		m_wire.reset();
		m_futurePositionY = futurePositionY;
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Y] = true;
		return *this;
	}

	ControlParameters&
	unsetFuturePositionY() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Y] = false;
		return *this;
	}

	bool hasFuturePositionZ() const {
		return m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Z];
	}

	double getFuturePositionZ() const {
		BOOST_ASSERT(this->hasFuturePositionZ());
		return m_futurePositionY;
	}

	ControlParameters&
	setFuturePositionZ(double futurePositionZ) {
		m_wire.reset();
		m_futurePositionZ = futurePositionZ;
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Z] = true;
		return *this;
	}

	ControlParameters&
	unsetFuturePositionZ() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FUTURE_POSITION_Z] = false;
		return *this;
	}

	bool hasTimeAtFuturePosition() const {
		return m_hasFields[CONTROL_PARAMETER_TIME_AT_FUTUREPOSITION];
	}

	double getTimeAtFuturePosition() const {
		BOOST_ASSERT(this->hasTimeAtFuturePosition());
		return m_timeAtFuturePosition;
	}

	ControlParameters&
	setTimeAtFuturePosition(double timeAtFuturePosition) {
		m_wire.reset();
		m_timeAtFuturePosition = timeAtFuturePosition;
		m_hasFields[CONTROL_PARAMETER_TIME_AT_FUTUREPOSITION] = true;
		return *this;
	}

	ControlParameters&
	unsetTimeAtFuturePosition() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_TIME_AT_FUTUREPOSITION] = false;
		return *this;
	}

	bool hasFlags() const {
		return m_hasFields[CONTROL_PARAMETER_FLAGS];
	}

	uint64_t getFlags() const {
		BOOST_ASSERT(this->hasFlags());
		return m_flags;
	}

	ControlParameters&
	setFlags(uint64_t flags) {
		m_wire.reset();
		m_flags = flags;
		m_hasFields[CONTROL_PARAMETER_FLAGS] = true;
		return *this;
	}

	ControlParameters&
	unsetFlags() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FLAGS] = false;
		return *this;
	}

	bool hasMask() const {
		return m_hasFields[CONTROL_PARAMETER_MASK];
	}

	uint64_t getMask() const {
		BOOST_ASSERT(this->hasMask());
		return m_mask;
	}

	ControlParameters&
	setMask(uint64_t mask) {
		m_wire.reset();
		m_mask = mask;
		m_hasFields[CONTROL_PARAMETER_MASK] = true;
		return *this;
	}

	ControlParameters&
	unsetMask() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_MASK] = false;
		return *this;
	}

	bool hasStrategy() const {
		return m_hasFields[CONTROL_PARAMETER_STRATEGY];
	}

	const Name&
	getStrategy() const {
		BOOST_ASSERT(this->hasStrategy());
		return m_strategy;
	}

	ControlParameters&
	setStrategy(const Name& strategy) {
		m_wire.reset();
		m_strategy = strategy;
		m_hasFields[CONTROL_PARAMETER_STRATEGY] = true;
		return *this;
	}

	ControlParameters&
	unsetStrategy() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_STRATEGY] = false;
		return *this;
	}

	bool hasExpirationPeriod() const {
		return m_hasFields[CONTROL_PARAMETER_EXPIRATION_PERIOD];
	}

	const time::milliseconds&
	getExpirationPeriod() const {
		BOOST_ASSERT(this->hasExpirationPeriod());
		return m_expirationPeriod;
	}

	ControlParameters&
	setExpirationPeriod(const time::milliseconds& expirationPeriod) {
		m_wire.reset();
		m_expirationPeriod = expirationPeriod;
		m_hasFields[CONTROL_PARAMETER_EXPIRATION_PERIOD] = true;
		return *this;
	}

	ControlParameters&
	unsetExpirationPeriod() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_EXPIRATION_PERIOD] = false;
		return *this;
	}

	bool hasFacePersistency() const {
		return m_hasFields[CONTROL_PARAMETER_FACE_PERSISTENCY];
	}

	FacePersistency getFacePersistency() const {
		BOOST_ASSERT(this->hasFacePersistency());
		return m_facePersistency;
	}

	ControlParameters&
	setFacePersistency(FacePersistency persistency) {
		m_wire.reset();
		m_facePersistency = persistency;
		m_hasFields[CONTROL_PARAMETER_FACE_PERSISTENCY] = true;
		return *this;
	}

	ControlParameters&
	unsetFacePersistency() {
		m_wire.reset();
		m_hasFields[CONTROL_PARAMETER_FACE_PERSISTENCY] = false;
		return *this;
	}

	const std::vector<bool>&
	getPresentFields() const {
		return m_hasFields;
	}

public:
	// Flags and Mask helpers
	/**
	 * \return whether bit is enabled in Mask
	 * \param bit bit position within range [0, 64) (least significant bit is 0)
	 */
	bool
	hasFlagBit(size_t bit) const;

	/**
	 * \return bit at a position in Flags
	 * \param bit bit position within range [0, 64) (least significant bit is 0)
	 */
	bool
	getFlagBit(size_t bit) const;

	/**
	 * \brief set a bit in Flags
	 * \param bit bit position within range [0, 64) (least significant bit is 0)
	 * \param value new value in Flags
	 * \param wantMask if true, enable the bit in Mask
	 */
	ControlParameters&
	setFlagBit(size_t bit, bool value, bool wantMask = true);

	/**
	 * \brief disable a bit in Mask
	 * \param bit bit position within range [0, 64) (least significant bit is 0)
	 * \post If all bits are disabled, Flags and Mask fields are deleted.
	 */
	ControlParameters&
	unsetFlagBit(size_t bit);

private:
	// fields
	std::vector<bool> m_hasFields;

	Name m_name;
	uint64_t m_faceId;
	std::string m_uri;
	std::string m_mac;
	double m_positionX;
	double m_positionY;
	double m_positionZ;
	double m_futurePositionX;
	double m_futurePositionY;
	double m_futurePositionZ;
	double m_timeAtFuturePosition;
	LocalControlFeature m_localControlFeature;
	uint64_t m_origin;
	uint64_t m_cost;
	uint64_t m_flags;
	uint64_t m_mask;
	Name m_strategy;
	time::milliseconds m_expirationPeriod;
	FacePersistency m_facePersistency;

private:
	mutable Block m_wire;
};

std::ostream&
operator<<(std::ostream& os, const ControlParameters& parameters);

} // namespace nfd
} // namespace ndn

#endif // NDN_MGMT_NFD_CONTROL_PARAMETERS_HPP
