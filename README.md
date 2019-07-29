# Chrome and Opera _Decrypt
Get the login information (URL, login name, password，etc.) automatically saved by Google Chrome and Opera browser AND password decryption.
# 头文件 Chrome(Opera)_Decrypt.h
包含获取存储登录信息文件路径方法（getProfilePath）和字符转换方法（U2G）和解密后密码处理方法（Password_Handle）。
# 执行模块 run.cpp（包含数据库操作及密码解密）
# 获取路径模块 ProfilePath_Get.cpp
# 密码处理模块 Password_Handle.cpp
# 字符格式转换模块 UTF8_Trans.cpp
# 具体实现步骤及方法见[我的博客](https://www.shangzg.top/c++/technology/Chrome-login-information-extraction-and-password-decryption.html)
# 注：
1.浏览器在打开情况下可能无法读取信息.  
2.该项目需要添加`Sqlite3`依赖.
