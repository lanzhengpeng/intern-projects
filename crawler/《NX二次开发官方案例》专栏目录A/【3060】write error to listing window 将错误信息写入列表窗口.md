### 【3060】write error to listing window 将错误信息写入列表窗口

#### 代码

```cpp
    #define WRITE_ERROR(X) (write_error_to_listing_window(#X, X))  
    void write_error_to_listing_window(char *title, int error)  
    {  
        char  
            err[133];  
        UF_get_fail_message(error, err);  
        ECHO("%s = %d (%s)\n", title, error, err);  
    }

```

#### 代码解析

> 这段代码定义了一个宏WRITE_ERROR，用于记录和打印错误信息。
>
> 宏WRITE_ERROR接收一个错误码作为参数，并调用write_error_to_listing_window函数来记录错误信息。
>
> 在write_error_to_listing_window函数中，使用UF_get_fail_message函数获取错误码对应的错误描述，并将错误码和描述打印到列表窗口。
>
> 因此，这段代码的主要功能是封装错误记录和打印功能，方便在代码中插入错误处理逻辑，以便跟踪和调试问题。
>
> 具体来说，代码首先定义了一个宏WRITE_ERROR，该宏接收一个错误码X作为参数。当宏被调用时，它会将错误码X和对应的错误描述打印到列表窗口。
>
> 在write_error_to_listing_window函数中，首先定义了一个长度为133的字符数组err，用于存储错误描述。然后，调用UF_get_fail_message函数，根据传入的错误码X获取对应的错误描述，并将其存储在err数组中。
>
> 最后，使用ECHO宏打印错误码X、错误描述err和传入的标题title。这样，就可以在代码中通过调用WRITE_ERROR(X)来记录和打印错误信息了。
>
> 这段代码的主要作用是提供了一种方便的错误记录和打印方式，便于在程序中插入错误处理逻辑，以跟踪和调试问题。
>
