#ifndef NefryDataStore_h
#define NefryDataStore_h

#include "./inc/Preferences/src/Preferences.h"
#include "Nefry.h"
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

class NefryDataStore_lib
{
public:
	void begin();
	//void clear();
		
	bool
		setModuleID(String id),
		setModulePass(String pass),
		setModuleClass(String className),
		setUser(String user),
		setUserPass(String pass),
		setStorageValue(long value, int pointer),
		setStorageStr(String str, int pointer),
		setConnectSSID(String str, int pointer),
		setConnectPass(String str, int pointer),
		setBootSelector(int mode),
		setWifiBoot(int mode),
		setWebServerMode(int mode);
		
	String
		getModuleID(),
		getModulePass(),
		getModuleClass(),
		getUser(),
		getUserPass(),
		getStorageStr(int pointer),
		getConnectSSID(int pointer),
		getConnectPass(int pointer);

	int
		getBootSelector(),
		getWebServerMode(),
		getWifiBoot();

	long
		getStorageValue(int pointer);
private:
	bool _readCacheFlg[31];
	String _readCacheStr[20];
	long _readCacheLong[13];
	Preferences nefryDataStorePreferences;
};
extern NefryDataStore_lib NefryDataStore;
#endif
