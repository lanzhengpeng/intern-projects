### 【3073】write tag array to listing window 将标签数组输出到列表窗口

#### 代码

```cpp
    #define WRITEnT(n,X) (write_tag_array_to_listing_window(#X, X, 1, n))  
    static void write_tag_array_to_listing_window(char *name, tag_t *array,  
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

> 这段代码是用于在NX的列表窗口中打印数组数据的函数。代码的主要功能包括：
>
> 1. 定义了一个宏WRITEnT，用于打印指定行数和列数的数组数据。它将数组名、数组指针、行数和列数作为参数，并调用write_tag_array_to_listing_window函数进行打印。
> 2. write_tag_array_to_listing_window函数实现了在列表窗口中打印数组数据的功能。它首先计算每行数据前的空格数，然后逐行遍历数组，打印出每行数据，每列数据之间用逗号分隔。
> 3. 在打印每行数据前，如果列数大于0，会先打印数组名或空格，以对齐数据。打印完每行数据后，会打印换行符。
> 4. 通过参数化宏和函数，这段代码提供了灵活的打印数组数据的工具，便于在NX的二次开发中使用。
>
