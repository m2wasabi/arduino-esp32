/**
Nefry lib

Copyright (c) 2015 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "NefryDataStore.h"


void NefryDataStore_lib::begin()
{
	nefryDataStorePreferences = Preferences();
	nefryDataStorePreferences.begin("nefryStore");
	for (int i = 0; i < 30; i++)_readCacheFlg[i] = false;
}
/*
void NefryDataStore_lib::clear()
{
	nefryDataStorePreferences.clear();
}
*/

bool NefryDataStore_lib::setModuleID(String id)
{
	if (id.equals("")) {
		_readCacheStr[10] = Nefry.getDefaultModuleId();
	}
	else {
		_readCacheStr[10] = id;
	}
	_readCacheFlg[10] = true;
	return nefryDataStorePreferences.putString("ModuleID", _readCacheStr[10]);
}

bool NefryDataStore_lib::setBootSelector(int mode)
{
	_readCacheLong[10] = mode;
	_readCacheFlg[30] = true;
	return nefryDataStorePreferences.putInt("BootSelector", mode);
}

bool NefryDataStore_lib::setConnectPass(String str, int pointer)
{
	String key;
	key = "ConnectPass";
	key += pointer;
	return nefryDataStorePreferences.putString(key.c_str(), str);
}

bool NefryDataStore_lib::setConnectSSID(String str, int pointer)
{
	String key;
	key = "ConnectSSID";
	key += pointer;
	return nefryDataStorePreferences.putString(key.c_str(), str);
}

bool NefryDataStore_lib::setStorageStr(String str, int pointer)
{
	if (pointer >= 10)return false;
	Serial.println(str);
	String key;
	key = "StorageStr";
	key += pointer;
	_readCacheStr[pointer] = str;
	_readCacheFlg[pointer] = true;
	return nefryDataStorePreferences.putString(key.c_str(), str);
}

bool NefryDataStore_lib::setStorageValue(long value, int pointer)
{
	if (pointer >= 10)return false;
	Serial.println(value);
	String key;
	key = "StorageValue";
	key += pointer;
	_readCacheLong[pointer] = value;
	_readCacheFlg[20+pointer] = true;
	return nefryDataStorePreferences.putLong(key.c_str(), value);
}

bool NefryDataStore_lib::setUserPass(String pass)
{
	_readCacheStr[11] = pass;
	_readCacheFlg[11] = true;
	return nefryDataStorePreferences.putString("UserPass", pass);
}

bool NefryDataStore_lib::setUser(String user)
{
	_readCacheStr[12] = user;
	_readCacheFlg[12] = true;
	return nefryDataStorePreferences.putString("User", user);
}

bool NefryDataStore_lib::setModuleClass(String className)
{
	_readCacheStr[13] = className;
	_readCacheFlg[13] = true;
	return nefryDataStorePreferences.putString("ModuleClass", className);
}

bool NefryDataStore_lib::setModulePass(String pass)
{
	_readCacheStr[14] = pass;
	_readCacheFlg[14] = true;
	return nefryDataStorePreferences.putString("ModulePass", pass);
}

bool NefryDataStore_lib::setWifiBoot(int mode)
{
	_readCacheLong[11] = mode;
	_readCacheFlg[15] = true;
	return nefryDataStorePreferences.putInt("WifiBoot", mode);
}

bool NefryDataStore_lib::setWebServerMode(int mode)
{
	_readCacheLong[12] = mode;
	_readCacheFlg[16] = true;
	return nefryDataStorePreferences.putInt("WebServerMode", mode);
}

/* 取得 */

String NefryDataStore_lib::getModuleID()
{
	if (_readCacheFlg[10] == false) {
		_readCacheStr[10] = nefryDataStorePreferences.getString("ModuleID","");
		_readCacheFlg[10] = true;
	}
	return _readCacheStr[10];
}

String NefryDataStore_lib::getConnectSSID(int pointer)
{
	String key;
	key = "ConnectSSID";
	key += pointer;
	return nefryDataStorePreferences.getString(key.c_str(), "");
}

String NefryDataStore_lib::getConnectPass(int pointer)
{
	String key;
	key = "ConnectPass";
	key += pointer;
	return nefryDataStorePreferences.getString(key.c_str(), "");
}

int NefryDataStore_lib::getBootSelector()
{
	if (_readCacheFlg[30] == false) {
		_readCacheLong[10] = nefryDataStorePreferences.getInt("BootSelector", 0);
		_readCacheFlg[30] = true;
	}
	return _readCacheLong[10];
}

long NefryDataStore_lib::getStorageValue(int pointer)
{
	if (pointer >= 10)return 0;
	if (_readCacheFlg[20 + pointer] == false) {
		_readCacheFlg[20 + pointer] = true;
		String key;
		key = "StorageValue";
		key += pointer;
		_readCacheLong[pointer] = nefryDataStorePreferences.getLong(key.c_str(), 0);
	}
	return _readCacheLong[pointer];
}

String NefryDataStore_lib::getStorageStr(int pointer)
{
	if (pointer >= 10)return "";
	if (_readCacheFlg[pointer] == false) {
		_readCacheFlg[pointer] = true;
		String key;
		key = "StorageStr";
		key += pointer;
		_readCacheStr[pointer]= nefryDataStorePreferences.getString(key.c_str(), "");
	}
	return _readCacheStr[pointer];
}

String NefryDataStore_lib::getUserPass()
{
	if (_readCacheFlg[11] == false) {
		_readCacheStr[11] = nefryDataStorePreferences.getString("UserPass", "");
	}
	return _readCacheStr[11];
}

String NefryDataStore_lib::getUser()
{
	if (_readCacheFlg[12] == false) {
		_readCacheStr[12] = nefryDataStorePreferences.getString("User", "");
	}
	return _readCacheStr[12];
}

String NefryDataStore_lib::getModuleClass()
{
	if (_readCacheFlg[13] == false) {
		_readCacheStr[13] = nefryDataStorePreferences.getString("ModuleClass", "");
	}
	return _readCacheStr[13];
}

String NefryDataStore_lib::getModulePass()
{
	if (_readCacheFlg[14] == false) {
		_readCacheStr[14] = nefryDataStorePreferences.getString("ModulePass", "");
	}
	
	return _readCacheStr[14];
}

int NefryDataStore_lib::getWifiBoot()
{
	if (_readCacheFlg[15] == false) {
		_readCacheLong[11] = nefryDataStorePreferences.getInt("WifiBoot", 1);
		_readCacheFlg[15] = true;
	}
	return _readCacheLong[11];
}

int NefryDataStore_lib::getWebServerMode()
{
	if (_readCacheFlg[16] == false) {
		_readCacheLong[12] = nefryDataStorePreferences.getInt("WebServerMode", 1);
		_readCacheFlg[16] = true;
	}
	return _readCacheLong[12];
}

NefryDataStore_lib NefryDataStore;
