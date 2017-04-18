/**
 * @file
 * @author  Patrick Jahns http://github.com/patrickjahns
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * https://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 *
 */
#pragma once

#include <SmingCore/SmingCore.h>
#include <RGBWWCtrl.h>

#define APP_SETTINGS_FILE ".cfg"
#define CFG_VERSION "1"

struct ApplicationSettings {
	struct network {
		struct connection {
			String mdnshostname;
			bool dhcp = true;
			IPAddress ip;
			IPAddress netmask;
			IPAddress gateway;
		};

		struct mqtt {
			bool enabled = true;
			String server = "minion";
			int port = 1883;
			String username;
			String password;
            String topic_base = "home/";
		};

		struct ap {
			bool secured = DEFAULT_AP_SECURED;
			String ssid;
			String password = DEFAULT_AP_PASSWORD;
		};

		connection connection;
		mqtt mqtt;
		ap ap;
	};

    struct sync {
        bool clock_master_enabled = false;
        int clock_master_interval = 5;

        bool clock_slave_enabled = true;
        String clock_slave_topic= "home/wz_lightLedCouch/clock";

        bool cmd_master_enabled = false;
        bool cmd_slave_enabled = false;
        String cmd_slave_topic = "home/wz_lightLedCouch/commands";
    };

    struct events {
        int colorEventIntervalMs = 500;
    };

	struct color {
		struct hsv {
			int model = 0;
			float red = 0;
			float yellow = 0;
			float green = 0;
			float cyan = 0;
			float blue = 0;
			float magenta = 0;
		};

		struct brightness {
			int red = 100;
			int green = 100;
			int blue = 100;
			int ww = 100;
			int cw = 100;
		};

		struct colortemp {
			int ww = DEFAULT_COLORTEMP_WW;
			int cw = DEFAULT_COLORTEMP_CW;
		};

		hsv hsv;
		brightness brightness;
		colortemp colortemp;
		int outputmode = 0;
	};

	struct general {
		bool api_secured = DEFAULT_API_SECURED;
		String api_password = DEFAULT_API_PASSWORD;
		String otaurl = DEFAULT_OTA_URL;
		String device_name;
	};

	general general;
	network network;
	color color;
	String configversion = CFG_VERSION;
    sync sync;
    events events;

	void load(bool print = false) {
		DynamicJsonBuffer jsonBuffer;
		if (exist()) {
			int size = fileGetSize(APP_SETTINGS_FILE);
			char* jsonString = new char[size + 1];
			fileGetContent(APP_SETTINGS_FILE, jsonString, size + 1);
			JsonObject& root = jsonBuffer.parseObject(jsonString);

			// connection
			network.connection.mdnshostname =
					root["network"]["connection"]["hostname"].asString();
			network.connection.dhcp = root["network"]["connection"]["dhcp"];
			network.connection.ip = root["network"]["connection"]["ip"].asString();
			network.connection.netmask = root["network"]["connection"]["netmask"].asString();
			network.connection.gateway = root["network"]["connection"]["gateway"].asString();

			// accesspoint
			network.ap.secured = root["network"]["ap"]["secured"];
			network.ap.ssid = root["network"]["ap"]["ssid"].asString();
			network.ap.password = root["network"]["ap"]["password"].asString();

			// mqtt
			network.mqtt.enabled = root["network"]["mqtt"]["enabled"];
			network.mqtt.server = root["network"]["mqtt"]["server"].asString();
			network.mqtt.port = root["network"]["mqtt"]["port"];
			network.mqtt.username = root["network"]["mqtt"]["username"].asString();
			network.mqtt.password = root["network"]["mqtt"]["password"].asString();

			// color
			color.outputmode = root["color"]["outputmode"];

			// hsv
			color.hsv.model = root["color"]["hsv"]["model"];
			color.hsv.red = root["color"]["hsv"]["red"];
			color.hsv.yellow = root["color"]["hsv"]["yellow"];
			color.hsv.green = root["color"]["hsv"]["green"];
			color.hsv.cyan = root["color"]["hsv"]["cyan"];
			color.hsv.blue = root["color"]["hsv"]["blue"];
			color.hsv.magenta = root["color"]["hsv"]["magenta"];

			// brightness
			color.brightness.red = root["color"]["brightness"]["red"];
			color.brightness.green = root["color"]["brightness"]["green"];
			color.brightness.blue = root["color"]["brightness"]["blue"];
			color.brightness.ww = root["color"]["brightness"]["ww"];
			color.brightness.cw = root["color"]["brightness"]["cw"];

			// general
			general.api_password = root["general"]["api_password"].asString();
			general.api_secured = root["general"]["api_secured"];
			general.otaurl = root["general"]["otaurl"].asString();
			general.device_name = root["general"]["device_name"].asString();

			// sync
            sync.clock_master_enabled = root["sync"]["clock_master_enabled"];
            sync.clock_master_interval = root["sync"]["clock_master_interval"];
            sync.clock_slave_topic = root["sync"]["clock_slave_topic"].asString();
            sync.clock_slave_enabled = root["sync"]["clock_slave_enabled"];

            sync.cmd_master_enabled = root["sync"]["cmd_master_enabled"];
            sync.cmd_slave_enabled = root["sync"]["cmd_slave_enabled"];
            sync.cmd_slave_topic = root["sync"]["cmd_slave_topic"].asString();

            // events
            events.colorEventIntervalMs = root["events"]["colorEventIntervalMs"];

			//TODO check if we can actually load the config
			configversion = root["general"]["config_version"].asString();
			if (print) {
				root.prettyPrintTo(Serial);
			}

			delete[] jsonString;
		}

	}

	void save(bool print = false) {
		DynamicJsonBuffer jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();

		JsonObject& net = root.createNestedObject("network");
		JsonObject& con = net.createNestedObject("connection");
		con["dhcp"] = network.connection.dhcp;
		con["ip"] = network.connection.ip.toString();
		con["netmask"] = network.connection.netmask.toString();
		con["gateway"] = network.connection.gateway.toString();
		con["mdnhostname"] = network.connection.mdnshostname.c_str();

		JsonObject& jap = net.createNestedObject("ap");
		jap["secured"] = network.ap.secured;
		jap["ssid"] = network.ap.ssid.c_str();
		jap["password"] = network.ap.password.c_str();

		JsonObject& jmqtt = net.createNestedObject("mqtt");
		jmqtt["enabled"] = network.mqtt.enabled;
		jmqtt["server"] = network.mqtt.server.c_str();
		jmqtt["port"] = network.mqtt.port;
		jmqtt["username"] = network.mqtt.username.c_str();
		jmqtt["password"] = network.mqtt.password.c_str();
        jmqtt["topic_base"] = network.mqtt.topic_base.c_str();

		JsonObject& c = root.createNestedObject("color");
		c["outputmode"] = color.outputmode;

		JsonObject& h = c.createNestedObject("hsv");
		h["model"] = color.hsv.model;
		h["red"] = color.hsv.red;
		h["yellow"] = color.hsv.yellow;
		h["green"] = color.hsv.green;
		h["cyan"] = color.hsv.cyan;
		h["blue"] = color.hsv.blue;
		h["magenta"] = color.hsv.magenta;

		JsonObject& b = c.createNestedObject("brightness");
		b["red"] = color.brightness.red;
		b["green"] = color.brightness.green;
		b["blue"] = color.brightness.blue;
		b["ww"] = color.brightness.ww;
		b["cw"] = color.brightness.cw;

		JsonObject& t = c.createNestedObject("colortemp");
		t["ww"] = color.colortemp.ww;
		t["cw"] = color.colortemp.cw;

		JsonObject& s = jsonBuffer.createObject();
		root["sync"] = s;
		s["clock_master_enabled"] = sync.clock_master_enabled;
		s["clock_master_interval"] = sync.clock_master_interval;
        s["clock_slave_enabled"] = sync.clock_slave_enabled;
        s["clock_slave_topic"] = sync.clock_slave_topic.c_str();

        s["cmd_master_enabled"] = sync.cmd_master_enabled;
        s["cmd_slave_enabled"] = sync.cmd_slave_enabled;
        s["cmd_slave_topic"] = sync.cmd_slave_topic.c_str();

        JsonObject& e = jsonBuffer.createObject();
        root["events"] = e;
        e["colorEventIntervalMs"] = events.colorEventIntervalMs;

		JsonObject& g = jsonBuffer.createObject();
		root["general"] = g;
		g["api_secured"] = general.api_secured;
		g["api_password"] = general.api_password;
		g["otaurl"] = general.otaurl;
		g["config_version"] = CFG_VERSION;
        g["device_name"] = general.device_name.c_str();

		String rootString;
		if (print) {
			root.prettyPrintTo(Serial);
		}
		root.printTo(rootString);
		fileSetContent(APP_SETTINGS_FILE, rootString);
	}

	bool exist() {
		return fileExist(APP_SETTINGS_FILE);
	}

	void reset() {
		if (exist()) {
			fileDelete(APP_SETTINGS_FILE);
		}
	}
};
