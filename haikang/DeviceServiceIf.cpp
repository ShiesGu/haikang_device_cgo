#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include "HCNetSDK.h"
#include "DeviceServiceIf.h"
using namespace std;

// SDK异常回调
void CALLBACK temp_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType)
    {
    case EXCEPTION_RECONNECT:
        printf("pyd----------reconnect--------%ld\n", time(NULL));
        break;
    default:
        break;
    }
};

DeviceServiceIf::~DeviceServiceIf() {
    // free((Scheme *)scheme)
}
// debug
DeviceServiceIf::DeviceServiceIf() {
    //Scheme *scheme = (Scheme *)malloc(sizeof(Scheme));
    //memset(scheme, 0, sizeof(Scheme));
    scheme = NULL;
}

DeviceServiceIf::DeviceServiceIf(Scheme *scheme) {
    this->scheme = scheme;
    // ping(NULL, temp_ExceptionCallBack);
}

// 初始化设备公共参数
Scheme DeviceServiceIf::init(std::string addr, std::string user, std::string pass, int port, int channel) {
    Scheme temp = {0};
    temp.channel = channel;
    temp.port = port;
    temp.address = this->address;
    temp.username = this->username;
    temp.password = this->password;
    strcpy(temp.address, addr.c_str());
    strcpy(temp.username, user.c_str());
    strcpy(temp.password, pass.c_str());
    return temp;
}

// SDK参数设置
int DeviceServiceIf::ping(void *in, ExceptionCallBack g_ExceptionCallBack) {
    NET_DVR_Init();
    int iRet;

    HealthParam param = *((HealthParam *)in);
    NET_DVR_SetConnectTime(param.connectTime, 3);
    NET_DVR_SetRecvTimeOut(param.recvTimeOut);
    NET_DVR_SetReconnect(param.reconnect, true);
    NET_DVR_SetLogToFile(param.logLevel, param.logToFile);

    // 设置抓图模式JPEG/BMP
    NET_DVR_SetCapturePictureMode(JPEG_MODE);
    iRet = NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);
    if (!iRet)
    {
        printf("pyd1---NET_DVR_SetExceptionCallBack_V30 error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 同步登录
int DeviceServiceIf::login(BOOL useAysnc, void *dto) {
    NET_DVR_Init();
    long lUserID;
    //login
    NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
    NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};
    struLoginInfo.bUseAsynLogin = useAysnc;

    struLoginInfo.wPort = scheme->port;
    memcpy(struLoginInfo.sDeviceAddress, scheme->address, NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(struLoginInfo.sUserName, scheme->username, NAME_LEN);
    memcpy(struLoginInfo.sPassword, scheme->password, NAME_LEN);

    lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
    if (lUserID < 0)
    {
        printf("pyd1---Login error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    LoginDeviceDto *response = (LoginDeviceDto *)dto;
    memset(response, 0, sizeof(LoginDeviceDto));
    response->lUserID = lUserID;
    response->device.struDeviceV30.byStartChan = struDeviceInfoV40.struDeviceV30.byStartChan;

    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 注销登录
int DeviceServiceIf::logout(LONG lUserID) {
    NET_DVR_Init();

    NET_DVR_Logout(lUserID);
    NET_DVR_Cleanup();
    return HPR_OK;
}

// 本地多网卡IP获取
int DeviceServiceIf::localIp(void *dto) {
    NET_DVR_Init();
    int iRet;
    char strIps[16][16];

    LocalIpDto *response = (LocalIpDto *)dto;
    memset(response, 0, sizeof(LocalIpDto));
    iRet = NET_DVR_GetLocalIP(strIps, &response->pValidNum, &response->pEnableBind);
    if (!iRet)
    {
        printf("pyd1---NET_DVR_GetLocalIP error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }
    for (int i = 0; i < 16; i++) {
        memcpy(response->strIp[i], strIps[i], strlen(strIps[i]));
        //printf("%s\n", response->strIp[i]);
    }

    //printf("%d\n", response->pValidNum);
    //printf("%d\n", response->pEnableBind);
    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 自动发现设备IP和端口
int DeviceServiceIf::iPByResolveSvr(void *in, void *dto) {
    NET_DVR_Init();
    int iRet;
    char sGetIp[16];
    if ((IPByResolveSvrParam *)in == NULL)
    {
        printf("pyd1---IPByResolveSvrParam error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    IPByResolveSvrParam param = *((IPByResolveSvrParam *)in);
    param.wDVRNameLen = strlen((char *)param.sDVRName);
    param.wDVRSerialLen = strlen((char *)param.sDVRSerialNumber);

    IPByResolveSvrDto *response = (IPByResolveSvrDto *)dto;
    memset(response, 0, sizeof(IPByResolveSvrDto));
    iRet = NET_DVR_GetDVRIPByResolveSvr_EX(param.sServerIP, param.wServerPort, param.sDVRName, param.wDVRNameLen, \
            param.sDVRSerialNumber, param.wDVRSerialLen, sGetIp, &response->dwPort);
    if (!iRet)
    {
        printf("pyd1---NET_DVR_GetDVRIPByResolveSvr_EX error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    memcpy(response->sGetIP, sGetIp, strlen(sGetIp));
    //printf("%s\n", response->sGetIP);
    //printf("%d\n", response->dwPort);
    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 人脸侦测报警
int DeviceServiceIf::setupAlarmChan(LONG lUserID, LONG *returnHandle, MessageCallback callback) {
    NET_DVR_Init();

    //设置报警回调函数
    NET_DVR_SetDVRMessageCallBack_V31(callback, NULL);

    //启用布防
    LONG lHandle;
    NET_DVR_SETUPALARM_PARAM  struAlarmParam = {0};
    struAlarmParam.dwSize = sizeof(struAlarmParam);
    struAlarmParam.byFaceAlarmDetection = 1; //人脸侦测报警，设备支持人脸侦测功能的前提下，上传COMM_ALARM_FACE_DETECTION类型报警信息
    // 其他报警布防参数不需要设置，不支持

    lHandle = NET_DVR_SetupAlarmChan_V41(lUserID, &struAlarmParam);
    if (lHandle < 0)
    {
        printf("NET_DVR_SetupAlarmChan_V41 error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    *returnHandle = lHandle;
    return HPR_OK;
}

// 关闭人脸侦测报警
int DeviceServiceIf::closeAlarmChan(LONG lHandle) {
    NET_DVR_Init();

    //撤销布防上传通道
    if (!NET_DVR_CloseAlarmChan_V30(lHandle))
    {
        printf("NET_DVR_CloseAlarmChan_V30 error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    return HPR_OK;
}

// errno-23-设备不支持 系统参数配置
int DeviceServiceIf::iPParaCfg(LONG lUserID, LONG lChannel, void *dto) {
    NET_DVR_Init();
    int iRet;
    DWORD uiReturnLen = 0;

    NET_DVR_IPPARACFG_V31 struParams = {0};
    struParams.dwSize = sizeof(struParams);
    iRet = NET_DVR_GetDVRConfig(lUserID,  NET_DVR_GET_IPPARACFG_V31, lChannel, \
        &struParams, sizeof(NET_DVR_IPPARACFG_V31), &uiReturnLen);
    if (!iRet)
    {
        printf("pyd---NET_DVR_GetDVRConfig NET_DVR_GET_IPPARACFG_V31 error.%d\n",  NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    IPParaCfgDto *response = (IPParaCfgDto *)dto;
    memset(response, 0, sizeof(IPParaCfgDto));
    response->uiReturnLen = uiReturnLen;

    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 实时预览
int DeviceServiceIf::realPlay(void *in, StdDataCallBack fStdDataCallBack) {
    //---------------------------------------
    // 初始化
    NET_DVR_Init();
    if ((RealPlayParam *)in == NULL)
    {
        printf("pyd1---RealPlayParam error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }
    //---------------------------------------
    // 注册设备
    RealPlayParam param = *((RealPlayParam *)in);
    LONG lUserID;
    lUserID = param.lUserID;
    //---------------------------------------
    //启动预览并设置回调数据流
    LONG lRealPlayHandle;

    NET_DVR_PREVIEWINFO struPlayInfo = {0};
    struPlayInfo.lChannel     = param.lChannel;       //预览通道号
    struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
    struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP
    struPlayInfo.bBlocked     = 1;       //0- 非阻塞取流，1- 阻塞取流
    struPlayInfo.bPassbackRecord  = 1;   //0-不启用录像回传，1-启用录像回传

    lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
    if (lRealPlayHandle < 0)
    {
        printf("NET_DVR_RealPlay_V40 error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    //printf("[GetStream]---RealPlay %d:%d success, \n", lRealPlayHandle, NET_DVR_GetLastError());
    //int iRet = NET_DVR_SaveRealData(lRealPlayHandle, "./record/realplay.dat");
    NET_DVR_SetStandardDataCallBackEx(lRealPlayHandle, fStdDataCallBack, 0);
    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 关闭预览
int DeviceServiceIf::realStopPlay(LONG lRealPlayHandle) {
    NET_DVR_Init();
    //---------------------------------------
    //关闭预览
    NET_DVR_StopRealPlay(lRealPlayHandle);
    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 预览抓图
int DeviceServiceIf::realCapPicture(LONG lRealPlayHandle, void *dto) {
    //---------------------------------------
    // 初始化
    NET_DVR_Init();
    int iRet;
    char sBuf[204800];

    RealCapPictureDto *response = (RealCapPictureDto *)dto;
    memset(response, 0, sizeof(RealCapPictureDto));
    iRet = NET_DVR_CapturePictureBlock_New(lRealPlayHandle, sBuf, 0, &response->lpSizeReturned);
    if (!iRet)
    {
        printf("pyd1---NET_DVR_CapturePictureBlock_New error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    memcpy(response->pPicBuf, sBuf, response->lpSizeReturned);
    //NET_DVR_Cleanup();
    return HPR_OK;
}

// 设备抓图
int DeviceServiceIf::capPicture(LONG lUserID, LONG lChannel, void *dto) {
    NET_DVR_Init();
    int iRet;
    char sBuf[204800];

    NET_DVR_JPEGPARA strPicPara = {0};
    strPicPara.wPicQuality = 2;
    strPicPara.wPicSize = 0;

    CapPictureDto *response = (CapPictureDto *)dto;
    memset(response, 0, sizeof(CapPictureDto));
    iRet = NET_DVR_CaptureJPEGPicture_NEW(lUserID, lChannel, &strPicPara, \
        sBuf, sizeof(sBuf), &response->lpSizeReturned);
    if (!iRet)
    {
        printf("pyd1---NET_DVR_CaptureJPEGPicture error, %d\n", NET_DVR_GetLastError());
        return HPR_ERROR;
    }

    memcpy(response->sJpegPicBuffer, sBuf, response->lpSizeReturned);
    response->wPicSize = strPicPara.wPicSize;
    response->wPicQuality = strPicPara.wPicQuality;
    //NET_DVR_Cleanup();
    return HPR_OK;
}
