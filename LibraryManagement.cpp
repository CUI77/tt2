// LibraryManagement.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"BTree.h"
#include"Manage.h"

int main() {
	BTree L = NULL;
	BookType B;
	result res;
	ReaderType R;
	int booknum, IDnum, k;
	char yn;
	Sleep(800);
	system("cls");//清屏
	for (;;) {
		switch (Menu()) {
		case 1://新书入库
		{
			int h = 0;
			//	do {
			B = (BookType)malloc(sizeof(BookNode));
			B->reader = NULL;
			cout << "请输入要入库的书号：\n";
			fflush(stdin);
			//scanf_s("%d", &B->booknum);
			for (;;)
			{
				int a1 = scanf_s("%d", &B->booknum);
				if (a1 == 0)
				{
					getchar();
					cout<<"不是数字，重输：";
				}
				else
					break;
			}
			//getchar();
			res = SearchBTree(L, B->booknum);
			if (!res.tag) {
				fflush(stdin);//清空输入缓冲区
				cout << "请输入书名：\n";
				fflush(stdin);
				cin >> B->bookname;
				//gets_s(B->bookname);
				cout<<"请输入作者：\n";
				fflush(stdin);
				cin >> B->writer;
				//gets_s(B->writer);
				cout << "请输入入库数：\n";
				fflush(stdin);
				for (;;)
				{
					int b1 = scanf_s("%d", &B->current);
					if (b1 == 0)
					{
						getchar();
						cout << "不是数字，重输：";
					}
					else
						break;
				}
				B->total = B->current;
				InsertBook(L, B, res);
				cout << "B树的凹入表显示：\n";
				//ShowBTree(L, 8);
				ShowBTree(L);
				//break;
			}
			else {//图书已存在，只增加库存
				cout << "该书已在书库中，请输入增加的册数：\n";
				fflush(stdin);
				scanf_s("%d", &B->total);
				InsertBook(L, B, res);
				B = res.pt->recptr[res.i];
				cout << "凹入表显示B树：\n";
				ShowBTree(L);
				cout << "入库完成！\n";
				system("pause");
				//break;
			}
			/*cout << "--------------------------------------------------------" << endl;
			cout << "1：继续当前操作" << endl;
			cout << "0: 返回上一级菜单" << endl;
			cout << "请输入--------------------------------------------------" << endl;
			cin >> h;
			} while (h = 1);//????
			*/
			break;
		}
		case 2://清空库存
			PrintAll(L);
			cout << "请输入要删除的图书书号：\n";
			fflush(stdin);
			scanf_s("%d", &booknum);
			res = SearchBTree(L, booknum);
			if (res.tag) {
				B = res.pt->recptr[res.i];
				PrintOneBook(B);
				if (B->current != B->total)cout << "提示：该书尚未全部归还！" << endl;
				cout << "\n确认删除?  是（Y） 否（N）";
				fflush(stdin);
				//getchar();
				//flag = getchar();
				//int yn;
				cin >> yn;
				if (yn == 'Y' || yn == 'y') {
					//DeleteBook(L, B);
					Remove(L, B->booknum);
					cout<<"\n清除成功！\t凹入表显示B树如下：\n";
					//ShowBTree(L, 8);
					ShowBTree(L);
				}
			}
			else {
				cout << "\n删除失败！\n";
			}
			cout << "\t";
			system("pause");
			break;
		case 3://借阅图书
			R = (ReaderType)malloc(sizeof(ReaderNode));
			R->rnext = NULL;
			PrintAll(L);
			cout << "请输入借阅图书书号：\n";
			fflush(stdin);
			scanf_s("%d", &booknum);
			getchar();
			res = SearchBTree(L, booknum);
			if (res.tag) {
				B = res.pt->recptr[res.i];
				cout << "输入您的姓名：\n";
				fflush(stdin);
				gets_s(R->reader_name);
				cout << "输入您的ID号：\n";
				fflush(stdin);
				scanf_s("%d", &R->IDnum);
				cout << "输入当前 年_月_日：" << endl;
				cin >> R->date.year >> R->date.month >> R->date.day;
				int noleap[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
				int leap[] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
				int *days = noleap;
				if ((R->date.year % 4 == 0 && R->date.year % 100 != 0) || R->date.year % 400 == 0)
					days = leap; //判断闰年
								 //curDate = d + 30 - days[m]
								 //if (CanBorrow(L, B, R)) {
				if (B->current>0) {
					BorrowBook(L, B, R);
					cout << "\n借书完成！";
					cout << "请于" << R->date.year << "年" << R->date.month + 1 << "月";
					cout << R->date.day + 30 - days[R->date.month] << "日前归还" << endl;
				}
				else {
					cout << "\n该书目前库存为0";
					free(R);
				}
			}
			else {
				cout << "\n没有您要借阅的书";
				free(R);
			}
			printf("\t");
			system("pause");
			break;
		case 4://归还图书
			cout << "\n请输入图书号：\n";
			scanf_s("%d", &booknum);
			cout << "请输入您的ID号：\n";
			scanf_s("%d", &IDnum);
			k = ReturnBook(L, booknum, IDnum, B, R);
			if (k == 1) {
				cout << "\n还书完成！ ";
				R = NULL;
			}
			else if (k == 0) {
				cout << "\n该书没有您的借阅记录";
			}
			else {
				cout << "\n不存在此书";
			}
			printf("\t");
			system("pause");
			break;
			/*case 5://输出总库存
			printf("图书馆总库存信息如下：\n");
			PrintAllbooks(L);
			break;
			case 6://查找某种图书
			printf("请输入要查找的图书的编号：\n");
			scanf_s("%d", &book_num);
			ShowBookinfo(L, book_num);
			break;
			case 7://输出某图书的借阅者信息
			printf("请输入要查找的图书的编号：\n");
			scanf_s("%d", &book_num);
			res = SearchBTree(L, book_num);
			if (res.tag) {
			B = res.pt->recptr[res.i];
			printf("借阅者信息如下：\n");
			PrintBorrower(B->reader);
			}
			else {
			printf("\n书库不存在此书，查找失败！\n");
			}
			printf("\t");
			system("pause");
			break;
			case 8:Creat(L);
			system("cls");
			break;*/
		case 0:  //退出管理系统
			exit(0);
		default:
			break;
		}
	}
	return 0;
}

