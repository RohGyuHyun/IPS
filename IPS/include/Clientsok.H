/////////////////////////////////////////////////////////////////////////////////
//clientsok.h
//Ŭ���̾�Ʈ�� ���� ���α׷� ���

#define WM_RECEIVE_DATA WM_USER+2
#define WM_CLOSE_SOCK WM_USER+3

class CClientSock : public CAsyncSocket
{

public:
		CClientSock();
		//�޼����� ������ HWND�� �����ϴ� �Լ�
		void SetWnd(HWND hwnd);
		//���ο� ����Ÿ�� �������� ����Ǵ� �Լ�
		virtual void OnReceive( int nErrorCode );
		virtual void OnClose(int nErrorCode);
public:
		//�޼����� ������ HWND
		HWND m_pHwnd;
};
///////////////////////////////////////////////////////////////////////////////