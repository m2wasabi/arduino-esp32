﻿/**
Nefry lib

Copyright (c) 2015 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

/*
ModuleID	: Nefryモジュール名 & Nefryが発信するSSID
ModulePass	: Nefryが発信するWiFiパスワード
ModuleClass	: Nefryのモジュールが属するクラス
user		: Nefryでログイン機能を使うときに使用するユーザ名
userPass	: Nefryでログイン機能を使うときに使用するパスワード
StorageValue: Nefryの環境変数を保存するときに使用する。(0-7)の範囲で保存する。末端に数字が追加される long型
StorageStr	: Nefryの環境変数を保存するときに使用する。(0-7)の範囲で保存する。末端に数字が追加される String型
ConnectSSID	: Nefryが接続するWiFiのSSIDを保存するときに使用する。5つまで保存。末端に数字が追加される String型
ConnectPass	: Nefryが接続するWiFiのパスワードを保存するときに使用する。5つまで保存。末端に数字が追加される String型
ConnectWiFi : 接続するWiFiを有効or無効化する
bootSelector: BootModeを決める
--------------
BootMode
1 : WriteMode切替をする
*/

#define LIBVERSION ("0.8.0")
#include "Nefry.h"

Adafruit_NeoPixel _NefryLED[40];

//main 

void Nefry_lib::nefry_init() {
	beginLed(1, 16, NEO_GRB);
	enableSW();
	delay(50);
	_bootMode = 0;
	setLed(0x00, 0x0f, 0x00);
	Serial.begin(115200);
	Serial.println(F("\n\nStartup"));
	Serial.println(F("\n\nNefry Startup"));
	NefryDataStore.begin();
	setLed(0x00, 0x2f, 0x00);
	/* Display設定 */
	setLed(0x00, 0x4f, 0x00);
	/* Module状況表示 */
	setLed(0x00, 0x6f, 0x00);

	setLed(0x00, 0x8f, 0x00);
	Serial.println(F("WiFi Startup"));
	if (NefryDataStore.getModuleID().equals(""))
		NefryDataStore.setModuleID(getDefaultModuleId());
	NefryWiFi.begin();
	setLed(0x00, 0xaf, 0x00);
	/* IPaddress display表示 */
	setLed(0x00, 0xcf, 0x00);
	setLed(0x00, 0xef, 0x00);
	if (NefryDataStore.getBootSelector() == 1 || readSW()) {
		setLed(0x0f, 0xff, 0xff);
		NefryDataStore.setBootSelector(0);
		_bootMode = 2;
		Serial.println("Write Mode");
	} else {
		_bootMode = 1;
	}
	disableSW();
	NefryConfig.begin();
	Serial.println(F("\nServer started"));
	setLed(0x00, 0xff, 0xff);
	
}

void Nefry_lib::nefry_loop() {
	NefryWiFi.run();
}

/* ModuleID */
bool Nefry_lib::setModuleID(String id)
{
	return NefryDataStore.setModuleID(id);
}
String Nefry_lib::getModuleID()
{
	return NefryDataStore.getModuleID();
}

/* ModuleClass */
bool Nefry_lib::setModuleClass(String className)
{
	return NefryDataStore.setModuleClass(className);
}
String Nefry_lib::getModuleClass()
{
	return NefryDataStore.getModuleClass();
}

/* ModulePass */
bool Nefry_lib::setModulePass(String pass)
{
	return NefryDataStore.setModulePass(pass);
}

/* user */
bool Nefry_lib::setUser(String user)
{
	return NefryDataStore.setUser(user);
}
String Nefry_lib::getUser()
{
	return NefryDataStore.getUser();
}

/* userPass */
bool Nefry_lib::setUserPass(String pass)
{
	return NefryDataStore.setUserPass(pass);
}

/* Storage */
/* Value */
bool Nefry_lib::setStoreValue(long value, int pointer)
{
	return NefryDataStore.setStorageValue(value, pointer);
}
long Nefry_lib::getStoreValue(int pointer)
{
	return NefryDataStore.getStorageValue(pointer);
}

/* String */
bool Nefry_lib::setStoreStr(String str, int pointer)
{
	return NefryDataStore.setStorageStr(str, pointer);
}
String Nefry_lib::getStoreStr(int pointer)
{
	return NefryDataStore.getStorageStr(pointer);
}

/* programName */
void Nefry_lib::setProgramName(const char *pn) {
	program = pn;
}
String Nefry_lib::getProgramName() {
	return program;
}

/* NefryDelay */
void Nefry_lib::ndelay(unsigned long ms) {
	unsigned long beforetime = millis();
	while (millis() - beforetime < ms) {
		if (millis() < beforetime)break;
		nefry_loop();
		delay(1);
		yield();
	}
}

/* Module Reset */
void Nefry_lib::reset() {
	Serial.println(F("Nefry Reset"));
	ESP.restart();
	delay(500);
}

void Nefry_lib::deleteWiFi(int id)
{
	NefryWiFi.deleteWifi(id);
}

void Nefry_lib::addWiFi(String ssid, String pass)
{
	NefryWiFi.addWifi(ssid, pass);
}
void Nefry_lib::saveWiFi() {
	NefryWiFi.saveWifi();
}

/* DeepSleep */
void Nefry_lib::sleep(const int sec) {
	ESP.deepSleep(sec * 1000 * 1000);
	delay(1000);
}

/* Library Version */
String Nefry_lib::getVersion() {
	return LIBVERSION;
}

/* IPaddress to String */
String Nefry_lib::getAddressStr(IPAddress ip)
{
	if (ip.toString().equals("0.0.0.0"))return "192.168.4.1";
	else return ip.toString();
}

/* SW 有効化します */
void Nefry_lib::enableSW() {
	pinMode(4, INPUT_PULLUP);
	_swEnableFlg = true;
}

/* SW 無効化します */
void Nefry_lib::disableSW() {
	_swEnableFlg = false;
}

/* SW の状態を取得します */
bool Nefry_lib::readSW() {
	if (_swEnableFlg == true) {
		if (_swflg == true) {
			_swflg = false;
			return true;
		}
		return false;
	}
	else {
		return !digitalRead(4);
	}
	return false;
}

/* SWを押されたときに割り込まれます */
void Nefry_lib::pollingSW() {
	if (_swEnableFlg == true && _swflg != true) {
		if(digitalRead(4) == LOW){
			if (_bootMode == 0) {
				Nefry.setLed(0xff, 0x2f, 0x00);
			}
			_swPushingflg = true;
		}
		if (digitalRead(4) == HIGH && _swPushingflg == true) {
			_swPushingflg = false;
			_swflg = true;	
		}

	}
}
bool Nefry_lib::getPollingSW()
{
	return _swEnableFlg;
}

//LED
void Nefry_lib::beginLed(const int num, const int pin, uint8_t t = NEO_GRB) {
	_NefryLED[pin] = Adafruit_NeoPixel(num, pin, t);
	_NefryLED[pin].begin();
}
void Nefry_lib::setLed(const int r, const int g, const int b, const char w, const int pin, const int num) {
	_NefryLED[pin].setBrightness(w);
	_NefryLED[pin].setPixelColor(num, r, g, b);
	_NefryLED[pin].show();
}
void Nefry_lib::setLed(String _colorStr, const char w, const int pin, const int num) {
	int _color[3];
	for (int i = 0; i < 3; i++) {
		_color[i] = 0;
		_color[i] += 16 * hextonum(_colorStr[i * 2]);
		_color[i] += hextonum(_colorStr[i * 2 + 1]);
	}
	setLed(_color[0], _color[1], _color[2], w, pin, num);
}
int Nefry_lib::hextonum(char c)
{
	char e;
	int n = 0;
	if ('0' <= c && c <= '9') {
		n = c - '0';
	}
	else if ('a' <= (e = tolower(c)) && c <= 'f') {
		n = e - 'a' + 10;
	}
	return n;
}

bool Nefry_lib::getWriteMode() {
	if (_bootMode == 2)return true;
	return false;
}
long Nefry_lib::getBootMode()
{
	return _bootMode;
}
void Nefry_lib::setStoreTitle(const char set[15], const int num)
{
	NefryConfig.setStoreTitle(set, num);
	return ;
}

String Nefry_lib::getDefaultModuleId() {
	uint8_t macAddr[6];
	char str[15];
	char* moduleName;
	WiFi.macAddress(macAddr);
	switch (boardId)
	{
	case 0:case 1:
		moduleName = "Nefry";
		break;
	}
	sprintf(str, "%s-%02x%02x", moduleName, macAddr[6 - 2], macAddr[6 - 1]);
	Serial.println(str);
	return str;
}

void Nefry_lib::println(String text) { NefryConsole.println(text); }
void Nefry_lib::println(float text) { NefryConsole.println(text); }
void Nefry_lib::println(double text) { NefryConsole.println(text); }
void Nefry_lib::println(char text) { NefryConsole.println(text); }
void Nefry_lib::println(int text) { NefryConsole.println(text); }
void Nefry_lib::println(long text) { NefryConsole.println(text); }
void Nefry_lib::println(unsigned char text) { NefryConsole.println(text); }
void Nefry_lib::println(unsigned int text) { NefryConsole.println(text); }
void Nefry_lib::println(unsigned long text) { NefryConsole.println(text); }
void Nefry_lib::print(float text) { NefryConsole.print(text); }
void Nefry_lib::print(double text) { NefryConsole.print(text); }
void Nefry_lib::print(char text) { NefryConsole.print(text); }
void Nefry_lib::print(int text) { NefryConsole.print(text); }
void Nefry_lib::print(long text) { NefryConsole.print(text); }
void Nefry_lib::print(unsigned char text) { NefryConsole.print(text); }
void Nefry_lib::print(unsigned int text) { NefryConsole.print(text); }
void Nefry_lib::print(unsigned long text) { NefryConsole.print(text); }
void Nefry_lib::print(String text) { NefryConsole.print(text); }

void Nefry_lib::clearConsole() { NefryConsole.clearConsole(); }
int Nefry_lib::available(){ return NefryConsole.available(); }
String Nefry_lib::read(){ return NefryConsole.read(); }
Nefry_lib Nefry;