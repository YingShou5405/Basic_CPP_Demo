#include <iostream>
#include<string>
#include<stdlib.h>
#include<cctype>

#define MAX_LENGTH  1000 //定义通讯录的最大长度
#define RECORDS_PER_PAGE 3 //定义每页信息长度
using namespace std;

void showMenu() {
	cout << "******通讯录管理系统********" << endl;
	cout << "请输入与功能对应的数字：" << endl;
	cout << "1、添加联系人" << endl;
	cout << "2、显示联系人" << endl;
	cout << "3、删除联系人" << endl;
	cout << "4、查找联系人" << endl;
	cout << "5、修改联系人" << endl;
	cout << "6、清空联系人" << endl;
	cout << "0、退出" << endl;
};

struct contacts //联系人结构体
{
		string name;
		string sex;
		int age;
		string phone;
		string address;
};

struct Address_Books
{
	struct contacts contactsArray[MAX_LENGTH];
	int curr_size; // 记录当前通讯录内的人数
};

//判断年龄是否合法
bool isNumber(string age) {
	for (char ch : age) {
		if (!isdigit(ch)) {
			return false;
		}
	}
	return true;
}

// 打印联络人
void print_contacts(Address_Books* Address_Book, int indices) {
	cout << indices << "\t";
	cout << Address_Book->contactsArray[indices].name << "\t";
	cout << Address_Book->contactsArray[indices].sex << "\t";
	cout << Address_Book->contactsArray[indices].age << "\t";
	cout << Address_Book->contactsArray[indices].phone << "\t";
	cout << Address_Book->contactsArray[indices].address << endl;
}

// 返回联系人所在数组下表
int is_contacts_exists(Address_Books* Address_Books) {
	string user_name;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "请输入联系人姓名：";getline(cin,user_name);
	for (int i = 0; i < Address_Books->curr_size; i++) {
		if (user_name == Address_Books->contactsArray[i].name) {
			return i;
		}
	}
	return -1;
}

//添加联系人
void add_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size >= MAX_LENGTH) {
		cout << "添加失败！用户数量已满，";
		system("pasue");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "请输入添加联系人的信息："<<endl;
	cout << "姓名：";getline(cin,name);
	Address_Books->contactsArray[Address_Books->curr_size].name = name;
	while (true) {
		cout << "性别："; getline(cin,sex);
		if (sex == "male" or sex == "female") {
			Address_Books->contactsArray[Address_Books->curr_size].sex = sex;
			break;
		}
		cout << "您输入的性别有误！请输入male或者female...:" << endl;
	}
	while (true) {
		cout << "年龄："; getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[Address_Books->curr_size].age = stoi(age);
				break;
			}
		}
		cout << "您输入的年龄不合法！请输入1-120内的整数...:" << endl;
	}
	while (true) {
		cout << "电话："; getline(cin,phone);
		if (phone.length() == 11&&isNumber(phone)) {
			Address_Books->contactsArray[Address_Books->curr_size].phone = phone;
			break;
		}
		cout << "您输入的号码格式不正确！请重新输入!" << endl;
	}
	cout << "地址："; getline(cin,address);
	Address_Books->contactsArray[Address_Books->curr_size].address = address;
	Address_Books->curr_size++;
	cout << "添加成功，";
	system("pause");
	system("cls");
}

//显示联系人
void  show_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size == 0) {
		system("cls");
		cout << "联系人为空！" ;
		system("pause");
		system("cls");
		return;
	}
	if (Address_Books->curr_size <= RECORDS_PER_PAGE) {
		cout << "联系人信息" << endl;
		cout << "序号\t"<<"姓名\t" << "性别\t" << "年龄\t" << "电话\t" << "地址" << endl;
		for (int i = 0; i < Address_Books->curr_size; i++) {
			print_contacts(Address_Books, i);
		}
		system("pause"); system("cls");
	}
	else {
		char operation;
		int page = 0;
		while (true) {
			system("cls");
			int bias = page * RECORDS_PER_PAGE;
			cout << "********联系人信息********" << endl;
			cout << "序号\t"<<"姓名\t" << "性别\t" << "年龄\t" << "电话\t" << "地址" << endl;
			for (int i = 0; i < min(RECORDS_PER_PAGE,(Address_Books->curr_size - bias)); i++) {
				print_contacts(Address_Books, i + bias);
			}
			cout << "请按w或者s翻页,或者按e退出...:";
			cin >> operation;
			if (operation == 'w' && page != 0) {
				page--; system("cls"); 
			}
			else if (operation == 's' && (page + 1) * RECORDS_PER_PAGE <= Address_Books->curr_size) {
				page++;system("cls"); 
			}
			else if (operation == 'e') {
				system("cls"); return;
			}
		}
	}
}

//删除联系人
void del_contacts(Address_Books * Address_Books) {
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "您所要删除的联系人不在列表当中...";
		system("pause");
		system("cls");
		return;
	}
	string flag;
	while (true) {
		system("cls");
		cout << "您所查找的联系人信息为:" << endl;
		cout << "序号\t" << "姓名\t" << "性别\t" << "年龄\t" << "电话\t" << "地址" << endl;
		print_contacts(Address_Books, indices);
		cout << "您确定要删除吗?(y/n)：";
		getline(cin,flag);
		if ( flag == "y") {
			for (int i = indices; i < Address_Books->curr_size - 1; i++) {
				Address_Books->contactsArray[i].name = Address_Books->contactsArray[i + 1].name;
				Address_Books->contactsArray[i].sex = Address_Books->contactsArray[i + 1].sex;
				Address_Books->contactsArray[i].age = Address_Books->contactsArray[i + 1].age;
				Address_Books->contactsArray[i].phone = Address_Books->contactsArray[i + 1].phone;
				Address_Books->contactsArray[i].address = Address_Books->contactsArray[i + 1].address;
			}
			Address_Books->curr_size--;
			cout << "联系人已删除,";
			system("pause");system("cls");
			break;
		}
		else if (flag == "n") {
			cout << "您选择不删除,";
			system("pause");
			system("cls");
			break;
		}
		system("cls");
	}
}

// 查找联系人
void find_contacts(Address_Books* Address_Books) {
	system("cls");
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "您所查找的联系人不在通讯录当中！";
		system("pause"); system("cls");
		return;
	}
	cout << "您查找的联系人信息为：\n";
	cout << "序号\t" << "姓名\t" << "性别\t" << "年龄\t" << "电话\t" << "地址" << endl;
	print_contacts(Address_Books, indices);
	system("pause");
}

//修改联系人
void modify_liaison(Address_Books* Address_Books) {
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "您所想修改的用户不在通讯录中！";
		system("pause");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cout << "请重新输入你想修改的用户信息" << endl;
	cout << "姓名："; getline(cin,name);
	while (true) {
		cout << "性别："; getline(cin,sex);
		if (sex == "male" or sex == "female") {
			Address_Books->contactsArray[indices].sex = sex;
			break;
		}
		cout << "您输入的性别有误！请输入male或者female...:" << endl;
	}
	while (true) {
		cout << "年龄：";getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[indices].age = stoi(age);
				break;
			}
		}
		cout << "您输入的年龄不合法！请输入1-120内的整数...:" << endl;
	}
	while (true) {
		cout << "电话："; getline(cin,phone);
		if (phone.length() == 11 && isNumber(phone)) {
			Address_Books->contactsArray[indices].phone = phone;
			break;
		}
		cout << "您输入的号码格式不正确！请重新输入!" << endl;
	}
	cout << "地址："; getline(cin,address);
	Address_Books->contactsArray[indices].address = address;
	system("cls");
	cout << "修改成功！";
	system("pause");
}

//清除通讯录
void clear_Address_Books(Address_Books* Address_Books) {
	string flag;
	while (true) {
		system("cls");
		cout << "确定要清空吗？(y/n)：";
		getline(cin, flag);
		if (flag == "y") {
			Address_Books->curr_size = 0;
			cout << "通讯录已清空，";
			system("pause"); system("cls"); break;
		}
		else if (flag == "n") {
			cout << "取消清空，";
			system("pause"); system("cls"); break;
		}
		system("cls");
	}


}

int main() {
	Address_Books  Address_Book;
	Address_Book.curr_size = 0;
	while (true) {
		int select;
		showMenu();
		cin >> select;
		switch (select) {
		case 1: // 添加联系人
			add_contacts(&Address_Book);
			break;
		case 2: // 显示联系人
			show_contacts(&Address_Book);
			break;
		case 3: // 删除联系人
			del_contacts(&Address_Book);
			break;
		case 4 :// 查找联系人
			find_contacts(&Address_Book);
			break;
		case 5:// 修改联系人
			modify_liaison(&Address_Book);			
			break;
		case 6:// 删除通讯录
			clear_Address_Books(&Address_Book);
			break;
		case 0: //退出系统
			cout << "感谢使用，下次再见(Thanks for using)，" << endl;
			system("pause");
			return 0;
		default:
			system("cls");
			break;
		}
	};
	system("pause");
	return 0;
}