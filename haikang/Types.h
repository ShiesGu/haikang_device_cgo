#ifndef TYPES_H
#define TYPES_H

#ifndef _HC_NET_SDK_H_
#define  CALLBACK
#define  BOOL  int

typedef  unsigned int       DWORD;
typedef  unsigned short     WORD;
typedef  unsigned short     USHORT;
typedef  short              SHORT;
typedef  int                LONG;
typedef  unsigned char      BYTE;
typedef  unsigned int       UINT;
typedef  void*              LPVOID;
typedef  void*              HANDLE;
typedef  unsigned int*      LPDWORD;
typedef  unsigned long long UINT64;
typedef  signed long long   INT64;

// 临时回调函数需要
typedef struct NET_DVR_ALARMER
{
    BYTE byUserIDValid;                 /* userid�Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE bySerialValid;                 /* ���к��Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE byVersionValid;                /* �汾���Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE byDeviceNameValid;             /* �豸�����Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE byMacAddrValid;                /* MAC��ַ�Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE byLinkPortValid;               /* login�˿��Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE byDeviceIPValid;               /* �豸IP�Ƿ���Ч 0-��Ч��1-��Ч */
    BYTE bySocketIPValid;               /* socket ip�Ƿ���Ч 0-��Ч��1-��Ч */
    LONG lUserID;                       /* NET_DVR_Login()����ֵ, ����ʱ��Ч */
    BYTE sSerialNumber[48];    /* ���к� */
    DWORD dwDeviceVersion;                /* �汾��Ϣ ��16λ��ʾ���汾����16λ��ʾ�ΰ汾*/
    char sDeviceName[32];            /* �豸���� */
    BYTE byMacAddr[6];        /* MAC��ַ */
    WORD wLinkPort;                     /* link port */
    char sDeviceIP[128];                /* IP��ַ */
    char sSocketIP[128];                /* ���������ϴ�ʱ��socket IP��ַ */
    BYTE byIpProtocol;                  /* IpЭ�� 0-IPV4, 1-IPV6 */
    BYTE byRes1[2];
    BYTE bJSONBroken;                   //JSON����������־��0����������1������
    WORD wSocketPort;
    BYTE byRes2[6];
}NET_DVR_ALARMER;
#endif

typedef BOOL(CALLBACK *MessageCallback)(LONG lCommand, NET_DVR_ALARMER *pAlarmer, char *pAlarmInfo, DWORD dwBufLen, void *pUser);
typedef void(CALLBACK *ExceptionCallBack)(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser);
typedef void(CALLBACK *StdDataCallBack)(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);

typedef struct Scheme
{
    DWORD port;
    DWORD channel;
    char *address;
    char *username;
    char *password;
}Scheme;

typedef struct LocalIpDto
{
    // char (*strIp)[16];
    char strIp[16][16];
    DWORD pValidNum;
    BOOL pEnableBind;
}LocalIpDto;

typedef struct IPByResolveSvrParam
{
    char *sServerIP;
    WORD wServerPort;
    BYTE *sDVRName;
    WORD wDVRNameLen;
    BYTE *sDVRSerialNumber;
    WORD wDVRSerialLen;
}IPByResolveSvrParam;

typedef struct IPByResolveSvrDto
{
    char *sGetIP;
    DWORD dwPort;
}IPByResolveSvrDto;

typedef struct IPParaCfgDto
{
    // NET_DVR_IPPARACFG_V31 struParams;
    DWORD uiReturnLen;
}IPParaCfgDto;

typedef struct RealPlayParam
{
    LONG lUserID;
    LONG lChannel;
    DWORD dwStreamType;
    DWORD dwLinkMode;
    DWORD bBlocked;
    DWORD bPassbackRecord;
}RealPlayParam;

typedef struct RealPlayInfo
{
	char szIP[16];
	LONG lUserID;
	DWORD lChannel;
}RealPlayInfo;

typedef struct DeviceInfoV30
{
    char *sSerialNumber;
    BYTE byDVRType;
    BYTE byChanNum;
    BYTE byStartChan;
}DeviceInfoV30;

typedef struct DeviceInfoV40
{
    DeviceInfoV30 struDeviceV30;
    BYTE byRetryLoginTime;
    BYTE byPasswordLevel;
}DeviceInfoV40;

typedef struct LoginDeviceDto
{
    LONG lUserID;
    DeviceInfoV40 device;
}LoginDeviceDto;

typedef struct RealCapPictureDto
{
    char  pPicBuf[204800];
    DWORD lpSizeReturned;
}RealCapPictureDto;

typedef struct CapPictureDto
{
    WORD  wPicSize;
    WORD  wPicQuality;
    char  sJpegPicBuffer[204800];
    DWORD lpSizeReturned;
}CapPictureDto;

typedef struct HealthParam {
	LONG connectTime;
	LONG recvTimeOut;
	LONG reconnect;
	char *logToFile;
	DWORD logLevel;
}HealthParam;

typedef struct VCARect
{
    float fX;               //�߽�����Ͻǵ��X������, 0.000~1
    float fY;               //�߽�����Ͻǵ��Y������, 0.000~1
    float fWidth;           //�߽��Ŀ��, 0.000~1
    float fHeight;          //�߽��ĸ߶�, 0.000~1
}VCARect;

typedef struct FaceDetection
{
    DWORD     dwSize;
    DWORD     dwRelativeTime;
    DWORD     dwAbsTime;
    DWORD     dwBackgroundPicLen;
    VCARect   struFacePic[30];
    BYTE*     pBackgroundPicpBuffer;
}FaceDetection;

typedef struct GoAlarmer
{
    LONG lUserID;
    BYTE sSerialNumber[48];
    DWORD dwDeviceVersion;
    char sDeviceName[128];
}Alarmer;

#endif
