#include <string>
using namespace std;

class MyChiToLetter
{
public:
	MyChiToLetter();
	~MyChiToLetter();
	//分隔符
	bool m_LetterEnd;
	//TRUE:得到首字母大写
	//FALSE:得到首字母小写
	bool m_blnFirstBig;
	//TRUE:得到全部大写
	//FALSE:得到除去首拼音的所有小写
	bool m_blnAllBiG;
	//True:得到全部拼音
	//FALSE:得到首拼音
	bool m_blnSimaple;
	//返回拼音
	string GetLetter(const string& strText);
private:
    string FindLetter(int nCode);
};