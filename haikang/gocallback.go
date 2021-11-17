package haikang

import "C"
import (
	"unsafe"
	"fmt"
)

//export GoRealDataCallBack
func GoRealDataCallBack(lRealHandle C.int, dwDataType C.uint, pBuffer *C.uchar, dwBufSize C.uint, pUser unsafe.Pointer) {
	fmt.Println("GoRealDataCallBack")
	fmt.Println(C.uint(dwBufSize))
	fmt.Println((*C.char)(unsafe.Pointer(pBuffer)))
	fmt.Println(C.int(lRealHandle))
	fmt.Println(C.uint(dwDataType))
	fmt.Println((*C.struct_RealPlayInfo)(pUser))
	return
}

//export GoCatchErrorCallback
func GoCatchErrorCallback(dwType C.uint, lUserID C.int, lHandle C.int, pUser unsafe.Pointer) {
	fmt.Println("GoCatchErrorCallback")
	// EXCEPTION_RECONNECT
	fmt.Println(C.uint(dwType))
	fmt.Println(C.int(lHandle))
	fmt.Println(C.int(lUserID))
	fmt.Println((*C.uint)(pUser))
	return
}

//export GoAlarmMsgCallback
func GoAlarmMsgCallback(lCommand C.int, pAlarmer unsafe.Pointer, pAlarmInfo *C.char, dwBufLen C.uint, pUser unsafe.Pointer) bool {
	fmt.Println("GoAlarmMsgCallback")
	fmt.Println(C.int(lCommand))
	fmt.Println((*C.char)(pAlarmInfo))
	fmt.Println((*C.struct_NET_DVR_ALARMER)(pAlarmer))
	fmt.Println(C.uint(dwBufLen))
	fmt.Println((*C.uint)(pUser))
	return false
}
