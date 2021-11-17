#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

#include "Types.h"
#include "_obj/_cgo_export.h"

#ifdef __cplusplus
extern "C" {
#endif

// The gateway function
void catchErrorCallback_cgo(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
void realDataCallBack_cgo(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);
BOOL alarmMsgCallback_cgo(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser);

// noAuth
void checkHealth(HealthParam *param, ExceptionCallBack fn);
void login(Scheme *scheme, BOOL useAsync, LoginDeviceDto *dto);
void logout(LONG lUserID);
void localIp(LocalIpDto *dto);
void iPByResolveSvr(IPByResolveSvrParam *in, IPByResolveSvrDto *dto);

// auth
void setupAlarmChan(LONG lUserID, LONG *returnHandle, MessageCallback callback);
void closeAlarmChan(LONG lHandle);
void iPParaCfg(LONG lUserID, LONG lChannel, IPParaCfgDto *dto);
void realPlay(RealPlayParam *in, StdDataCallBack fn);
void realStopPlay(LONG lRealPlayHandle);
void realCapPicture(LONG lRealPlayHandle, RealCapPictureDto *dto);
void capPicture(LONG lUserID, LONG lChannel, CapPictureDto *dto);

#ifdef __cplusplus
}
#endif

#endif