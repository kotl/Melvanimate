
#pragma once

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "Melvanimate.h"
#include "IPAddress.h"
#include <functional>

#define DebugMelvanimateMQTT

#ifdef DebugMelvanimateMQTT
#define DebugMelvanimateMQTTf(...) Serial.printf(__VA_ARGS__)
#else
#define DebugMelvanimateMQTTf(...) {}
#endif

using namespace std::placeholders;

class Melvanimate;

class MelvanimateMQTT
{
public:

	MelvanimateMQTT(Melvanimate * lights, IPAddress Addr, uint16_t Port = 1883) : _melvanimate(lights)
	{
		_client.setClient(_espClient);
		_client.setServer(Addr, Port);
		_client.setCallback(  std::bind (&MelvanimateMQTT::_handle, this, _1, _2, _3));

	}
	~MelvanimateMQTT() {}

	void loop();

	operator bool()
	{
		return _client.connected();
	}

	bool publish(const char * topic, const char * payload)
	{
		return 	_client.publish( topic, payload );
	}


private:

	void _reconnect();
	void _handle(char* topic, byte* payload, unsigned int length);

	WiFiClient _espClient;
	PubSubClient _client;
	uint32_t _reconnectTimer{0};
	Melvanimate * _melvanimate{nullptr};

};