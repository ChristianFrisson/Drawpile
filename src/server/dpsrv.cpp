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

#include "../shared/protocol.h"

#include "sockets.h"
#include "user.h"
#include "event.h"
#include "server.h"

//#include <sys/time.h>

#include <bitset>
#include <map>
#include <list>
#include <vector>
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
	std::ios::sync_with_stdio(false);
	
	std::cout << "dpsrv v0.0a\n(C) 2006 M.K.A.\n" << std::endl;
	
	Server srv;
	
	srv.getArgs(argc, argv);
	
	if (srv.init() != 0)
		return 1;
	
	netInit();
	
	std::cout << "running main" << std::endl;
	
	srv.run();
	
	std::cout << "done" << std::endl;
	
	netStop();
	
	return 0; // never reached
}