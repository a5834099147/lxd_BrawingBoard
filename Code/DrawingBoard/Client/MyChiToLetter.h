#include <string>
using namespace std;

class MyChiToLetter
{
public:
	MyChiToLetter();
	~MyChiToLetter();
	//�ָ���
	bool m_LetterEnd;
	//TRUE:�õ�����ĸ��д
	//FALSE:�õ�����ĸСд
	bool m_blnFirstBig;
	//TRUE:�õ�ȫ����д
	//FALSE:�õ���ȥ��ƴ��������Сд
	bool m_blnAllBiG;
	//True:�õ�ȫ��ƴ��
	//FALSE:�õ���ƴ��
	bool m_blnSimaple;
	//����ƴ��
	string GetLetter(const string& strText);
private:
    string FindLetter(int nCode);
};