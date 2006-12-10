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

   ---

   For more info, see: http://drawpile.sourceforge.net/

*******************************************************************************/

#ifndef Server_C_Included
#define Server_C_Included

#include "sockets.h"
#include "event.h"
#include "user.h"

//#include <sys/time.h>
#include <getopt.h> // for command-line opts

#include <bitset>
#include <map>
#include <list>
#include <vector>

//! foo
namespace defaults
{

//! Hard limit for users and sessions.
const int hard_limit = 255;

} // namespace defaults

//! Server
class Server
{
protected:
	/* data */
	
	Event ev;
	
	std::bitset<defaults::hard_limit> user_ids;
	std::bitset<defaults::hard_limit> session_ids;
	
	std::vector<uint32_t> sockets;
	std::map<uint32_t, User*> users;
	std::map<uint8_t, uint32_t> user_id_map;
	
	// listening socket
	Socket lsock;
	
	char* password;
	size_t pw_len,
		user_limit,
		cur_users;

	uint16_t
		hi_port,
		lo_port;

	/* functions */

	//! Frees user ID
	void freeUserID(uint8_t id);

	//! Frees session ID
	void freeSessionID(uint8_t id);

	//! Cleanup anything that's left.
	inline
	void cleanup(int rc);
	
	//! Get free user ID
	uint8_t getUserID();

	//! Get free session ID
	uint8_t getSessionID();

	//! Write to user socket
	void uWrite(uint32_t fd);

	//! Read from user socket
	void uRead(uint32_t fd);

public:
	//! ctor
	Server();
	
	//! dtor
	~Server();
	
	//! Initializes anything that need to be done so.
	int init();
	
	//! Parses command-line args
	void getArgs(int argc, char** argv);
	
	//! Enter main loop
	int run();
}; // class Server

#endif // Server_C_Included