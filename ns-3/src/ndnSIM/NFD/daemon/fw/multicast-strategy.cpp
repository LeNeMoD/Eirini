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

#include "multicast-strategy.hpp"
#include "algorithm.hpp"
#include "ns3/node-list.h"
#include "ns3/node.h"

#include "ns3/mobility-module.h"
#include "ns3/object.h"
#include "helper/ndn-fib-helper.hpp"

namespace nfd {
namespace fw {

const Name MulticastStrategy::STRATEGY_NAME(
		"ndn:/localhost/nfd/strategy/multicast/%FD%01");
NFD_REGISTER_STRATEGY(MulticastStrategy);

MulticastStrategy::MulticastStrategy(Forwarder& forwarder, const Name& name) :
		Strategy(forwarder, name) {
}

void MulticastStrategy::afterReceiveInterest(const Face& inFace,
		const Interest& interest, const shared_ptr<pit::Entry>& pitEntry) {
	const fib::Entry& fibEntry = this->lookupFib(*pitEntry);
	const fib::NextHopList& nexthops = fibEntry.getNextHops();
	ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(
			ns3::Simulator::GetContext());
	ns3::Ptr<ns3::MobilityModel> model = node->GetObject<ns3::MobilityModel>();
	ns3::Ns2MobilityHelper ns2MobHelper = ns3::Ns2MobilityHelper("ns-movements-test2-n3.txt");
	ns3::Ptr<ns3::ConstantVelocityMobilityModel> constModel = node->GetObject<ns3::ConstantVelocityMobilityModel>();
	std::cout<< ns2MobHelper.GetSchedPosition(constModel,5) << "  is sceduled position for node "<< node->GetId() << "at time 5 " <<std::endl;
	ns3::Vector pos = model->GetPosition();
	std::cout << ns3::Simulator::Now() << std::endl;
	std::ostringstream addr[node->GetNDevices()];
	std::string currentMacAddresses[node->GetNDevices()];


	for (int index = 0; index < node->GetNDevices(); index++) {
		addr[index] << node->GetDevice(index)->GetAddress();
		currentMacAddresses[index] = addr[index].str().substr(6);
	}
//print to check what the FIB contains
	for (fib::NextHopList::const_iterator it = nexthops.begin();
			it != nexthops.end(); ++it) {
		std::cout << "x,y,z:  " << pos.x << "  " << pos.y << "  " << pos.z
				<< "  " << std::endl;
		std::cout << " FIB: NODE: " << node->GetId() << " name: "
				<< interest.getName() << " face " << it->getFace() << " mac : "
				<< it->getMac() << std::endl << " position-X : "
				<< it->getPositionX() << " position-Y : " << it->getPositionY()
				<< std::endl;
	}
	for (fib::NextHopList::const_iterator it = nexthops.begin();
			it != nexthops.end(); ++it) {
		Face& outFace = it->getFace();
		if (!wouldViolateScope(inFace, interest, outFace)
				&& canForwardToLegacy(*pitEntry, outFace)) {
			int a = 0;
			//Since there are many MAC addresses in each node, we avoid looping between the same node
			for (int index = 0; index < node->GetNDevices(); index++) {
				if (it->getMac() == currentMacAddresses[index]) {
					a = 1;
				}
			}
			if (nexthops.size() == 1 && it->getMac() == "eirini") {
				this->sendInterest(pitEntry, outFace, interest, it->getMac());

			} else {
				if (a == 0) {
					this->sendInterest(pitEntry, outFace, interest,
							it->getMac());
					break;
				}
			}

		}
	}

	if (!hasPendingOutRecords(*pitEntry)) {
		this->rejectPendingInterest(pitEntry);
	}
}

} // namespace fw
} // namespace nfd
