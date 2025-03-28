#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "server.h"

class FrontController  {
private:
	server server_;

public:
	explicit FrontController(short port);
	void run();


};



#endif //REQUESTHANDLER_H
