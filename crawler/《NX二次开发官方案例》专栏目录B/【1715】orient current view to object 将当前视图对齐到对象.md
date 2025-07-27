### 【1715】orient current view to object 将当前视图对齐到对象

#### 代码

```cpp
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void orient_current_view_to_object(tag_t object)  
    {  
        WRITENZ(uc6434("", 3, object, NULL));  
    }

```

#### 代码解析

> 这段代码是NX二次开发代码，其主要功能包括：
>
> 1. 定义了一个宏WRITENZ，用于将整型变量名和值输出到NX的列表窗口。当变量X非0时，调用write_integer_to_listing_window函数输出。
> 2. write_integer_to_listing_window函数用于将变量名和值格式化成字符串，并输出到NX的列表窗口。
> 3. orient_current_view_to_object函数用于将当前视角对齐到指定对象。这里使用了WRITENZ宏，将函数返回值输出到列表窗口。
> 4. uc6434函数是一个NX的UF函数，用于设置当前视角。这里作为示例代码。
> 5. UF_UI_open_listing_window和UF_UI_write_listing_window是NX的用户界面函数，用于打开和写入列表窗口。
> 6. MAX_LINE_SIZE是NX定义的一个宏，表示单行最大长度。
> 7. UF_CALL是一个宏，用于调用NX的UF函数，并检查返回值是否成功。
>
> 总体来说，这段代码通过宏封装，实现了将变量输出到NX列表窗口的功能，同时提供了一个示例函数orient_current_view_to_object，展示了如何使用NX的UF函数。
>
