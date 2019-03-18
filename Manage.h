#include	"windows.h"
#include	"stdio.h"
#include	"conio.h"
#include	"stdlib.h"
#include	"malloc.h"
#include	"time.h"
#include    "errno.h"
#include "BTree.h"
#include <iostream>
#include <string>
using namespace std;

void InsertBook(BTree &L, BookType B, result res);//�����µ��鼮
void BorrowBook(BTree L, BookType B, ReaderType R);
int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R);//�鼮�黹
void PrintB(BookType B);//���ָ���鼮����Ϣ
void PrintAll(BTree p);//���B������ȫ���ؼ�����Ϣ
void PrintOneBook(BookType B);
void PrintBorrower(ReaderType R);
int Menu() {//�˵�
	cout << " ----------------------------------\n";
	cout << " \tLibrary Management" << endl;
	cout << " ----------------------------------\n";
	cout << "1.�������\t2.������" << endl;
	cout << " ----------------------------------\n";
	cout << "3.��    ��\t4.��    ��" << endl;
	cout << " ----------------------------------\n";
	cout << "0.�˳�" << endl;
	cout << "-----------------------------------\n";
	int a;
	do {
		cout << "��ѡ��:";
		cin >> a;
	} while (a < 0 || a>4);//����Ƿ�����
	return a;
}
void InsertBook(BTree &L, BookType B, result res) {
	//���L�Ѵ��ڣ�res����B�������L�е�λ�û�Ӧ�ò����λ��
	if (res.tag == 0) //����в����ڸ��飬����
		Insert(L, B->booknum, res.pt, res.i, B);
	else {//����д��ڸ��飬��������
		BookType b = res.pt->recptr[res.i];
		b->current = b->current + B->total;//�ִ�������
		b->total = b->total + B->total;//�ܿ������
	}
}

void BorrowBook(BTree L, BookType B, ReaderType R)
// ���L���ڣ�B��������е��鲢�ҿɱ�����R���ģ���ͨ��CanBorrow()�жϣ�
// ���һ��B�飬�Ǽǽ�����R����Ϣ���ı��ִ�������¼�������ڣ����Ӧ�����ڵ���Ϣ��
{
	ReaderType	r;
	if (!B->reader) B->reader = R;				// �����������ߣ���ֱ�ӵǼ�
	else {
		for (r = B->reader; r->rnext; r = r->rnext);
		r->rnext = R;							// ���򵽽���������β���Ǽ�
	}
	B->current--;								// �ִ�����1

}

int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R) {
	//booknumΪ������ţ�IDnum�ǽ����ߵ�ID��
	//������в��������Ϊbooknum���飬��������������-1
	//���н��ļ�¼����ע���ü�¼������B��R����ͼ����Ϣ�ͽ�������Ϣ������1
	//��û��r���ĵļ�¼������B����ͼ����Ϣ��������0
	result res = SearchBTree(L, booknum); //��B���ϰ��������
	if (res.tag == 0) return -1;
	B = res.pt->recptr[res.i];
	ReaderType p = res.pt->recptr[res.i]->reader, pre = p;
	while (p) {
		if (pre == p && p->IDnum == IDnum) {
			R = p;
			B->current++;//�ִ�����һ
			return 1;
		}
		if (p->IDnum == IDnum) {//���������
			R = p;
			pre->rnext = p->rnext;
			B->current++;//�ִ���+1
			return 1;
		}
		pre = p;
		p = p->rnext;
	}
	return 0;
}

void PrintB(BookType B) {
	cout << "\n   "<<B->booknum<<"\t"<< B->bookname<<"\t\t"<< B->writer<<"\t"<< B->current<<"\t"<< B->total<< endl;
	cout << "-----------------------------------------------";
}
void PrintAll(BTree p) {
	cout << "-----------------------------------------------";
	cout << "\n   ���\t����\t\t����\t�ִ�\t�ܿ�� "<<endl;
	cout << "-----------------------------------------------";

	for (int i = 1; i <= p->keynum; i++) {
		PrintB(p->recptr[i]);
	}
}
void PrintOneBook(BookType B) {
	cout << "-----------------------------------------------";
	cout << "\n   ���\t����\t\t����\t�ִ�\t�ܿ�� " << endl;
	cout << "-----------------------------------------------";
	PrintB(B);
}

void PrintBorrower(ReaderType R) {
	ReaderType p = R;
	printf("------------------------\n");
	while (p) {
		printf("|  %-5d  |  %-10s  |\n", p->IDnum, p->reader_name);
		p = p->rnext;
	}
	printf("------------------------\n");
}
int PrintDate(int argc, char *argv[])
{
	int days1[13] = { 0,31,59,90,120,151,181,212,243,273,304,334,365 };
	int days2[13] = { 0,31,60,91,121,152,182,213,244,274,305,335,366 };
	int y, m, d, m2, d2;
	int *days = days1;
	cin >> y >> m >> d >> m2 >> d2;
	if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) days = days2; //�ж�����
	cout << days[m2 - 1] + d2 - days[m - 1] - d << endl;
	return 0;
}
