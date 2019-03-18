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

void InsertBook(BTree &L, BookType B, result res);//插入新的书籍
void BorrowBook(BTree L, BookType B, ReaderType R);
int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R);//书籍归还
void PrintB(BookType B);//输出指定书籍的信息
void PrintAll(BTree p);//输出B树结点的全部关键字信息
void PrintOneBook(BookType B);
void PrintBorrower(ReaderType R);
int Menu() {//菜单
	cout << " ----------------------------------\n";
	cout << " \tLibrary Management" << endl;
	cout << " ----------------------------------\n";
	cout << "1.新书入库\t2.清除库存" << endl;
	cout << " ----------------------------------\n";
	cout << "3.借    书\t4.还    书" << endl;
	cout << " ----------------------------------\n";
	cout << "0.退出" << endl;
	cout << "-----------------------------------\n";
	int a;
	do {
		cout << "请选择:";
		cin >> a;
	} while (a < 0 || a>4);//避免非法输入
	return a;
}
void InsertBook(BTree &L, BookType B, result res) {
	//书库L已存在，res包含B书在书库L中的位置或应该插入的位置
	if (res.tag == 0) //书库中不存在该书，插入
		Insert(L, B->booknum, res.pt, res.i, B);
	else {//书库中存在该书，存量增加
		BookType b = res.pt->recptr[res.i];
		b->current = b->current + B->total;//现存量增加
		b->total = b->total + B->total;//总库存增加
	}
}

void BorrowBook(BTree L, BookType B, ReaderType R)
// 书库L存在，B书是书库中的书并且可被读者R借阅（已通过CanBorrow()判断）
// 借出一本B书，登记借阅者R的信息，改变现存量，记录借书日期，最迟应还日期等信息。
{
	ReaderType	r;
	if (!B->reader) B->reader = R;				// 无其他借阅者，则直接登记
	else {
		for (r = B->reader; r->rnext; r = r->rnext);
		r->rnext = R;							// 否则到借阅者链表尾，登记
	}
	B->current--;								// 现存量减1

}

int ReturnBook(BTree L, int booknum, int IDnum, BookType &B, ReaderType &R) {
	//booknum为还书书号，IDnum是借阅者的ID号
	//若书库中不存在书号为booknum的书，则搜索出错，返回-1
	//若有借阅记录，则注销该记录，并用B和R返回图书信息和借阅者信息并返回1
	//若没有r借阅的记录，则用B返回图书信息，并返回0
	result res = SearchBTree(L, booknum); //在B树上按书号搜索
	if (res.tag == 0) return -1;
	B = res.pt->recptr[res.i];
	ReaderType p = res.pt->recptr[res.i]->reader, pre = p;
	while (p) {
		if (pre == p && p->IDnum == IDnum) {
			R = p;
			B->current++;//现存量加一
			return 1;
		}
		if (p->IDnum == IDnum) {//多个借书者
			R = p;
			pre->rnext = p->rnext;
			B->current++;//现存量+1
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
	cout << "\n   书号\t书名\t\t作者\t现存\t总库存 "<<endl;
	cout << "-----------------------------------------------";

	for (int i = 1; i <= p->keynum; i++) {
		PrintB(p->recptr[i]);
	}
}
void PrintOneBook(BookType B) {
	cout << "-----------------------------------------------";
	cout << "\n   书号\t书名\t\t作者\t现存\t总库存 " << endl;
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
	if ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) days = days2; //判断闰年
	cout << days[m2 - 1] + d2 - days[m - 1] - d << endl;
	return 0;
}
