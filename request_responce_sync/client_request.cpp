//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>
#include <fstream>


enum class MSG_TYPE {
	TYPE_1,
	TYPE_2,
	TYPE_3,
	TYPE_4
};

struct Msg_TYPE_1 {
	MSG_TYPE msg_type; 						// тип сообщения
	uint32_t msg_uid;						// ид сообщения (счетчик для контроля)
	uint64_t send_utc_timestamp_ms;			// UTC времени отправки в мсек,заполняется перед send()

	uint64_t msg_data_len = 20;					// длина данных, на которые указывает p_msg_date
	uint8_t  data[20];						// указатель на данные, длина хранится в msg_data_len
};

struct Msg_TYPE_2 {
	MSG_TYPE msg_type; 						// тип сообщения
	uint32_t msg_uid;						// ид сообщения (счетчик для контроля)
	uint64_t send_utc_timestamp_ms;			// UTC времени отправки в мсек,заполняется перед send()

	uint64_t msg_data_len = 1024;					// длина данных, на которые указывает p_msg_date
	uint8_t  data[1024];					// указатель на данные, длина хранится в msg_data_len
};

struct Msg_TYPE_3 {
	MSG_TYPE msg_type; 						// тип сообщения
	uint32_t msg_uid;						// ид сообщения (счетчик для контроля)
	uint64_t send_utc_timestamp_ms;			// UTC времени отправки в мсек,заполняется перед send()

	uint64_t msg_data_len = 512000;					// длина данных, на которые указывает p_msg_date
	uint8_t  data[512000];				// указатель на данные, длина хранится в msg_data_len
};


int main (int argc, char **argv) {
	using namespace std;
    using namespace std::chrono;


	if (argc != 2) {
		std::cerr << "Please set name file for result test, example: $> ./sub test.log" << std::endl;
		return 0;
	}

    //  Prepare our context and socket
    zmq::context_t context;
    zmq::socket_t socket (context, zmq::socket_type::req);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect ("tcp://localhost:5555");

	std::vector<std::pair<uint64_t, uint64_t>> time_storage;

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		
		Msg_TYPE_2 *msg = new Msg_TYPE_2(); // compilyator error
		msg->msg_type = MSG_TYPE::TYPE_2;
		msg->msg_uid =  123456;
		msg->data[0] = 1;
		msg->data[1] = 2;
		msg->data[2] = 3;

        zmq::message_t request(sizeof(Msg_TYPE_2));
        memcpy(request.data (), reinterpret_cast<const void *>(msg), sizeof(Msg_TYPE_2));
        
		
		system_clock::time_point now = system_clock::now();
		chrono::nanoseconds d = now.time_since_epoch();
		chrono::milliseconds start_time = chrono::duration_cast<chrono::milliseconds>(d);

		std::cout << "Sending message " << std::endl;

        socket.send(request, zmq::send_flags::none);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (reply, zmq::recv_flags::none);

		now = system_clock::now();
		d = now.time_since_epoch();
		chrono::milliseconds end_time = chrono::duration_cast<chrono::milliseconds>(d);
		time_storage.push_back(std::make_pair(start_time.count(),end_time.count()));

        std::cout << "Received message size = " << reply.size() << std::endl;
    }

	std::ofstream out("./" + std::string(argv[1]), std::ios_base::out | std::ios_base::trunc);
	out << "Print Vector Time:"  << std::endl;
	for(const auto& el : time_storage) {
		out << "message create on client time " << el.first << " take client responce time " << el.second << " Время посылки серверу и обратно заняло милисек = " << el.second - el.first <<  std::endl;
	}
    return 0;
}