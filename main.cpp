#include <stdio.h>
#include <string>
#include <iostream>
 
int intercePosition = 0; // 记录单个运算数据的长度
int intercePositionIndex[] = { 0 }; // 记录“（”的下标
int intercePositionEnd[] = { 0 }; // 记录“）”的下标
std::string forward = "";
std::string back = "";
 
//替换所有字符串
std::string subreplace(std::string resource_str, std::string sub_str, std::string new_str)
{
        std::string::size_type pos = 0;
        while ((pos = resource_str.find(sub_str)) != std::string::npos)   //替换所有指定子串
        {
                resource_str.replace(pos, sub_str.length(), new_str);
        }
        return resource_str;
}
 
// 计算结果
std::string reslutString(char operation, double num1, double num2) {
        double sumResult = 0;
        char str[1000] = "";
        if (operation == '*')
                sumResult = num1 * num2;
        if (operation == '-')
                sumResult = num1 - num2;
        if (operation == '/')
                sumResult = num1 / num2;
        if (operation == '+')
                sumResult = num1 + num2;
        sprintf_s(str, sizeof(str), "%g", sumResult);
        //%g用来输出实数，它根据数值的大小，自动选f格式或e格式（选择输出时占宽度较小的一种），且不输出无意义的0
        return (std::string)str;
}
 
// 截取第一个数
double interceptNumIndex(std::string str) {
        double a = 0; // 记录数据
        int InrerceIndex = 0; // 记录运算符的位置
        bool temp = false; // 记录数据前运算符的状态
        char stringOperation[1000] = {};
        strcpy_s(stringOperation, str.c_str());
        for (int i = strlen(stringOperation) - 1; i >= 0; i--) {
                switch (stringOperation[i]) {
                case '*':
                case '/':
                case '+':
                case '-':
                        InrerceIndex = i;
                        temp = true;
                        break;
                default:
                        break;
                }
                if (temp)
                        break;
        }
        // 判断此数据是否在运算字符串的第一位
        if (InrerceIndex == 0) {
                a = atof(str.c_str());
                intercePosition = str.length();
        }
        else {
                std::string aa = str.substr(InrerceIndex);
                a = std::stof(aa);
                // 记录数据的真实长度
                intercePosition = str.substr(InrerceIndex).length();
        }
        return a;
}
 
// 截取第二个数
double interceptNumEnd(std::string str) {
        double a = 0;
        int InrerceIndex = 0;
        char stringOperation[1000] = {};
        strcpy_s(stringOperation, str.c_str());
        bool ispas = false; // 记录是否为负数
        for (unsigned int i = 0; i < strlen(stringOperation); i++) {
                switch (stringOperation[i]) {
                case '*':
                case '/':
                case '+':
                case '-':
                        InrerceIndex = i;
                        if (i != 0) // 判断该数是否为负数
                                ispas = true;
                        break;
                default:
                        break;
                }
                if (ispas)
                        break;
        }
        // 判断此数据是否在运算字符串的最后一位
        if (InrerceIndex == 0) {
                a = atof(str.c_str());
                intercePosition = str.length();
                if (ispas)
                        intercePosition++;
        }
        else {
                a = atof(str.substr(0, InrerceIndex).c_str());
                // 记录数据的真实长度
                intercePosition = str.substr(0, InrerceIndex).length();
        }
        return a;
}
 
// 不带括号的四则运算
std::string interceptOperation(std::string operationNumber, char a, char b, bool hasChar) {
        std::string mess = operationNumber;
        char stringOperation[1000] = {};
        strcpy_s(stringOperation, mess.c_str());
        // 循环遍历运算字符串，并做相应的运算
        for (unsigned int i = 0; i < strlen(stringOperation); i++) {
                // 判断运算符所在的索引
                if (stringOperation[i] == a || stringOperation[i] == b) {
                        if (i != 0) {
                                // 运算符前的第一个数
                                double num1 = interceptNumIndex(mess.substr(0, i));
                                // 记录第一个数据的长度
                                int frontPosition = intercePosition;
                                // 运算符前的第二个数
                                double num2 = interceptNumEnd(mess.substr(i + 1, strlen(stringOperation) - i - 1));
                                // 记录第二个数据的长度
                                int backPosition = intercePosition;
                                // 算完乘除，将结果替换到原来运算的位置，得到新的运算字符串
                                std::string IndexMess = mess.substr(0, i - frontPosition + 1);
                                std::string IndexResult = "";
                                // 判断是否运算到最后的结果了
                                if (IndexMess.find('+') == -1
                                        && IndexMess.find('*') == -1
                                        && IndexMess.find('/') == -1
                                        && IndexMess.find('-') == -1)
                                        IndexMess = "";
                                if (IndexMess != "")
                                        IndexResult = IndexMess.find_last_of('-') == IndexMess.length() - 1 ? IndexMess.substr(0, i - frontPosition) : IndexMess;
                                // 组装新的运算字符串
                                mess = IndexResult
                                        + reslutString(stringOperation[i], num1, num2)
                                        + mess.substr(i + backPosition + 1);
 
                                if (mess.find_last_of('-') == 0 
                                        && mess.find('+') == -1
                                        && mess.find('*') == -1
                                        && mess.find('/') == -1) {
                                        std::string temp = "=" + mess ;
                                        std::cout << temp << std::endl;                                        break;
                                }
 
                                //判断传入不带括号之前的式子是否带括号，如果带括号需要在输出步骤时加上
                                if (hasChar) {
 
                                        //判断是否运算符，如果有，需要带括号；没有，则不需要
                                        if (operationNumber.find("+") > 0 || operationNumber.find("-") > 0 || operationNumber.find("/") > 0 || operationNumber.find("*") > 0) {
                                                //输出需要带括号的
                                                std::string temp = "=" + forward + "(" + mess + ")" + back;
                                                std::cout << temp << std::endl;
                                        }
                                        else {
                                                //输出不需要带括号
                                                std::string temp = "=" + forward + mess + back;
                                                std::cout << temp << std::endl;
                                        }
 
                                }
                                else {
                                        //原本的式子不带括号，直接输出
                                        std::string temp = "=" + mess;
                                        std::cout << temp << std::endl;
                                }
 
 
                                // 回调，继续运算
                                return interceptOperation(mess, a, b, hasChar);
                        }
                        else
                                continue;
                }
                if (i == strlen(stringOperation) - 1) {
                        // 递归出口，判断是否还有运算字符串在
                        if (mess.find('+') != -1 || mess.find('-') != -1) {
 
                                return interceptOperation(mess, '+', '-', hasChar);
 
                        }
                        break;
 
 
                }
        }
        return mess;
}
 
// 判断是否有带括号的运算字符串存在
std::string interceResult(std::string str) {
        if (str.empty()) {
                return "";
        }
        std::string result = str;
        char numberString[1000] = {};
        strcpy_s(numberString, str.c_str());;
        int IndexStart = 0; // 记录“（”的实际数量
        int EndStart = 0; // 记录“）”的实际数量
        for (unsigned int i = 0; i < strlen(numberString); i++) {
                if ('(' == numberString[i]) {
                        // 记录最后一个正括号的位置
                        IndexStart = i;
                }
                if (')' == numberString[i]) {
                        // 记录反括号的最初始下标的位置
                        EndStart = i;
                        // 截取最里面一个括号里的运算字符串
                        result = result.substr(IndexStart + 1, EndStart - IndexStart - 1);
 
                        //截取左括号左边的字符
                        forward = str.substr(0, IndexStart);
                        //截取右括号右边的字符
                        back = str.substr(EndStart + 1);
 
                        // 截取括号的运算字符串进行运算,生成新的运算字符串
                        result = str.substr(0, IndexStart)
                                + interceptOperation(result, '*', '/', true)
                                + str.substr(EndStart + 1);
 
                        // 回调执行，其它小括号的运算字符串
                        return interceResult(result);
                }
                if (i == strlen(numberString) - 1)
                        if (EndStart == 0)
                                break;
        }
        // 不存在括号了，再进行混合运算
        result = interceptOperation(str, '*', '/', false);
        return result;
}
 
int main()
{
        while (1) {
                std::string b;
                printf_s("请输入你要计算的字符串(输入完毕后,直接回车):\n");// 1 + 2 * (2 + 2 * 3 / 2) * 2 + 3
                getline(std::cin, b);
                b = interceResult(subreplace(subreplace(subreplace(b, "（", "("), "）", ")"), " ", ""));
                std::cout << "计算结果为："+b << std::endl;        }
 
}
