#ifndef CRYPTOPP_CHANNELS_H
#define CRYPTOPP_CHANNELS_H
// *****************************************************************************
// channels.h                                                      Tao3D project
// *****************************************************************************
//
// File description:
//
//
//
//
//
//
//
//
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2011, Jérôme Forissier <jerome@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

#include "simple.h"
#include "smartptr.h"
#include <map>
#include <list>

NAMESPACE_BEGIN(CryptoPP)

#if 0
//! Route input on default channel to different and/or multiple channels based on message sequence number
class MessageSwitch : public Sink
{
public:
	void AddDefaultRoute(BufferedTransformation &destination, const std::string &channel);
	void AddRoute(unsigned int begin, unsigned int end, BufferedTransformation &destination, const std::string &channel);

	void Put(byte inByte);
	void Put(const byte *inString, unsigned int length);

	void Flush(bool completeFlush, int propagation=-1);
	void MessageEnd(int propagation=-1);
	void PutMessageEnd(const byte *inString, unsigned int length, int propagation=-1);
	void MessageSeriesEnd(int propagation=-1);

private:
	typedef std::pair<BufferedTransformation *, std::string> Route;
	struct RangeRoute
	{
		RangeRoute(unsigned int begin, unsigned int end, const Route &route)
			: begin(begin), end(end), route(route) {}
		bool operator<(const RangeRoute &rhs) const {return begin < rhs.begin;}
		unsigned int begin, end;
		Route route;
	};

	typedef std::list<RangeRoute> RouteList;
	typedef std::list<Route> DefaultRouteList;

	RouteList m_routes;
	DefaultRouteList m_defaultRoutes;
	unsigned int m_nCurrentMessage;
};
#endif

class ChannelSwitchTypedefs
{
public:
	typedef std::pair<BufferedTransformation *, std::string> Route;
	typedef std::multimap<std::string, Route> RouteMap;

	typedef std::pair<BufferedTransformation *, value_ptr<std::string> > DefaultRoute;
	typedef std::list<DefaultRoute> DefaultRouteList;

	// SunCC workaround: can't use const_iterator here
	typedef RouteMap::iterator MapIterator;
	typedef DefaultRouteList::iterator ListIterator;
};

class ChannelSwitch;

class ChannelRouteIterator : public ChannelSwitchTypedefs
{
public:
	ChannelSwitch& m_cs;
	std::string m_channel;
	bool m_useDefault;
	MapIterator m_itMapCurrent, m_itMapEnd;
	ListIterator m_itListCurrent, m_itListEnd;

	ChannelRouteIterator(ChannelSwitch &cs) : m_cs(cs) {}
	void Reset(const std::string &channel);
	bool End() const;
	void Next();
	BufferedTransformation & Destination();
	const std::string & Channel();
};

//! Route input to different and/or multiple channels based on channel ID
class CRYPTOPP_DLL ChannelSwitch : public Multichannel<Sink>, public ChannelSwitchTypedefs
{
public:
	ChannelSwitch() : m_it(*this), m_blocked(false) {}
	ChannelSwitch(BufferedTransformation &destination) : m_it(*this), m_blocked(false)
	{
		AddDefaultRoute(destination);
	}
	ChannelSwitch(BufferedTransformation &destination, const std::string &outChannel) : m_it(*this), m_blocked(false)
	{
		AddDefaultRoute(destination, outChannel);
	}

	void IsolatedInitialize(const NameValuePairs &parameters=g_nullNameValuePairs);

	size_t ChannelPut2(const std::string &channel, const byte *begin, size_t length, int messageEnd, bool blocking);
	size_t ChannelPutModifiable2(const std::string &channel, byte *begin, size_t length, int messageEnd, bool blocking);

	bool ChannelFlush(const std::string &channel, bool completeFlush, int propagation=-1, bool blocking=true);
	bool ChannelMessageSeriesEnd(const std::string &channel, int propagation=-1, bool blocking=true);

	byte * ChannelCreatePutSpace(const std::string &channel, size_t &size);
	
	void AddDefaultRoute(BufferedTransformation &destination);
	void RemoveDefaultRoute(BufferedTransformation &destination);
	void AddDefaultRoute(BufferedTransformation &destination, const std::string &outChannel);
	void RemoveDefaultRoute(BufferedTransformation &destination, const std::string &outChannel);
	void AddRoute(const std::string &inChannel, BufferedTransformation &destination, const std::string &outChannel);
	void RemoveRoute(const std::string &inChannel, BufferedTransformation &destination, const std::string &outChannel);

private:
	RouteMap m_routeMap;
	DefaultRouteList m_defaultRoutes;

	ChannelRouteIterator m_it;
	bool m_blocked;

	friend class ChannelRouteIterator;
};

NAMESPACE_END

#endif
