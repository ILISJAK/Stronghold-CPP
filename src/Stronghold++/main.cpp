#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>

class APIRequestHandler : public Fastcgipp::Request<char> {
public:
	bool response() override {
		out << "Content-Type: text/html; charset=utf-8\r\n\r\n";
		out << "<html><body>Hello, world!</body></html>";
		return true;
	}
};

int main() {
	Fastcgipp::Manager<APIRequestHandler> fcgi;
	fcgi.start();
	fcgi.join();
	return 0;
}

