#pragma  once
/*
����Ĵ�����Ϣ����
*/

namespace ChargingClosetError{

	//������Ϣ
	enum errorCode{
		noError = 0,		//��ȷ
		filePathError,		//�ļ�·������
		paramError,			//��������
		configInfoError,	//�����ļ����ݴ���
		recvTomeOut,	//���ճ�ʱ
	};

};