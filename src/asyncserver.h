#ifndef ASYNCSERVER_H
#define ASYNCSERVER_H

#include <thread>
#include <vector>
#include <memory>
#include <functional>
#include "asynctask.h"

namespace cgserver{
    class AsyncServer{
    public:
	typedef std::shared_ptr <boost::asio::ip::tcp::socket> socket_ptr;
        explicit AsyncServer(int port);
        ~AsyncServer();

	void start();
	void stop();

	void handleAccept(AsyncTaskPtr task, const asio_error &err);
	void doAccept();	
    private:
	void run();
	boost::asio::io_service _service;
	boost::asio::ip::tcp::acceptor _acceptor;
	std::vector<std::thread> threads;
	std::vector<socket_ptr> sockets;
    };
}
#endif
