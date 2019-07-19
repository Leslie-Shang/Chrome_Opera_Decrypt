#include "Chrome(Opera)_Decrypt.h"
#include <Wincrypt.h>
#include "sqlite3.h"
#pragma comment(lib,"crypt32.lib")
#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	sqlite3* DB = NULL; // 一个打开的数据库实例
	sqlite3_stmt* stmt = NULL;// sqlite3_stmt 预编译语句对象. 由sqlite3_prepare()创建，由sqlite3_finalize()销毁
	string LoginDataPath = getProfilePath() + "Login Data";
	const char* path = LoginDataPath.c_str();	// Login Data sql文件的路径
	char** dbResult; // 是 char ** 类型，两个*号
	int nRow, nColumn;// 表的行数，列数
	int index;
	DATA_BLOB DataOut;
	DATA_BLOB Dataput;
	LPWSTR pbtest = NULL;

	// 根据文件路径打开数据库连接。如果数据库不存在，则创建。
	// 数据库文件的路径必须以C字符串传入。
	int result = sqlite3_open(path, &DB);

	if (result == SQLITE_OK) {
		std::clog << "打开数据库连接成功\n";
		//开始查询，传入的 dbResult 已经是 char **，这里又加了一个 & 取地址符，传递进去的就成了char ***
		const char* sql = "Select * from logins";
		int result1 = sqlite3_prepare_v2(DB, sql, -1, &stmt, NULL);
		int result2 = sqlite3_get_table(DB, "Select * from logins", &dbResult, &nRow, &nColumn, NULL);
		if (result1 == SQLITE_OK && result2 == SQLITE_OK)
		{
			//查询成功
			printf("表格共%d 记录!\n", nRow);
			printf("表格共%d 列!\n", nColumn);
			printf("字段名|字段值\n");
			index = nColumn;
			for (int i = 0; i < nRow; i++)
			{
				printf("第 %d 条记录\n", i);
				for (int j = 0; j < nColumn; j++)
				{
					if (!(strcmp(dbResult[j], "password_value")))//如果列名为password_value，则需要对该列的列值进行解密操作
					{
						if (sqlite3_step(stmt) == SQLITE_ROW) {
							//初始化加密结构DataOut
							BYTE* pbDataInput = (BYTE*)sqlite3_column_text(stmt, j);
							DWORD cbDataInput = sqlite3_column_bytes(stmt, j);
							DataOut.pbData = pbDataInput;
							DataOut.cbData = cbDataInput;
						}
						if (CryptUnprotectData(&DataOut, &pbtest, NULL, NULL, NULL, 0, &Dataput))
						{
							string s = Password_Handle((char*)Dataput.pbData);//解密后的密码处理
							strcpy(dbResult[index], (char*)s.c_str());
						}
						else
						{
							printf("Decryption error!\n");
						}
					}
					printf("字段名: %s | 字段值: %s\n", dbResult[j], U2G(dbResult[index]));
					index++;
					// dbResult 的字段值是连续的，从第0索引到第 nColumn - 1索引都是字段名称，从第 nColumn 索引开始，后面都是字段值，它把一个二维的表（传统的行列表示法）用一个扁平的形式来表示
				}
				printf("--------------------------------------------------------------------------------\n");
			}
		}
		//清理语句句柄，准备执行下一个语句
		sqlite3_finalize(stmt);
		//到这里，不论数据库查询是否成功，都释放 char** 查询结果dbResult，使用 sqlite 提供的功能来释放
		//sqlite3_free_table(dbResult); //执行会抛出断点，因为在运行过程中dbResult的地址已改变，不是最初分配给他的
		//关闭数据库
		sqlite3_close(DB);
	}
	else {
		std::clog << "打开数据库连接失败\n";
	}
	return 0;
}