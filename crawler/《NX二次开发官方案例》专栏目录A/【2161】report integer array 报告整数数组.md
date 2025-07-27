### 【2161】report integer array 报告整数数组

#### 代码

```cpp
    static void report_integer_array(char *name, int *array, int rows, int cols)  
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

> 这段代码定义了一个静态函数 report_integer_array，用于打印二维整数数组。
>
> 函数的主要参数包括：
>
> 函数的实现逻辑如下：
>
> 1. 首先打印数组名称作为标题。
> 2. 通过两层循环遍历数组的每个元素：外层循环遍历行，从0到rows-1。内层循环遍历列，从0到cols-1。
> 3. 外层循环遍历行，从0到rows-1。
> 4. 内层循环遍历列，从0到cols-1。
> 5. 每打印一个元素后，如果是同一行的元素，则在元素间添加逗号和空格分隔。
> 6. 每打印完一行后，换行继续打印下一行。
> 7. 使用变量kk作为数组下标，每次打印一个元素后递增。
>
> 总之，这个函数用于格式化打印二维整数数组，每行以制表符开始，每列以逗号分隔，并以换行符结束。
>
