#include <iostream>
#include<string>
#include<stdlib.h>
#include<cctype>

#define MAX_LENGTH  1000 //����ͨѶ¼����󳤶�
#define RECORDS_PER_PAGE 3 //����ÿҳ��Ϣ����
using namespace std;

void showMenu() {
	cout << "******ͨѶ¼����ϵͳ********" << endl;
	cout << "�������빦�ܶ�Ӧ�����֣�" << endl;
	cout << "1�������ϵ��" << endl;
	cout << "2����ʾ��ϵ��" << endl;
	cout << "3��ɾ����ϵ��" << endl;
	cout << "4��������ϵ��" << endl;
	cout << "5���޸���ϵ��" << endl;
	cout << "6�������ϵ��" << endl;
	cout << "0���˳�" << endl;
};

struct contacts //��ϵ�˽ṹ��
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
	int curr_size; // ��¼��ǰͨѶ¼�ڵ�����
};

//�ж������Ƿ�Ϸ�
bool isNumber(string age) {
	for (char ch : age) {
		if (!isdigit(ch)) {
			return false;
		}
	}
	return true;
}

// ��ӡ������
void print_contacts(Address_Books* Address_Book, int indices) {
	cout << indices << "\t";
	cout << Address_Book->contactsArray[indices].name << "\t";
	cout << Address_Book->contactsArray[indices].sex << "\t";
	cout << Address_Book->contactsArray[indices].age << "\t";
	cout << Address_Book->contactsArray[indices].phone << "\t";
	cout << Address_Book->contactsArray[indices].address << endl;
}

// ������ϵ�����������±�
int is_contacts_exists(Address_Books* Address_Books) {
	string user_name;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "��������ϵ��������";getline(cin,user_name);
	for (int i = 0; i < Address_Books->curr_size; i++) {
		if (user_name == Address_Books->contactsArray[i].name) {
			return i;
		}
	}
	return -1;
}

//�����ϵ��
void add_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size >= MAX_LENGTH) {
		cout << "���ʧ�ܣ��û�����������";
		system("pasue");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "�����������ϵ�˵���Ϣ��"<<endl;
	cout << "������";getline(cin,name);
	Address_Books->contactsArray[Address_Books->curr_size].name = name;
	while (true) {
		cout << "�Ա�"; getline(cin,sex);
		if (sex == "male" or sex == "female") {
			Address_Books->contactsArray[Address_Books->curr_size].sex = sex;
			break;
		}
		cout << "��������Ա�����������male����female...:" << endl;
	}
	while (true) {
		cout << "���䣺"; getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[Address_Books->curr_size].age = stoi(age);
				break;
			}
		}
		cout << "����������䲻�Ϸ���������1-120�ڵ�����...:" << endl;
	}
	while (true) {
		cout << "�绰��"; getline(cin,phone);
		if (phone.length() == 11&&isNumber(phone)) {
			Address_Books->contactsArray[Address_Books->curr_size].phone = phone;
			break;
		}
		cout << "������ĺ����ʽ����ȷ������������!" << endl;
	}
	cout << "��ַ��"; getline(cin,address);
	Address_Books->contactsArray[Address_Books->curr_size].address = address;
	Address_Books->curr_size++;
	cout << "��ӳɹ���";
	system("pause");
	system("cls");
}

//��ʾ��ϵ��
void  show_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size == 0) {
		system("cls");
		cout << "��ϵ��Ϊ�գ�" ;
		system("pause");
		system("cls");
		return;
	}
	if (Address_Books->curr_size <= RECORDS_PER_PAGE) {
		cout << "��ϵ����Ϣ" << endl;
		cout << "���\t"<<"����\t" << "�Ա�\t" << "����\t" << "�绰\t" << "��ַ" << endl;
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
			cout << "********��ϵ����Ϣ********" << endl;
			cout << "���\t"<<"����\t" << "�Ա�\t" << "����\t" << "�绰\t" << "��ַ" << endl;
			for (int i = 0; i < min(RECORDS_PER_PAGE,(Address_Books->curr_size - bias)); i++) {
				print_contacts(Address_Books, i + bias);
			}
			cout << "�밴w����s��ҳ,���߰�e�˳�...:";
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

//ɾ����ϵ��
void del_contacts(Address_Books * Address_Books) {
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "����Ҫɾ������ϵ�˲����б���...";
		system("pause");
		system("cls");
		return;
	}
	string flag;
	while (true) {
		system("cls");
		cout << "�������ҵ���ϵ����ϢΪ:" << endl;
		cout << "���\t" << "����\t" << "�Ա�\t" << "����\t" << "�绰\t" << "��ַ" << endl;
		print_contacts(Address_Books, indices);
		cout << "��ȷ��Ҫɾ����?(y/n)��";
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
			cout << "��ϵ����ɾ��,";
			system("pause");system("cls");
			break;
		}
		else if (flag == "n") {
			cout << "��ѡ��ɾ��,";
			system("pause");
			system("cls");
			break;
		}
		system("cls");
	}
}

// ������ϵ��
void find_contacts(Address_Books* Address_Books) {
	system("cls");
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "�������ҵ���ϵ�˲���ͨѶ¼���У�";
		system("pause"); system("cls");
		return;
	}
	cout << "�����ҵ���ϵ����ϢΪ��\n";
	cout << "���\t" << "����\t" << "�Ա�\t" << "����\t" << "�绰\t" << "��ַ" << endl;
	print_contacts(Address_Books, indices);
	system("pause");
}

//�޸���ϵ��
void modify_liaison(Address_Books* Address_Books) {
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "�������޸ĵ��û�����ͨѶ¼�У�";
		system("pause");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cout << "���������������޸ĵ��û���Ϣ" << endl;
	cout << "������"; getline(cin,name);
	while (true) {
		cout << "�Ա�"; getline(cin,sex);
		if (sex == "male" or sex == "female") {
			Address_Books->contactsArray[indices].sex = sex;
			break;
		}
		cout << "��������Ա�����������male����female...:" << endl;
	}
	while (true) {
		cout << "���䣺";getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[indices].age = stoi(age);
				break;
			}
		}
		cout << "����������䲻�Ϸ���������1-120�ڵ�����...:" << endl;
	}
	while (true) {
		cout << "�绰��"; getline(cin,phone);
		if (phone.length() == 11 && isNumber(phone)) {
			Address_Books->contactsArray[indices].phone = phone;
			break;
		}
		cout << "������ĺ����ʽ����ȷ������������!" << endl;
	}
	cout << "��ַ��"; getline(cin,address);
	Address_Books->contactsArray[indices].address = address;
	system("cls");
	cout << "�޸ĳɹ���";
	system("pause");
}

//���ͨѶ¼
void clear_Address_Books(Address_Books* Address_Books) {
	string flag;
	while (true) {
		system("cls");
		cout << "ȷ��Ҫ�����(y/n)��";
		getline(cin, flag);
		if (flag == "y") {
			Address_Books->curr_size = 0;
			cout << "ͨѶ¼����գ�";
			system("pause"); system("cls"); break;
		}
		else if (flag == "n") {
			cout << "ȡ����գ�";
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
		case 1: // �����ϵ��
			add_contacts(&Address_Book);
			break;
		case 2: // ��ʾ��ϵ��
			show_contacts(&Address_Book);
			break;
		case 3: // ɾ����ϵ��
			del_contacts(&Address_Book);
			break;
		case 4 :// ������ϵ��
			find_contacts(&Address_Book);
			break;
		case 5:// �޸���ϵ��
			modify_liaison(&Address_Book);			
			break;
		case 6:// ɾ��ͨѶ¼
			clear_Address_Books(&Address_Book);
			break;
		case 0: //�˳�ϵͳ
			cout << "��лʹ�ã��´��ټ�(Thanks for using)��" << endl;
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