#pragma  once
/*
软件的错误信息声明
*/

namespace ChargingClosetError{

	//错误信息
	enum errorCode{
		noError = 0,		//正确
		filePathError,		//文件路径错误
		paramError,			//参数错误
		configInfoError,	//配置文件内容错误
		recvTomeOut,	//接收超时
	};

};