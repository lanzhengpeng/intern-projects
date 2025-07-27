### 【2955】test external execution 测试外部执行

#### 代码

```cpp
    /*HEAD TEST_EXTERNAL_EXECUTION CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，关于UF_print_syslog的新特性可以翻译为：

UF_print_syslog是V18版本中的新增功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            WRITE("It works!\n");  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open的二次开发代码示例，主要用于测试NX的UF初始化和打印功能。其主要功能和特点包括：
>
> 1. 引入必要的头文件，如stdio.h、uf.h和uf_ui.h，以确保使用NX提供的接口函数。
> 2. 定义UF_CALL宏，用于封装UF函数调用，并在调用失败时打印错误信息。其中，report_error函数用于获取UF函数返回的错误码，并打印出错误信息。
> 3. 定义WRITE宏，用于打开NX的列表窗口并写入字符串。
> 4. 在main函数中，使用UF_CALL宏调用UF_initialize()函数来初始化NX，并检查返回码。如果初始化成功，则调用WRITE宏在列表窗口打印"It works!"字符串。
> 5. 最后，使用UF_CALL宏调用UF_terminate()函数来终止NX。
> 6. 整个代码结构清晰，通过宏封装简化了UF函数的调用，并在出错时提供了友好的错误提示。这段代码可以作为二次开发的基础示例，方便开发者在此基础上进行扩展和修改。
>
