package haikang

/*
#cgo CXXFLAGS: -I./
#cgo LDFLAGS: -L. -Wl,-rpath=./:./lib:./lib/HCNetSDKCom -lhcnetsdk -lstdc++
#cgo LDFLAGS: -L. -ldevice_handler -lstdc++
#include <stdio.h>
#include <stdlib.h>
#include "DeviceHandler.h"
*/
import "C"

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"io/ioutil"
	"mime/multipart"
	"net/http"
	"unsafe"

	s "./service"
)

const OssPath = "http://oss-path/upload/image"
var UserApi = NewUserApi(context.Background())
var TempResolveParam = &IPByResolveSvrParam{
	ServerIP:        "127.0.0.1",
	ServerPort:      8000,
	DVRName:         "海康威视",
	DVRSerialNumber: "123456",
}

type userApi struct {
	Service *s.Service
}

func NewUserApi(ctx context.Context) *userApi {
	a := &userApi{
		Service: s.NewService(ctx),
	}

	return a
}

func RegisterRouterNoAuth() {
	/*
	g.POST("/user/ping", ping)
	g.POST("/user/login", login)
	g.POST("/user/logout", logout)
	g.POST("/user/active", active)
	g.POST("/user/local/ip", localIp)
	g.POST("/user/resolve/server/ip", iPByResolveSvr)
	g.POST("/user/ip/config/param", iPParaCfg)
	g.POST("/user/real/play", realPlay)
	g.POST("/user/real/stop/play", realStopPlay)
	g.POST("/user/real/cap/picture", realCapPicture)
	g.POST("/user/cap/picture", capPicture)
	return
	*/
}


// @summary 心跳检测
// @tags    海康对接-心跳检测
// @produce json
// @Param param body PingParam true "心跳检测参数"
// @router  /haikang/user/ping [POST]
// @success 200 {object} Response "获取成功"
func ping() {
	param := LocalHealth(UserApi.Service.Health)
	C.checkHealth(&param, (C.ExceptionCallBack)(unsafe.Pointer(C.catchErrorCallback_cgo)))
	checkHealth := &PingParam{
		Welcome: "ok",
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 设备激活
// @tags    海康对接-设备激活
// @produce json
// @Param param body PingParam true "设备激活参数"
// @router  /haikang/user/active [POST]
// @success 200 {object} Response "获取成功"
func active() {
	fmt.Println(fmt.Printf("%v\n", nil))
	return
}

// @summary 设备登录
// @tags    海康对接-设备登录
// @produce json
// @Param param body PingParam true "设备登录参数"
// @router  /haikang/user/login [POST]
// @success 200 {object} Response "获取成功"
func login() {
	CDto := C.struct_LoginDeviceDto{}

	useAsync := 0
	scheme := GetCScheme(UserApi.Service.Scheme)
	C.login(scheme, C.int(useAsync), (*C.struct_LoginDeviceDto)(unsafe.Pointer(&CDto)))
	checkHealth := &PingParam{
		Welcome: C.int(CDto.lUserID),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 设备登出
// @tags    海康对接-设备登出
// @produce json
// @Param param body PingParam true "设备登出参数"
// @router  /haikang/user/logout [POST]
// @success 200 {object} Response "获取成功"
func logout() {
	lUserID := 0
	C.logout(C.int(lUserID))
	checkHealth := &PingParam{
		Welcome: C.int(lUserID),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 本地多网卡IP获取
// @tags    海康对接-本地多网卡IP获取
// @produce json
// @Param param body PingParam true "本地多网卡IP获取参数"
// @router  /haikang/user/local/ip [POST]
// @success 200 {object} Response "获取成功"
func localIp() {
	CDto := C.struct_LocalIpDto{}

	C.localIp((*C.struct_LocalIpDto)(unsafe.Pointer(&CDto)))
	checkHealth := &PingParam{
		Welcome: GetLocalIp(CDto.strIp),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 自动发现设备IP和端口
// @tags    海康对接-自动发现设备IP和端口
// @produce json
// @Param param body PingParam true "自动发现设备IP和端口"
// @router  /haikang/user/resolve/server/ip [POST]
// @success 200 {object} Response "获取成功"
func iPByResolveSvr() {
	CDto := C.struct_IPByResolveSvrDto{}
	CParam := GetIpResolveParam(TempResolveParam)

	C.iPByResolveSvr((*C.struct_IPByResolveSvrParam)(unsafe.Pointer(&CParam)), (*C.struct_IPByResolveSvrDto)(unsafe.Pointer(&CDto)))
	checkHealth := &PingParam{
		Welcome: C.GoString(CDto.sGetIP),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary errno-23-设备不支持 系统参数配置
// @tags    海康对接-系统参数配置
// @produce json
// @Param param body PingParam true "系统参数配置"
// @router  /haikang/user/ip/config/param [POST]
// @success 200 {object} Response "获取成功"
func iPParaCfg() {
	lUserID := 0
	lChannel := 1
	CDto := C.struct_IPParaCfgDto{}

	C.iPParaCfg(C.int(lUserID), C.int(lChannel), (*C.struct_IPParaCfgDto)(unsafe.Pointer(&CDto)))
	checkHealth := &PingParam{
		Welcome: C.int(CDto.uiReturnLen),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 实时预览
// @tags    海康对接-实时预览
// @produce json
// @Param param body PingParam true "实时预览参数"
// @router  /haikang/user/real/play [POST]
// @success 200 {object} Response "获取成功"
func realPlay() {
	CParam := C.struct_RealPlayParam{
		lUserID: C.int(0),
		lChannel: C.int(1),
	}
	C.realPlay((*C.struct_RealPlayParam)(unsafe.Pointer(&CParam)), (C.StdDataCallBack)(unsafe.Pointer(C.realDataCallBack_cgo)))
	checkHealth := &PingParam{
		Welcome: "ok",
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 停止预览
// @tags    海康对接-停止预览
// @produce json
// @Param param body PingParam true "停止预览参数"
// @router  /haikang/user/real/stop/play [POST]
// @success 200 {object} Response "获取成功"
func realStopPlay() {
	lRealPlayHandle := 0
	C.realStopPlay(C.int(lRealPlayHandle))
	checkHealth := &PingParam{
		Welcome: C.int(lRealPlayHandle),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 预览抓拍
// @tags    海康对接-预览抓拍
// @produce json
// @Param param body PingParam true "预览抓拍参数"
// @router  /haikang/user/real/cap/picture [POST]
// @success 200 {object} Response "获取成功"
func realCapPicture() {
	lRealPlayHandle := 1
	CDto := C.struct_RealCapPictureDto{}

	C.realCapPicture(C.int(lRealPlayHandle), (*C.struct_RealCapPictureDto)(unsafe.Pointer(&CDto)))
	length := int(C.uint(CDto.lpSizeReturned))
	buffer := GetStream(CDto.pPicBuf, length)
	//ioutil.WriteFile("./record/preview-ssss.jpeg", buffer, 0644)

	checkHealth := &PingParam{
		Welcome: string(buffer),
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// @summary 设备抓拍
// @tags    海康对接-设备抓拍
// @produce json
// @Param param body PingParam true "设备抓拍参数"
// @router  /haikang/user/cap/picture [POST]
// @success 200 {object} Response "获取成功"
func capPicture() {
	param := LocalHealth(UserApi.Service.Health)
	C.checkHealth(&param, (C.ExceptionCallBack)(unsafe.Pointer(C.catchErrorCallback_cgo)))

	lDto := C.struct_LoginDeviceDto{}
	scheme := GetCScheme(UserApi.Service.Scheme)
	C.login(scheme, C.int(0), (*C.struct_LoginDeviceDto)(unsafe.Pointer(&lDto)))
	lUserID := C.int(lDto.lUserID)
	lChannel := C.int(int(byte(lDto.device.struDeviceV30.byStartChan)))

	CDto := C.struct_CapPictureDto{}
	C.capPicture(lUserID, lChannel, (*C.struct_CapPictureDto)(unsafe.Pointer(&CDto)))
	length := int(C.uint(CDto.lpSizeReturned))
	buffer := GetStream(CDto.sJpegPicBuffer, length)

	result, _ := UploadImg(buffer, OssPath)
	C.logout(lUserID)
	checkHealth := &PingParam{
		Welcome: result,
	}
	fmt.Println(fmt.Printf("%v\n", checkHealth))
	return
}

// 转换CScheme唤醒海康摄像头
func GetCScheme(scheme *s.Scheme) *C.struct_Scheme {
	var CScheme C.struct_Scheme
	CScheme = C.struct_Scheme{
		channel:  C.uint(scheme.Channel),
		port:     C.uint(scheme.Port),
		address:  C.CString(scheme.Address),
		username: C.CString(scheme.Username),
		password: C.CString(scheme.Password),
	}
	return (*C.struct_Scheme)(unsafe.Pointer(&CScheme))
}

// 发现设备ip需要的参数
func GetIpResolveParam(param *IPByResolveSvrParam) C.struct_IPByResolveSvrParam {
	var CParam C.struct_IPByResolveSvrParam
	CParam = C.struct_IPByResolveSvrParam{
		sServerIP: C.CString(param.ServerIP),
		wServerPort: C.ushort(param.ServerPort),
		sDVRName: (*C.uchar)(unsafe.Pointer(C.CString(param.DVRName))),
		sDVRSerialNumber: (*C.uchar)(unsafe.Pointer(C.CString(param.DVRSerialNumber))),
	}

	return CParam
}

// 获取本地服务器多网卡IP地址
func GetLocalIp(strIp [16][16]C.char) []string {
	var welcome []string
	for i := 0; i < len(strIp); i++ {
		var ipch []byte
		for j := 0; j < len(strIp[i]); j++ {
			if strIp[i][j] != 0 {
				ipch = append(ipch, byte(strIp[i][j]))
			}
		}
		welcome = append(welcome, string(ipch))
	}

	return welcome
}

// 抓拍图片流解析
func GetStream(buf [204800]C.char, size int) []byte {
	var out []byte
	for i := 0; i < size; i++ {
		out = append(out, byte(buf[i]))
	}

	return out
}

// SDK参数设置
func LocalHealth(health *s.DeviceServiceHealth) C.struct_HealthParam {
	var param C.struct_HealthParam
	param = C.struct_HealthParam{
		connectTime: C.int(health.ConnectTime),
		recvTimeOut: C.int(health.RecvTimeOut),
		reconnect: C.int(health.Reconnect),
		logToFile: C.CString(health.LogToFile),
		logLevel: C.uint(health.LogLevel),
	}

	return param
}

// UploadImg 上传图片
func UploadImg(buf []byte, fileUploadPath string) (UploadImageResult, error) {
	result := UploadImageResult{}
	bodyBuf := bytes.NewBufferString("")
	bodyWriter := multipart.NewWriter(bodyBuf)
	// 创建一个文件句柄
	ioWriter, err := bodyWriter.CreateFormFile("file", "cap.jpeg")
	if err != nil {
		return result, err
	}
	// 将图片流写入文件句柄
	ioWriter.Write(buf)
	boundary := bodyWriter.Boundary()
	closeBuf := bytes.NewBufferString(fmt.Sprintf("\r\n--%s--\r\n", boundary))
	requestReader := io.MultiReader(bodyBuf, closeBuf)
	req, err := http.NewRequest("POST", fileUploadPath, requestReader)
	if err != nil {
		return result, err
	}
	req.Header.Add("Content-Type", "multipart/form-data; boundary="+boundary)
	req.ContentLength = int64(bodyBuf.Len()) + int64(closeBuf.Len())
	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return result, err
	}

	defer res.Body.Close()
	body, err := ioutil.ReadAll(res.Body)
	if err != nil {
		return result, err
	}

	err = json.Unmarshal(body, &result)
	if err != nil {
		return result, err
	}
	return result, nil
}
