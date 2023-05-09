#include "StrongholdAPI.h"

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




