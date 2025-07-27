### 【3059】write double array to listing window 向列表窗口写入双精度数组

#### 代码

```cpp
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%.3f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发代码，用于将一个双精度浮点数数组打印到NX的日志窗口中。其中定义了几个宏：
>
> 这些宏都调用了write_double_array_to_listing_window函数，该函数接受数组的名字、数组指针、行数和列数。函数内部通过两层循环遍历数组，并使用ECHO宏将数组元素打印到NX的日志窗口中。每行打印前会打印数组名字，后续行打印空白字符。每个元素之间用逗号分隔，每打印一个元素就使用ECHO宏输出。行尾打印换行符。
>
> 这段代码实现了在NX日志窗口中格式化打印浮点数数组的通用功能，通过宏封装简化了调用。
>
