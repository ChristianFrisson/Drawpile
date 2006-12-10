/*******************************************************************************

   Copyright (C) 2006 M.K.A. <wyrmchild@sourceforge.net>
   
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:
   
   Except as contained in this notice, the name(s) of the above copyright
   holders shall not be used in advertising or otherwise to promote the sale,
   use or other dealings in this Software without prior written authorization.
   
   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

// #include <set>
#include <stdint.h>
//#include <cassert>

#include <signal.h>

#include <sys/time.h>

#include <list>
#include <vector>

#if defined(EV_EPOLL)
#elif defined(EV_KQUEUE)
	#error kqueue() not implemented.
#elif defined(EV_PSELECT)
	#include <sys/select.h> // fd_set, FD* macros, etc.
#elif !defined(EV_SELECT)
	#define EV_SELECT
	#ifdef WIN32
		#include <winsock2.h>
	#else
		#include <sys/select.h> // fd_set, FD* macros, etc.
	#endif
#endif

#if defined( EV_PSELECT )
	#define EV_USE_SIGMASK
#endif

//! Event info container
struct EventInfo
{
	//! Associated file descriptor.
	uint32_t fd;
	
	//! Triggered events.
	uint8_t events;
};

typedef std::vector<EventInfo> EvList;

//! Event I/O abstraction
class Event
{
protected:
	#ifndef EV_NO_EVENT_LIST
	std::list<EventInfo> fd_list;
	#endif
	
	#if defined(EV_EPOLL)
	#elif defined(EV_KQUEUE)
	#elif defined(EV_PSELECT) || defined(EV_SELECT)
	fd_set fds[2], t_fds[2];
	#endif
	
	#if defined(EV_USE_SIGMASK)
	sigset_t *_sigmask;
	#endif
	
	//! Returns the set ID for event type 'ev'.
	inline
	int inSet(int ev) throw();
	
public:
	
	// MinGW is buggy... think happy thoughts :D
	static const int
		//! identifier for 'read' event
		read,
		//! identifier for 'write' event
		write;
	
	//! ctor
	Event();
	
	//! dtor
	~Event() throw();
	
	#if defined(EV_USE_SIGMASK)
	//! Set signal mask.
	/**
	 * Only used by pselect() so far
	 */
	void setMask(sigset_t* mask) throw() { _sigmask = mask; }
	#endif
	
	//! Initialize event system.
	void init() throw();
	
	//! Finish event system.
	void finish() throw();
	
	//! Wait for events.
	/**
	 * @param secs seconds to wait.
	 * @param nsecs nanoseconds to wait (may be truncated to milli/microseconds
	 * depending on event system)
	 *
	 * @return number of file descriptors triggered, -1 on error, and 0 otherwise.
	 */
	int wait(uint32_t secs, uint32_t nsecs) throw();
	
	#ifndef EV_NO_EVENT_LIST
	//! Returns a vector of triggered sockets.
	EvList getEvents( int count ) const;
	#endif
	
	//! Adds file descriptor to event polling.
	/**
	 * @param fd is the file descriptor to be added to event stack.
	 * @param ev is the events in which fd is to be found.
	 *
	 * @return true if the fd was added, false if not
	 */
	int add(uint32_t fd, int ev) throw();
	
	//! Removes file descriptor from event set.
	/**
	 * @param fd is the file descriptor to be removed from a event set.
	 * @param ev is the event sets in which fd is to be found.
	 *
	 * @return true if the fd was removed, false if not (or was not part of the event set)
	 */
	int remove(uint32_t fd, int ev) throw();
	
	//! Tests if the file descriptor was triggered in event set.
	/**
	 * @param fd is the file descriptor to be tested in a set.
	 * @param ev is the fd set in which fd is to be found.
	 *
	 * @return true if fd was triggered, false if not (or was not part of the set)
	 */
	int isset(uint32_t fd, int ev) throw();
};

#endif // EVENT_H_INCLUDED