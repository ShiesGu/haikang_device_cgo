#ifndef DEVICESERVICEIF_H
#define DEVICESERVICEIF_H

#define  HPR_OK 0
#define  HPR_ERROR -1

#if (defined(_WIN32)) //windows
#include <windows.h>
#endif

#include "Types.h"
class DeviceServiceIf
{
protected:
    int port;
    int channel;
    char address[129];
    char username[32];
    char password[32];
    Scheme *scheme;

public:
    ~DeviceServiceIf();
    DeviceServiceIf();
    DeviceServiceIf(Scheme *scheme);
    Scheme init(std::string addr, std::string user, std::string pass, int port, int channel);

    // noAuth
    int ping(void *in, ExceptionCallBack g_ExceptionCallBack);
    int login(BOOL useAsync, void *dto);
    int logout(LONG lUserID);
    int localIp(void *dto);
    int iPByResolveSvr(void *in, void *dto);

    // auth
    int setupAlarmChan(LONG lUserID, LONG *returnHandle, MessageCallback callback);
    int closeAlarmChan(LONG lHandle);
    int iPParaCfg(LONG lUserID, LONG lChannel, void *dto);
    int realPlay(void *in, StdDataCallBack fStdDataCallBack);
    int realStopPlay(LONG lRealPlayHandle);
    int realCapPicture(LONG lRealPlayHandle, void *dto);
    int capPicture(LONG lUserID, LONG lChannel, void *dto);
};

#endif
