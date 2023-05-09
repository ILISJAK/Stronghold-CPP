#include <C:\vcpkg\packages\cpprestsdk_x64-windows\include\cpprest/http_listener.h>
#include <C:\vcpkg\packages\cpprestsdk_x64-windows\include\cpprest/json.h>
#include "Town.h"
#include "Lord.h"

using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class StrongholdApi {
public:
	StrongholdApi() {}
	StrongholdApi(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);

	http_listener m_listener;
};

StrongholdApi::StrongholdApi(utility::string_t url) : m_listener(url) {
	m_listener.support(methods::GET, std::bind(&StrongholdApi::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&StrongholdApi::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&StrongholdApi::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&StrongholdApi::handle_delete, this, std::placeholders::_1));
}

void StrongholdApi::handle_get(http_request message) {
	// 
}

void StrongholdApi::handle_put(http_request message) {
	// 
}

void StrongholdApi::handle_post(http_request message) {
	// 
}

void StrongholdApi::handle_delete(http_request message) {
	// 
}

int main(int argc, char* argv[]) {
	utility::string_t port = U("34568");
	utility::string_t address = U("http://localhost:");
	address.append(port);

	StrongholdApi api(address);
	api.open().wait();

	std::cout << "Press ENTER to exit." << std::endl;
	std::string line;
	std::getline(std::cin, line);

	api.close().wait();

	return 0;
}


