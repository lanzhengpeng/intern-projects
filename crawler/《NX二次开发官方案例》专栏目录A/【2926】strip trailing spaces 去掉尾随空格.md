### 【2926】strip trailing spaces 去掉尾随空格

#### 代码

```cpp
    static void strip_trailing_spaces(double n, char *s)  
    {  
        int ii;  
        sprintf(s, "%f", n);  
        for (ii = strlen(s) - 1; s[ii] == ' '; ii--) s[ii] = '\0';  
    }

```

#### 代码解析

> 这段代码定义了一个名为strip_trailing_spaces的静态函数，用于去除字符串末尾的空格。具体步骤如下：
>
> 1. 使用sprintf函数将double类型的数值n格式化为字符串，并存储在字符数组s中。
> 2. 通过调用strlen函数获取字符串s的长度。
> 3. 从字符串s的末尾开始，使用一个for循环逐个字符向前遍历。
> 4. 判断当前字符是否为空格(’ ‘)。如果是空格，则将该字符替换为字符串结束符(’\0’)。
> 5. 循环继续，直到遇到非空格字符为止。
> 6. 最终返回去除末尾空格后的字符串s。
>
> 这个函数的作用是确保输出的字符串末尾不包含多余的空格，使得字符串更加整洁。
>
