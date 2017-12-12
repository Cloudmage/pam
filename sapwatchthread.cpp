#include "sapwatchthread.h"
#include "Groupsock.hh"
#include "GroupsockHelper.hh"
#include "BasicUsageEnvironment.hh"
#include <stdio.h>
#include <wx/string.h>
#include <wx/log.h>

#include "wmlogevent.h"

DEFINE_EVENT_TYPE(wxEVT_SAP)

SapWatchThread::SapWatchThread(wxEvtHandler* pParent) : m_pParent(pParent)
{

}


void* SapWatchThread::Entry()
{
    // Begin by setting up our usage environment:
    TaskScheduler* scheduler = BasicTaskScheduler::createNew();
    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);


      // Create a 'groupsock' for the input multicast group,port:
      char const* sessionAddressStr = "239.255.255.255";
      struct in_addr sessionAddress;
      sessionAddress.s_addr = our_inet_addr(sessionAddressStr);

    const Port port(9875);
    const unsigned char ttl = 0; // we're only reading from this mcast group

    Groupsock inputGroupsock(*env, sessionAddress, port, ttl);

    // Start reading and printing incoming packets
    // (Because this is the only thing we do, we can just do this
    // synchronously, in a loop, so we don't need to set up an asynchronous
    // event handler like we do in most of the other test programs.)
    unsigned packetSize;
    struct sockaddr_in fromAddress;
    while (inputGroupsock.handleRead(m_packet, 65536, packetSize, fromAddress) && !TestDestroy())
    {
        // Ignore the first 8 bytes (SAP header).
        if (packetSize >= 8)
        {

            // convert "application/sdp\0" -> "application/sdp\0x20"
            // or all other nonprintable characters to blank, except new line
            unsigned idx = 8;
            while (idx < packetSize)
            {
                if (m_packet[idx] < 0x20 && m_packet[idx] != '\n')
                    m_packet[idx] = 0x20;
                idx++;
            }
            m_packet[packetSize] = '\0'; // just in case

            wxString sSDP;
            sSDP.Printf(wxT("%s\n%s"), wxString::FromAscii(AddressString(fromAddress).val()).c_str(), wxString::FromAscii((char*)(m_packet+8)).c_str());

            if(m_pParent)
            {
                wxCommandEvent event(wxEVT_SAP);
                event.SetString(sSDP);
                wxPostEvent(m_pParent, event);
            }
        }
    }
    return NULL;
}
