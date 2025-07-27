### 【2504】report tag array 报告标签数组

#### 代码

```cpp
    static void report_tag_array(char *name, tag_t *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%d", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }

```

#### 代码解析

> 这段代码定义了一个名为report_tag_array的静态函数，用于打印一个二维数组。
>
> 函数接受5个参数：
>
> 1. name: 字符串，表示数组名称，用于打印输出。
> 2. array: 指向tag_t类型的指针，表示要打印的二维数组的首地址。
> 3. rows: int类型，表示二维数组的行数。
> 4. cols: int类型，表示二维数组的列数。
> 5. kk: int类型，用于遍历数组元素的计数器。
>
> 函数首先打印数组名称name，然后通过双重循环遍历二维数组array，逐行逐列打印数组元素，元素间以逗号和空格分隔。每打印完一行后换行。
>
> 该函数的作用是输出一个二维数组的元素，适用于打印NX系统中的标签数组。
>
