### 【3064】write integer array to listing window 将整数数组写入列表窗口

#### 代码

```cpp
    #define WRITEnD(n,X) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%d", array[kk++]);  
                if (jj < cols - 1)  
                {  
                    ECHO("%s", ", ");  
                }  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }

```

#### 代码解析

> 这段代码是一个用于在NX的列表窗口中打印整数数组的函数。主要功能包括：
>
> 1. 定义了一个宏WRITEnD，用于调用write_integer_array_to_listing_window函数，其中n是数组的大小，X是要打印的数组名。
> 2. write_integer_array_to_listing_window函数接受数组名、数组指针、行数和列数作为参数。
> 3. 打开NX的列表窗口。
> 4. 创建一个全为空格的字符串，长度与数组名加3个空格相同。
> 5. 遍历数组，按行和列打印每个元素。
> 6. 每行的第一个元素前打印数组名，之后打印空格字符串。
> 7. 每列打印一个元素，元素之间用逗号和空格分隔。
> 8. 每打印完一行，输出一个换行符。
> 9. 最终关闭NX的列表窗口。
>
> 总的来说，这个函数通过遍历打印，将一个二维整数数组在NX的列表窗口中以表格形式展现出来，非常直观地展示了数组内容。
>
