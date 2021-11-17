package haikang

type IPByResolveSvrParam struct {
	ServerIP        string
	ServerPort      int
	DVRName         string
	DVRNameLen      int
	DVRSerialNumber string
	DVRSerialLen    int
}

type IPParaCfgDto struct {
	// other response param
	ReturnLen int
}

type IPByResolveSvrDto struct {
	SGetIP string
	DwPort int
}

type RealPlayInfo struct {
	SGetIP   string
	LUserID  int
	LChannel int
}

type UploadImageResult struct {
	Code    int      `json:"code"`
	Message string   `json:"message"`
	Data    []string `json:"data"`
}

type PingParam struct {
	Welcome interface{} `json:"welcome"` // hello world
}

type Response struct {
	Code    int         `json:"code"`
	Data    interface{} `json:"data"`
	Message string      `json:"message"`
}
