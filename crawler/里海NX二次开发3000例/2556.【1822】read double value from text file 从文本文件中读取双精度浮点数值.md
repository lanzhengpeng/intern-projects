### 【1822】read double value from text file 从文本文件中读取双精度浮点数值

#### 代码

```cpp
    static double read_double_value_from_text_file(char *fspec, char *value_name)  
    {  
        double  
            value = -999;  // default for value not found  
        char  
            a_line[MAX_LINE_SIZE+1],  
            *found;  
        FILE  
            *the_file;  
        if ((the_file = fopen(fspec, "r")) != NULL)  
        {  
            while ((fgets(a_line, MAX_LINE_SIZE, the_file)) != NULL)  
            {  
                found =  strstr(a_line, value_name);  
                if (found == NULL) continue;  
                value = atof(strstr(found, "=")+1);  
                break;  
            }  
            fclose(the_file);  
        }  
        return value;  
    }

```

#### 代码解析

> 这段代码是用于从文本文件中读取双精度浮点数的函数。其主要功能包括：
>
> 1. 函数名为read_double_value_from_text_file，接受两个参数：文件路径fspec和要读取的值的名字value_name。
> 2. 函数内部首先定义了双精度浮点数变量value，并初始化为-999，作为找不到值时的默认返回值。
> 3. 定义了一个字符数组a_line，用于存储从文件中读取的一行文本。
> 4. 定义了一个FILE指针the_file，用于指向打开的文件。
> 5. 使用fopen函数以只读模式打开文件，如果文件打开成功，则进入循环，逐行读取文件内容。
> 6. 在循环中，使用strstr函数查找当前行是否包含要读取的值的名字value_name。如果找到，则使用strstr和atof函数组合截取等号后面的字符串，并转换为双精度浮点数赋值给value。
> 7. 如果找到要读取的值，则跳出循环。循环结束后，关闭文件。
> 8. 最后返回读取到的值value。如果文件打开失败或者未找到要读取的值，则返回默认值-999。
>
> 总的来说，这个函数实现了从指定文件中按名字读取双精度浮点数的功能，具有简单的错误处理和清晰的逻辑结构。
>
