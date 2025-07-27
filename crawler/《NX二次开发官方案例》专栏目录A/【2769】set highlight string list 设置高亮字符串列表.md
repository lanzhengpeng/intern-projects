### 【2769】set highlight string list 设置高亮字符串列表

#### 代码

```cpp
    static void set_highlight_string_list(UF_STRING_t *string, int flag)  
    {  
        int ii, jj, kk = 0;  
        for (ii = 0; ii < string->num; ii++)  
            for (jj = 0; jj < string->string[ii]; jj++, kk++)  
                UF_CALL(UF_DISP_set_highlight(string->id[kk], flag));  
    }

```

#### 代码解析

> 这段代码是用于设置NX中字符串高亮的二次开发函数。
>
> 主要功能是遍历一个结构体数组，该数组存储了多个字符串的ID和长度。然后，根据传入的标志位，调用UF_DISP_set_highlight函数，对每个字符串设置高亮或取消高亮。
>
> 具体步骤如下：
>
> 1. 定义结构体UF_STRING_t，用于存储字符串的ID和长度。
> 2. 定义函数set_highlight_string_list，用于设置字符串高亮。
> 3. 在函数中，通过两层循环遍历结构体数组，得到每个字符串的ID和长度。
> 4. 根据字符串长度，计算出字符串的ID在数组中的位置。
> 5. 调用UF_DISP_set_highlight函数，根据传入的标志位，设置当前字符串ID的高亮或取消高亮。
> 6. 循环结束后，所有字符串的高亮状态都会根据传入的标志位进行设置。
> 7. UF_DISP_set_highlight函数是NX提供的显示模块的函数，用于设置高亮状态。
> 8. 通过设置标志位，可以控制是设置高亮还是取消高亮。
>
> 这段代码的主要作用是在NX中实现字符串的高亮显示功能。
>
