#include "asyncserver.h"

using boost::asio::ip::tcp;
namespace cgserver{
    AsyncServer::AsyncServer(asio_service &service, int port)
	:_service(service),_acceptor(_service, tcp::endpoint(tcp::v4(), port))
    {
	_stop = false;
    }
    
    AsyncServer::~AsyncServer(){
	_work.reset();
    }

    void AsyncServer::run() {
	_service.run();
    }

    void AsyncServer::start() {
	std::cout << "start server." << std::endl;	
	doAccept();
	// auto f = std::bind(&AsyncServer::run, this);
	// for (int i = 0; i < 8; ++i) {
	//     std::thread t(f);
	// }
	//_work.reset(new asio_service::work(_service));
	std::cout << "service run" << std::endl;
	_service.run();
    }

    void AsyncServer::doAccept() {
	if (_stop){
	    return;
	}
	_task.reset(new AsyncTask(_service));
	_acceptor.async_accept(
	    _task->getSocket(),
	    std::bind(&AsyncServer::handleAccept, this, _task, std::placeholders::_1));
	std::cout << "finish doaccept"<< std::endl;
    }

    void AsyncServer::stop() {
	this->_work.reset();
	_stop = true;
    }

    void AsyncServer::handleAccept(AsyncTaskPtr task, const asio_error &err) {
	std::cout << "New connection come." << std::endl;
	// process this connection
	if (!err) {
	    task->read();
	}
	doAccept();
    }    
}
