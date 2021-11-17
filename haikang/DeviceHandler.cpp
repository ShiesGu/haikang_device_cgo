#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "DeviceServiceIf.h"
#include "DeviceHandler.h"
using namespace std;

// The gateway function
void catchErrorCallback_cgo(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser) {
	GoCatchErrorCallback(dwType, lUserID, lHandle, pUser);
}
void realDataCallBack_cgo(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser) {
	GoRealDataCallBack(lRealHandle, dwDataType, pBuffer, dwBufSize, pUser);
}
BOOL alarmMsgCallback_cgo(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser) {
    GoAlarmMsgCallback(lCommand, pAlarmer, pAlarmInfo, dwBufLen, pUser);
    return true;
}

void setupAlarmChan(LONG lUserID, LONG *returnHandle, MessageCallback fn) {
    fn(1, NULL, NULL, 2, NULL);
    DeviceServiceIf *s = new DeviceServiceIf();
    s->setupAlarmChan(lUserID, returnHandle, fn);
    delete s;
    std::cout << "\n" << std::endl;
}

void closeAlarmChan(LONG lHandle) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->closeAlarmChan(lHandle);
    delete s;
    std::cout << "\n" << std::endl;
}

void checkHealth(HealthParam *param, ExceptionCallBack fn) {
    fn(1, 2, 3, NULL);
    DeviceServiceIf *s = new DeviceServiceIf();
    s->ping(param, fn);
    delete s;
    std::cout << "\n" << std::endl;
}

void login(Scheme *scheme, BOOL useAsync, LoginDeviceDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf(scheme);
    s->login(useAsync, dto);
    delete s;
    std::cout << "\n" << std::endl;
}

void logout(LONG lUserID) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->logout(lUserID);
    delete s;
    std::cout << "\n" << std::endl;
}

void localIp(LocalIpDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->localIp(dto);
    delete s;
    std::cout << "\n" << std::endl;
}

void iPByResolveSvr(IPByResolveSvrParam *in, IPByResolveSvrDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->iPByResolveSvr(in, dto);
    delete s;
    std::cout << "\n" << std::endl;
}

void iPParaCfg(LONG lUserID, LONG lChannel, IPParaCfgDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->iPParaCfg(lUserID, lChannel, dto);
    delete s;
    std::cout << "\n" << std::endl;
}

void realPlay(RealPlayParam *in, StdDataCallBack fn) {
    fn(4, 5, NULL, 6, NULL);
    DeviceServiceIf *s = new DeviceServiceIf();
    s->realPlay(in, fn);
    delete s;
    std::cout << "\n" << std::endl;
}

void realStopPlay(LONG lRealPlayHandle) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->realStopPlay(lRealPlayHandle);
    delete s;
    std::cout << "\n" << std::endl;
}

void realCapPicture(LONG lRealPlayHandle, RealCapPictureDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->realCapPicture(lRealPlayHandle, dto);
    delete s;
    std::cout << "\n" << std::endl;
}

void capPicture(LONG lUserID, LONG lChannel, CapPictureDto *dto) {
    DeviceServiceIf *s = new DeviceServiceIf();
    s->capPicture(lUserID, lChannel, dto);
    delete s;
    std::cout << "\n" << std::endl;
}

/*
int main()
{
    int serverPort = 8000;
    char serverIp[16] = "192.168.5.1";
    char DVRName[256] = "海康威视";
    char DVRNumber[256] = "321321";

    IPByResolveSvrParam param = {0};
    param.sServerIP = serverIp;
    param.wServerPort = serverPort;
    param.sDVRName = (BYTE *) DVRName;
    param.sDVRSerialNumber = (BYTE *)DVRNumber;

    RealPlayParam realParam = {0};
    realParam.lUserID = 0;
    realParam.lChannel = 0;

    Scheme scheme;
    DeviceServiceIf *s = new DeviceServiceIf();
    scheme = s->init("192.168.5.165", "admin", "zxw123456", 8000, 1);

    checkHealth(catchErrorCallback_cgo);
    LoginDeviceDto dto = {0};
    login(&scheme, false, &dto);
    printf("%d\n", dto.lUserID);
    printf("%d\n", dto.device.struDeviceV30.byStartChan);

    CapPictureDto capDto = {0};
    capPicture(dto.lUserID, dto.device.struDeviceV30.byStartChan, &capDto);
    printf("%d\n", capDto.lpSizeReturned);
    for (int i = 0; i < 204800; i++) {
        printf("%d", capDto.sJpegPicBuffer[i]);
    }
    logout(dto.lUserID);
    delete s;
    return 0;
}
*/
