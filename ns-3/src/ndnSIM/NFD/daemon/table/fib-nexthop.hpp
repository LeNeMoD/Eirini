/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014-2016,  Regents of the University of California,
 *                           Arizona Board of Regents,
 *                           Colorado State University,
 *                           University Pierre & Marie Curie, Sorbonne University,
 *                           Washington University in St. Louis,
 *                           Beijing Institute of Technology,
 *                           The University of Memphis.
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NFD_DAEMON_TABLE_FIB_NEXTHOP_HPP
#define NFD_DAEMON_TABLE_FIB_NEXTHOP_HPP

#include "core/common.hpp"
#include "face/face.hpp"

namespace nfd {
namespace fib {

/** \class NextHop
 *  \brief represents a nexthop record in FIB entry
 */
class NextHop {
public:
	explicit
	NextHop(Face& face);

	Face&
	getFace() const {
		return *m_face;
	}

	uint64_t getCost() const {
		return m_cost;
	}

	void setCost(uint64_t cost) {
		m_cost = cost;
	}

	void setMac(std::string mac) {
		m_mac = mac;
	}

	std::string getMac() const {
		return m_mac;
	}

	double getPosition() {
		return m_position;
	}
	void setPosition(double pos) {
		m_position(0) = pos(0);
		m_position(1) = pos(1);
		m_position(2) = pos(2);
	}

	void setBaseTime(double baseTime) {
		m_baseTime = baseTime;
	}

	double Mac() const {
		return m_baseTime;
	}

	void setDeltaTime(double deltaTime) {
		m_deltaTime = deltaTime;
	}

	double getDeltaTime() const {
		return m_deltaTime;
	}
private:
	Face* m_face;
	uint64_t m_cost;
	std::string m_mac;
	double m_position;
	double m_baseTime;
	double m_deltaTime;
};

} // namespace fib
} // namespace nfd

#endif // NFD_DAEMON_TABLE_FIB_NEXTHOP_HPP
