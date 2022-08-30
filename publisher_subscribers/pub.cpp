#include <iostream>
#include <zmq_addon.hpp>
#include <zmq.hpp>
#include <thread>
#include <chrono>
#include <ctime>


#define SLEEP_TIME_MS 1000

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


int main() {

	using namespace std;
    using namespace std::chrono;

	zmq::context_t context;
	zmq::socket_t socket(context, ZMQ_PUB);
	socket.bind("tcp://127.0.0.1:9999");

	
	while(true) {
		system_clock::time_point now = system_clock::now();
		chrono::nanoseconds d = now.time_since_epoch();
		chrono::milliseconds millsec = chrono::duration_cast<chrono::milliseconds>(d);
		Msg_TYPE_2 *msg = new Msg_TYPE_2(); // compilyator error
		msg->msg_type = MSG_TYPE::TYPE_2;
		msg->msg_uid =  123456;
		msg->send_utc_timestamp_ms =  millsec.count();
		msg->data[0] = 1;
		msg->data[1] = 2;
		msg->data[2] = 3;

		zmq::message_t message(sizeof(Msg_TYPE_2));
		memcpy(message.data(), reinterpret_cast<const void *>(msg), sizeof(Msg_TYPE_2));

		std::cout << "Send message, size " << message.size() << std::endl;
		
		socket.send(message);
		
		delete msg;

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
	}
}
