### 【1341】get integer from string end 从字符串尾部获取整数值

#### 代码

```cpp
    static int get_integer_from_string_end(const char *str)  
    {  
        const char  
            *numStr = str + strlen( str );  
        while ( (numStr > str) && (isdigit( numStr[ -1 ] ))) --numStr;  
        return atoi( numStr );  
    }

```

#### 代码解析

> 这段代码的主要功能是从字符串的末尾开始，向前搜索并解析出末尾的整数，并返回这个整数值。
>
> 具体步骤如下：
>
> 1. 通过strlen(str)计算出字符串str的长度，并得到指向字符串末尾的指针numStr。
> 2. 使用循环，从numStr开始向前移动，直到找到第一个非数字字符。在循环中，通过isdigit(numStr[-1])来判断numStr的前一个字符是否为数字。
> 3. 循环结束后，numStr指向字符串中最后一个数字字符的下一个位置，即数字字符串的开始位置。
> 4. 最后，调用atoi(numStr)将numStr指向的数字字符串转换为整型数，并返回该整数值。
>
> 因此，这个函数可以用于从字符串的末尾解析出连续的数字部分，并返回对应的整数值。
>
