### 【1716】orient current view to wcs 将当前视图定向到世界坐标系

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
    static void orient_current_view_to_wcs(void)  
    {  
        WRITENZ(uc6434("", 2, NULL_TAG, NULL));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API用于NX二次开发的一部分，主要实现了以下功能：
>
> 1. 定义了WRITENZ宏，用于将整数变量及其值写入NX的Listing窗口，方便调试和输出信息。
> 2. 实现了write_integer_to_listing_window函数，用于将整数变量及其值格式化为字符串，并写入NX的Listing窗口。其中使用了sprintf函数进行格式化。
> 3. 实现了orient_current_view_to_wcs函数，使用WRITENZ宏将调用NX API函数uc6434的返回值写入Listing窗口，该函数的作用是将当前视图定向到WCS坐标系。
> 4. WRITENZ宏和write_integer_to_listing_window函数封装了NX API函数的调用，简化了代码，便于在开发过程中调试输出。
> 5. orient_current_view_to_wcs函数利用NX API实现了视图定向到WCS坐标系的功能，是NX二次开发中的一个常见操作。
> 6. 整体代码简洁清晰，使用了宏和函数封装，提高了代码的可读性和可维护性。
>
