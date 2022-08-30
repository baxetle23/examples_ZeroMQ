#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include "zmq.hpp"
#include "zmq_addon.hpp"
#include <fstream>


#define COUNT 10


uint64_t work_with_buffer(zmq::mutable_buffer& buf) {
	uint8_t *data = (uint8_t *)buf.data();
	for(size_t i = 0; i < 27; ++i) {
		std::cout << (int32_t)data[i] << " ";
	}
	std::cout << std::endl;
	return *reinterpret_cast<uint64_t *>(data + 8);
}

int main (int argc, char const *argv[]) {
    using namespace std;
    using namespace std::chrono;


	zmq::context_t context;
	zmq::socket_t socket(context, ZMQ_SUB);

	socket.connect("tcp://127.0.0.1:9999");
	const char* filter = ""; //фильтр сообщений общая подписка
	socket.setsockopt(ZMQ_SUBSCRIBE, filter, strlen(filter));
	std::vector<std::pair<uint64_t, uint64_t>> time_storage;
	std::vector<uint8_t> buffer(100000);

	while (true) {
		zmq::mutable_buffer buf(buffer.data(), buffer.size());
		zmq::recv_buffer_result_t result = socket.recv(buf, zmq::recv_flags::dontwait);
		
		if (result.has_value()) {

			std::cout << "Message received! size = " << result.value().size << std::endl;
			uint64_t start_time = work_with_buffer(buf);

			system_clock::time_point now = system_clock::now();
			chrono::nanoseconds d = now.time_since_epoch();
			chrono::milliseconds end_time = chrono::duration_cast<chrono::milliseconds>(d);
			time_storage.push_back(std::make_pair(start_time,end_time.count()));
		}
		if (time_storage.size() == 20) {
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	std::ofstream out("./" + std::string(argv[1]), std::ios_base::out | std::ios_base::trunc);
	out << "Print Vector Time:"  << std::endl;
	for(const auto& el : time_storage) {
		out << "message create on server " << el.first << " take client " << el.second << " Время посылки заняло милисек = " << el.second - el.first <<  std::endl;
	}
}