/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "ndn-consumer.hpp"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/callback.h"
#include "ns3/string.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/integer.h"
#include "ns3/double.h"
#include "ns3/node.h"
#include "ns3/wifi-phy.h"
#include "ns3/node-list.h"

#include "utils/ndn-ns3-packet-tag.hpp"
#include "utils/ndn-rtt-mean-deviation.hpp"

#include <ndn-cxx/lp/tags.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>

NS_LOG_COMPONENT_DEFINE("ndn.Consumer");

namespace ns3 {
namespace ndn {

NS_OBJECT_ENSURE_REGISTERED(Consumer);
static int counter = 0;
static std::string allSendAndReceivedData = "";
static int retransmissions = 0;
static std::vector<std::string> distinctPathsTakenOfData;
const int NUMBER_OF_INTERESTS = 100000;
const int NUMBER_OF_DATA = 5;
static std::string results[NUMBER_OF_INTERESTS][NUMBER_OF_DATA];
TypeId
Consumer::GetTypeId(void)
{
  static TypeId tid =
    TypeId("ns3::ndn::Consumer")
      .SetGroupName("Ndn")
      .SetParent<App>()
      .AddAttribute("StartSeq", "Initial sequence number", IntegerValue(0),
                    MakeIntegerAccessor(&Consumer::m_seq), MakeIntegerChecker<int32_t>())

      .AddAttribute("Prefix", "Name of the Interest", StringValue("/"),
                    MakeNameAccessor(&Consumer::m_interestName), MakeNameChecker())
      .AddAttribute("LifeTime", "LifeTime for interest packet", StringValue("12s"),
                    MakeTimeAccessor(&Consumer::m_interestLifeTime), MakeTimeChecker())

      .AddAttribute("RetxTimer",
                    "Timeout defining how frequent retransmission timeouts should be checked",
                    StringValue("50ms"),
                    MakeTimeAccessor(&Consumer::GetRetxTimer, &Consumer::SetRetxTimer),
                    MakeTimeChecker())

      .AddTraceSource("LastRetransmittedInterestDataDelay",
                      "Delay between last retransmitted Interest and received Data",
                      MakeTraceSourceAccessor(&Consumer::m_lastRetransmittedInterestDataDelay),
                      "ns3::ndn::Consumer::LastRetransmittedInterestDataDelayCallback")

      .AddTraceSource("FirstInterestDataDelay",
                      "Delay between first transmitted Interest and received Data",
                      MakeTraceSourceAccessor(&Consumer::m_firstInterestDataDelay),
                      "ns3::ndn::Consumer::FirstInterestDataDelayCallback");

  return tid;
}

Consumer::Consumer()
  : m_rand(CreateObject<UniformRandomVariable>())
  , m_seq(0)
  , m_seqMax(0) // don't request anything
{
  NS_LOG_FUNCTION_NOARGS();
  std::fill(results[0], results[0] +  NUMBER_OF_INTERESTS * NUMBER_OF_DATA, "x");
  m_rtt = CreateObject<RttMeanDeviation>();
}

void
Consumer::SetRetxTimer(Time retxTimer)
{
  m_retxTimer = retxTimer;
  if (m_retxEvent.IsRunning()) {
    // m_retxEvent.Cancel (); // cancel any scheduled cleanup events
    Simulator::Remove(m_retxEvent); // slower, but better for memory
  }

  // schedule even with new timeout
  m_retxEvent = Simulator::Schedule(m_retxTimer, &Consumer::CheckRetxTimeout, this);
}

Time
Consumer::GetRetxTimer() const
{
  return m_retxTimer;
}

void
Consumer::CheckRetxTimeout()
{
  Time now = Simulator::Now();

  Time rto = m_rtt->RetransmitTimeout();
  // NS_LOG_DEBUG ("Current RTO: " << rto.ToDouble (Time::S) << "s");

  while (!m_seqTimeouts.empty()) {
    SeqTimeoutsContainer::index<i_timestamp>::type::iterator entry =
      m_seqTimeouts.get<i_timestamp>().begin();
    if (entry->time + rto <= now) // timeout expired?
    {
      uint32_t seqNo = entry->seq;
      m_seqTimeouts.get<i_timestamp>().erase(entry);
      OnTimeout(seqNo);
    }
    else
      break; // nothing else to do. All later packets need not be retransmitted
  }

  m_retxEvent = Simulator::Schedule(m_retxTimer, &Consumer::CheckRetxTimeout, this);
}

// Application Methods
void
Consumer::StartApplication() // Called at time specified by Start
{
  NS_LOG_FUNCTION_NOARGS();

  // do base stuff
  App::StartApplication();

  ScheduleNextPacket();
}

void
Consumer::StopApplication() // Called at time specified by Stop
{
  NS_LOG_FUNCTION_NOARGS();

  // cancel periodic packet generation
  Simulator::Cancel(m_sendEvent);

  // cleanup base stuff
  App::StopApplication();
}

void
Consumer::SendPacket()
{
	// Dome
	// consumer should send out his feature position, a time stamp to check RTT or time to data income
  if (!m_active)
    return;

  NS_LOG_FUNCTION_NOARGS();
  time::milliseconds interestLifeTime(m_interestLifeTime.GetMilliSeconds());
  ns3::Ptr<ns3::Node> node = ns3::NodeList::GetNode(ns3::Simulator::GetContext());
  int nofinterfaces= node->GetNDevices();
  shared_ptr<Interest> interest2[nofinterfaces] = make_shared<Interest>();
  uint32_t seq2[nofinterfaces];
  shared_ptr<Name> nameWithSequence[nofinterfaces];// = make_shared<Name>(m_interestName);
  //send as many interests as a nodes interface, at the same time
  for (int i=0; i<nofinterfaces; i++){
	  seq2[i]= std::numeric_limits<uint32_t>::max(); // invalid
	  while (m_retxSeqs.size()) {
		  seq2[i] = *m_retxSeqs.begin();
		  m_retxSeqs.erase(m_retxSeqs.begin());
		    break;
	  }

	  if (seq2[i] == std::numeric_limits<uint32_t>::max()) {
		  if (m_seqMax != std::numeric_limits<uint32_t>::max()) {
			  if (m_seq >= m_seqMax) {
				  return; // we are tota lly done
			  }
		  }

		  seq2[i] = m_seq++;

	  }
	  nameWithSequence[i] = make_shared<Name>(m_interestName);
	  nameWithSequence[i]->appendSequenceNumber(seq2[i]);
	  interest2[i]->setNonce(m_rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
	  interest2[i]->setName(*nameWithSequence[i]);
	  WillSendOutInterest(seq2[i]);
	  m_transmittedInterests(interest2[i], this, m_face);
	  m_appLink->onReceiveInterest(*interest2[i]);}

      for (int i=0;i< nofinterfaces;i++){
    	Name name;
    	name = interest2[i]->getName();
    	std::ostringstream tmpName;
    	tmpName << name;
    	std::string interestName = tmpName.str();
    	//LOGS
    	for(int i = 0; i < NUMBER_OF_INTERESTS; i++) {
    		if(results[i][0] == interestName) {
    			retransmissions++;
    			break;
    		} else if(results[i][0] == "x") {
    			results[i][0] = interestName;
    			results[i][2] = std::to_string(Simulator::Now().GetNanoSeconds());
    			break;
    		}

    	}
  }
      ScheduleNextPacket();
}


///////////////////////////////////////////////////
//          Process incoming packets             //
///////////////////////////////////////////////////

void
Consumer::OnData(shared_ptr<const Data> data)
{
	// Dome
	// consumer should now once where the station which is sending data to it.
  if (!m_active)
    return;

  App::OnData(data); // tracing inside

  NS_LOG_FUNCTION(this << data);

  // NS_LOG_INFO ("Received content object: " << boost::cref(*data));

  // This could be a problem......
  uint32_t seq = data->getName().at(-1).toSequenceNumber();
  NS_LOG_INFO("< DATA for " << seq);

  int hopCount = 0;
  auto hopCountTag = data->getTag<lp::HopCountTag>();
  if (hopCountTag != nullptr) { // e.g., packet came from local node's cache
    hopCount = *hopCountTag;
  }
//LOGS
  /*NS_LOG_DEBUG("Hop count: " << hopCount);
  std::cout << "*****************************************************************" << std::endl;
  std::cout << "......consumer receiving data for: " << seq << " with hopcount: " << hopCount << " and name " << data->getName() << std::endl;
  std::cout << "*****************************************************************" << std::endl;
  Name name;
  name = data->getName();
  std::ostringstream tmpName;
  tmpName << name;
  std::string dataName = tmpName.str();
  for(int i = 0; i < NUMBER_OF_INTERESTS; i++) {
	  if(results[i][0] == dataName && results[i][0] != "OK") {
		  results[i][1] = "OK";
		  results[i][3] = std::to_string(Simulator::Now().GetNanoSeconds());
		  results[i][4] = std::to_string(std::stod(results[i][3]) - std::stod(results[i][2]));
	  }
	  else if(results[i][0] == "x") {
		  break;
	  }

  }
  double totalLatency = 0;
  int arrivedData = 0;
  int send = 0;
  for(int i = 0; i < NUMBER_OF_INTERESTS; i++) {
	  if(results[i][0] != "x") {
		  std::cout << results[i][0] << " - " << results[i][1] << " - " << results[i][2] << " - " << results[i][3] << " - "<< results[i][4] << std::endl;
		  send++;
		  if(results[i][1] == "OK") {
			  arrivedData++;
			  totalLatency += std::stod(results[i][4]);
		  }
	  } else {
			break;
		}
	}
  	std::cout << "-> allSendAndReceivedData: \n\n" << allSendAndReceivedData << "\n"<< std::endl;
	std::cout << "-- -- -->> " << arrivedData << "/" << send << "<<-- -- --" << std::endl;
    std::cout << "-- -- -->> RETRANSMISSIONS: " << retransmissions << "<<-- -- --" << std::endl;
    std::cout << "-- -- -->> RETRANSMISSIONS + SENDS: " << retransmissions + send << "<<-- -- --\n" << std::endl;
	std::cout << "-- -->> average latency: " << totalLatency/arrivedData << " ns <<-- --\n" << std::endl;
	std::cout << "-- -->> average latency: " << (totalLatency/arrivedData)/1000 << " us <<-- --\n" << std::endl;
	std::cout << "-- -->> average latency: " << (totalLatency/arrivedData)/1000000 << " ms <<-- --\n" << std::endl;

  // TODO: delete the following two lines after finished testing.
  counter++;
  std::cout << "counter of received Data Packages is: " << counter << std::endl;
  //END LOGS
   * */

  SeqTimeoutsContainer::iterator entry = m_seqLastDelay.find(seq);
  if (entry != m_seqLastDelay.end()) {
    m_lastRetransmittedInterestDataDelay(this, seq, Simulator::Now() - entry->time, hopCount);
  }

  entry = m_seqFullDelay.find(seq);
  if (entry != m_seqFullDelay.end()) {
    m_firstInterestDataDelay(this, seq, Simulator::Now() - entry->time, m_seqRetxCounts[seq], hopCount);
  }

  m_seqRetxCounts.erase(seq);
  m_seqFullDelay.erase(seq);
  m_seqLastDelay.erase(seq);

  m_seqTimeouts.erase(seq);
  m_retxSeqs.erase(seq);

  m_rtt->AckSeq(SequenceNumber32(seq));
}

void
Consumer::OnNack(shared_ptr<const lp::Nack> nack)
{
  /// tracing inside
  App::OnNack(nack);

  NS_LOG_INFO("NACK received for: " << nack->getInterest().getName()
              << ", reason: " << nack->getReason());
}

void
Consumer::OnTimeout(uint32_t sequenceNumber)
{
  NS_LOG_FUNCTION(sequenceNumber);

  m_rtt->IncreaseMultiplier(); // Double the next RTO
  m_rtt->SentSeq(SequenceNumber32(sequenceNumber),
                 1); // make sure to disable RTT calculation for this sample
  m_retxSeqs.insert(sequenceNumber);
  ScheduleNextPacket();
}

void
Consumer::WillSendOutInterest(uint32_t sequenceNumber)
{
  NS_LOG_DEBUG("Trying to add " << sequenceNumber << " with " << Simulator::Now() << ". already "
                                << m_seqTimeouts.size() << " items");

  m_seqTimeouts.insert(SeqTimeout(sequenceNumber, Simulator::Now()));
  m_seqFullDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqLastDelay.erase(sequenceNumber);
  m_seqLastDelay.insert(SeqTimeout(sequenceNumber, Simulator::Now()));

  m_seqRetxCounts[sequenceNumber]++;

  m_rtt->SentSeq(SequenceNumber32(sequenceNumber), 1);
}

} // namespace ndn
} // namespace ns3
