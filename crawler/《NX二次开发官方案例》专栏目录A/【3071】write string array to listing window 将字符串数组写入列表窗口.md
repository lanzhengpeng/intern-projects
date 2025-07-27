### 【3071】write string array to listing window 将字符串数组写入列表窗口

#### 代码

```cpp
    #define WRITEnS(n,X) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        for (ii = 0; ii < n_strings; ii++)  
            ECHO("%s[%d] = %s\n", title, ii, strings[ii]);  
    }

```

#### 代码解析

> 这段代码定义了一个宏WRITEnS，用于将一个字符串数组的内容输出到列表窗口，并展示每个元素的索引和值。
>
> 关键点如下：
>
> 1. 宏WRITEnS用于简化调用函数write_string_array_to_listing_window的代码，提供参数n、title和数组名X。
> 2. 函数write_string_array_to_listing_window接受一个标题字符串title、一个字符串数组strings以及数组的大小n_strings作为参数。
> 3. 函数遍历数组中的每个字符串，并使用ECHO宏输出其索引和值，格式为 “%s[%d] = %s\n”，其中%s表示字符串，%d表示整数。
> 4. 通过调用ECHO宏，可以将索引和值输出到列表窗口。
> 5. 使用宏可以减少重复代码，使调用更加简洁。
> 6. 这段代码展示了如何使用宏和函数来实现字符串数组的输出，适用于NX二次开发中。
>
