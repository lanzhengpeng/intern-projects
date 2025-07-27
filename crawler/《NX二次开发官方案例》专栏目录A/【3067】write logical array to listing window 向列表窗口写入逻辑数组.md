### 【3067】write logical array to listing window 向列表窗口写入逻辑数组

#### 代码

```cpp
    #define WRITEnL(n,X) (write_logical_array_to_listing_window(#X, X, n))  
    void write_logical_array_to_listing_window(char *title, logical *logicals,  
        int n_logicals)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_logicals; ii++)  
        {  
            if (logicals[ii])  
                ECHO("%s[%d] = TRUE\n", title, ii);  
            else  
                ECHO("%s[%d] = FALSE\n", title, ii);  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要用于在NX的Listing窗口中打印一个布尔数组。
>
> 主要功能如下：
>
> 1. 定义了一个宏WRITEnL，用于在Listing窗口中打印布尔数组。宏接收数组名、数组本身和数组长度作为参数。
> 2. 实现了一个函数write_logical_array_to_listing_window，该函数接收数组标题、布尔数组指针和数组长度作为参数。在函数内部，使用for循环遍历数组，通过NX Open C++ API中的ECHO函数，将每个布尔值打印到Listing窗口中。
> 3. 在ECHO函数中，使用%s格式化字符串打印标题，%d打印索引，并根据布尔值打印TRUE或FALSE。索引使用中括号包围，以表示数组元素。
> 4. 使用宏封装函数，简化了在代码中打印布尔数组的操作。只需一行WRITEnL即可完成打印，无需编写循环和格式化字符串。
> 5. 该代码利用了NX Open C++ API提供的函数，实现了在NX环境中打印自定义数据的功能，增强了程序的交互性和可调试性。
>
