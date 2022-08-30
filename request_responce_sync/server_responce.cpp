#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

#define TIME_WORK_MS 1000
#define COUNT_THREADS 50

bool check_message(zmq::message_t& message) {
	return true;
}

void *worker_routine (void *arg) {
    zmq::context_t *context = (zmq::context_t *) arg;

    zmq::socket_t socket (*context, ZMQ_REP);
    socket.connect ("inproc://workers");
    while (true) {
		zmq::message_t request;
		socket.recv(&request);
		std::cout << "Received message size " << request.size() << std::endl;
		if (!check_message(request)) {
			std::cerr<< "message dont sync" << std::endl;
		}
		//  Do some 'work'
		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_WORK_MS));

		//  Send reply back to client
		zmq::message_t reply (1024);
		std::vector<uint8_t> test (123, 123);
		memcpy(reply.data (), test.data(), test.size());
		socket.send (reply);
    }
    return (NULL);
}

int main () {
    //  Prepare our context and sockets
    zmq::context_t context (1);
    zmq::socket_t clients (context, ZMQ_ROUTER);
    clients.bind ("tcp://*:5555");
    zmq::socket_t workers (context, ZMQ_DEALER);
    workers.bind ("inproc://workers");

    //  Launch pool of worker threads
    for (int thread_nbr = 0; thread_nbr != COUNT_THREADS; thread_nbr++) {
        pthread_t worker;
        pthread_create (&worker, NULL, worker_routine, (void *) &context);
    }
    //  Connect work threads to client threads via a queue
    zmq::proxy(static_cast<void*>(clients), static_cast<void*>(workers), nullptr);
    return 0;
}