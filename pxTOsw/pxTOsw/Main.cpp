#include<iostream>
#include<fstream>
#include <sstream>  
#include<string>
#include <windows.h>
#include <stdio.h>  
using namespace std;
string o_unit;
string n_unit;
double ratio;
double width;

//double->string
string toString(double d) {
	ostringstream os;
	if (os << d)
		return os.str();
	else
		return "false";
}

//保留两位有效数
double pointTwo(double num) {
	int t_num = num * 100;
	return (double)t_num / 100;
}

//重新设置数字
string setNum(string str, string num, int sl) {
	double n = atof(num.c_str());
	int o_num_len = num.length();
	string new_num = toString(pointTwo(n *ratio / width));
	int n_num_len = new_num.length();
	string new_str = str.replace(sl + 1, o_num_len + n_unit.length(), new_num + n_unit);
	return new_str;
}

//查找替换数字
string replaceNum(string str, int pl) {
	string num = "";
	while (pl >= 0) {
		char ch = str[pl];
		if ((ch >= '0'&&ch <= '9'))
			num = ch + num;
		else if (ch == '.' && (pl - 1) >= 0 && (pl + 1) <= str.length() && (str[pl - 1] >= '0'&&str[pl - 1] <= '9')) {
			num = ch + num;
		}
		else {
			return setNum(str, num, pl);
		}
		pl--;
	}
	return str;
}

//转换单位
void toReverse(string path) {
	ifstream read;
	read.open(path);
	ofstream out;
	out.open("temp.scss", ios::out);
	string str;
	int px_loc = -1;
	while (getline(read, str)) {
		int i = str.length();
		while (i >= 0) {
			int loc = str.rfind(o_unit, i);
			if (loc != -1) {
				i = loc - 1;
				if (str[i] >= '0'&&str[i] <= '9')
					str = replaceNum(str, i);
			}
			else
				i--;
		}
		out << str << endl;
	}
	read.close();
	out.close();
}

//遍历文件夹
void toDo(char * lpPath)
{
	char save_path[200];
	char szFile[MAX_PATH] = { 0 };
	char szFind[MAX_PATH];
	char root[MAX_PATH];

	WIN32_FIND_DATA FindFileData;
	strcpy(szFind, lpPath);

	strcat(szFind, "*.*");
	HANDLE hFind = ::FindFirstFile(szFind, &FindFileData);


	if (INVALID_HANDLE_VALUE == hFind) return;
	while (TRUE)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				strcpy(szFile, lpPath);
				//strcat(szFile,"//");  
				strcat(szFile, FindFileData.cFileName);
				strcat(szFile, "//");
				toDo(szFile);
			}
		}
		else
		{

			strcpy(root, lpPath);
			strcat(root, FindFileData.cFileName);
			string pa(root);
			if (pa != "temp.scss"&&pa.find(".scss")!=-1) {
				toReverse(pa);
				DeleteFile(pa.c_str());
				rename("temp.scss", pa.c_str());
				cout << pa << endl;
			}
		}


		if (!FindNextFile(hFind, &FindFileData)) break;
	}
	FindClose(hFind);
}
int main() {
	cout << "请输入替换单位（px,vw,vh..）:";
	cin >> o_unit;

	cout << "请输入新单位（px,vw,vh..）:";
	cin >> n_unit;

	cout << "输入放大倍数:";
	cin >> ratio;

	cout << "输入参考宽度:";
	cin >> width;

	toDo("");
	cout << "SUCCESS" << endl;
}
