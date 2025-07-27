### 【0520】convert grip entity to number 将GRIP实体转换为数字

#### 代码

```cpp
    /*HEAD CONVERT_GRIP_ENTITY_TO_NUMBER CCC UFUN */  
    /*  Here is a sample corresponding GRIP program  
            ENTITY/ obj  
            NUMBER/ n  
            GRARGS/ obj, n  
    A10:    IDENT/ 'Select an object', obj, resp  
            IFTHEN/ resp > 2  
                XSPAWN/ UFUN, '/full/path/to/this_library'  
                print/ 'The object tag is ' + ISTR(n)  
                JUMP/ A10:  
            ENDIF  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog在V18版本中是新增的。只提供译文，不要添加任何废话。 */  
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
    static void do_it(void)  
    {  
        tag_t  
            obj;  
        double  
            n;  
        UF_args_t  
            args[2];  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &obj;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &n;  
        UF_CALL(UF_ask_grip_args(2, args));  
        n = (double)obj;  
        UF_CALL(UF_set_grip_args(2, args));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发代码示例，其主要功能是将一个选择的对象转换为数字。
>
> 代码流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用出错时打印错误信息。
> 2. 定义了一个执行函数do_it，用于获取GRIP传入的两个参数（对象tag和数字），将对象tag转换为数字，并设置GRIP的返回参数。
> 3. 定义了UF函数ufusr，在初始化后调用do_it函数，并在结束时进行终止。
> 4. 定义了卸载函数ufusr_ask_unload，返回立即卸载。
> 5. 使用了UF函数UF_ask_grip_args和UF_set_grip_args来获取和设置GRIP的参数。
> 6. 使用了宏UF_CALL来包装UF函数调用，以便进行错误处理。
> 7. 在注释中提供了一个对应的GRIP程序，说明了该函数的调用方式。
> 8. 使用了C语言标准库函数如stdio.h、string.h等进行字符串和文件操作。
> 9. 引入了UF和UF_UI的头文件，以使用NX提供的API函数。
>
> 总体来说，这段代码实现了将GRIP传入的对象tag转换为数字的功能，并通过UF函数与GRIP进行交互。代码结构清晰，注释详细，是一个典型的NX二次开发函数示例。
>
