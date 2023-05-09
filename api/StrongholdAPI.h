#include <C:\vcpkg\packages\cpprestsdk_x64-windows\include\cpprest/http_listener.h>
#include <C:\vcpkg\packages\cpprestsdk_x64-windows\include\cpprest/json.h>
#include "Exception.h"
#include "Town.h"
#include "Archer.h"
#include "Knight.h"
#include "Maceman.h"
#include "Pikeman.h"
#include "Market.h"
#include "WoodCamp.h"
#include "WheatFarm.h"
#include "StoneQuarry.h"

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