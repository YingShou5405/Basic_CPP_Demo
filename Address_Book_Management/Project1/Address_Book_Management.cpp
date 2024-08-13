#include <iostream>
#include<string>
#include<stdlib.h>
#include<cctype>

#define MAX_LENGTH  1000 //����ͨѶ¼����󳤶�
#define RECORDS_PER_PAGE 3 //����ÿҳ��Ϣ����
using namespace std;

void showMenu() {
	cout << "******Address_Book_Management********" << endl;
	cout << "please enter the corresponding number��" << endl;
	cout << "1��add contacts" << endl;
	cout << "2��show contacts" << endl;
	cout << "3��delete contacts" << endl;
	cout << "4��search contacts" << endl;
	cout << "5��modify contacts" << endl;
	cout << "6��clear the Address book" << endl;
	cout << "0��exit" << endl;
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
	cout << "please enter the name: ";getline(cin,user_name);
	for (int i = 0; i < Address_Books->curr_size; i++) {
		if (user_name == Address_Books->contactsArray[i].name) {
			return i;
		}
	}
	return -1;
}

//������ϵ��
void add_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size >= MAX_LENGTH) {
		cout << "failed to add, the address book is already full, ";
		system("pasue");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "please enter the contacts info: "<<endl;
	cout << "name: ";getline(cin,name);
	Address_Books->contactsArray[Address_Books->curr_size].name = name;
	while (true) {
		cout << "sex:"; getline(cin,sex);
		if (sex == "male" || sex == "female") {
			Address_Books->contactsArray[Address_Books->curr_size].sex = sex;
			break;
		}
		cout << "please enter male or female...: " << endl;
	}
	while (true) {
		cout << "age: "; getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[Address_Books->curr_size].age = stoi(age);
				break;
			}
		}
		cout << "age is illegal! The range is 0 to 120...: " << endl;
	}
	while (true) {
		cout << "phone: "; getline(cin,phone);
		if (phone.length() == 11 && isNumber(phone)) {
			Address_Books->contactsArray[Address_Books->curr_size].phone = phone;
			break;
		}
		cout << "Phone number is illegal! Please enter the correct number...: " << endl;
	}
	cout << "address: "; getline(cin,address);
	Address_Books->contactsArray[Address_Books->curr_size].address = address;
	Address_Books->curr_size++;
	cout << "success!";
	system("pause");
	system("cls");
}

//��ʾ��ϵ��
void  show_contacts(Address_Books* Address_Books) {
	if (Address_Books->curr_size == 0) {
		system("cls");
		cout << "The Address Book is empty! " ;
		system("pause");
		system("cls");
		return;
	}
	if (Address_Books->curr_size <= RECORDS_PER_PAGE) {
		cout << "Contacts Info" << endl;
		cout << "ID\t"<<"NAME\t" << "SEX\t" << "AGE\t" << "PHONE\t" << "ADDRESS" << endl;
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
			cout << "********Contacts Info********" << endl;
			cout << "id\t"<<"name\t" << "sex\t" << "age\t" << "phone\t" << "address" << endl;
			for (int i = 0; i < min(RECORDS_PER_PAGE,(Address_Books->curr_size - bias)); i++) {
				print_contacts(Address_Books, i + bias);
			}
			cout << "please enter w or s to page up or page down, or enter e to exit...:";
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
		cout << "The person you want to delete is not in the list...";
		system("pause");
		system("cls");
		return;
	}
	string flag;
	while (true) {
		system("cls");
		cout << "The info you want to search:" << endl;
		cout << "ID\t" << "NAME\t" << "SEX\t" << "AGE\t" << "PHONE\t" << "ADDRESS" << endl;
		print_contacts(Address_Books, indices);
		cout << "Confirmed to delete?(y/n)��";
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
			cout << "Confirmed,";
			system("pause");system("cls");
			break;
		}
		else if (flag == "n") {
			cout << "Canceled,";
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
		cout << "The contacts you want to search is not in the list��";
		system("pause"); system("cls");
		return;
	}
	cout << "The info you want to search��\n";
	cout << "ID\t" << "NAME\t" << "SEX\t" << "AGE\t" << "PHONE\t" << "ADDRESS" << endl;
	print_contacts(Address_Books, indices);
	system("pause");
}

//�޸���ϵ��
void modify_contacts(Address_Books* Address_Books) {
	int indices = is_contacts_exists(Address_Books);
	if (indices == -1) {
		cout << "Contacts not in the list��";
		system("pause");
		system("cls");
		return;
	}
	string name, sex, phone, address,age;
	cout << "Please re-enter  the Contacts Info" << endl;
	cout << "NAME: "; getline(cin,name);
	while (true) {
		cout << "SEX: "; getline(cin,sex);
		if (sex == "male" || sex == "female") {
			Address_Books->contactsArray[indices].sex = sex;
			break;
		}
		cout << "Wrong Sex��Please enter male or female...:" << endl;
	}
	while (true) {
		cout << "AGE: ";getline(cin, age);
		if (isNumber(age)) {
			if (stoi(age) > 0 && stoi(age) <= 120) {
				Address_Books->contactsArray[indices].age = stoi(age);
				break;
			}
		}
		cout << "iLLegal Age��Range is 1 to 120...:" << endl;
	}
	while (true) {
		cout << "PHONE: "; getline(cin,phone);
		if (phone.length() == 11 && isNumber(phone)) {
			Address_Books->contactsArray[indices].phone = phone;
			break;
		}
		cout << "iLLegal Number! Please enter the Correct Number!" << endl;
	}
	cout << "Address: "; getline(cin,address);
	Address_Books->contactsArray[indices].address = address;
	system("cls");
	cout << "Success��";
	system("pause");
}

//���ͨѶ¼
void clear_Address_Books(Address_Books* Address_Books) {
	string flag;
	while (true) {
		system("cls");
		cout << "Confirm to Clear��(y/n)��";
		getline(cin, flag);
		if (flag == "y") {
			Address_Books->curr_size = 0;
			cout << "Cleared";
			system("pause"); system("cls"); break;
		}
		else if (flag == "n") {
			cout << "Canceled";
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
		case 1: // ������ϵ��
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
			modify_contacts(&Address_Book);			
			break;
		case 6:// ɾ��ͨѶ¼
			clear_Address_Books(&Address_Book);
			break;
		case 0: //�˳�ϵͳ
			cout << "Thanks for using��" << endl;
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