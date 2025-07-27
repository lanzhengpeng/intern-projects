### 【2132】report first empty layer 报告首个空图层

#### 代码

```cpp
    /*HEAD REPORT_FIRST_EMPTY_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的。翻译如下：

UF_print_syslog是V18版本新增的。 */  
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
    static int ask_first_empty_layer(void)  
    {  
        tag_t  
            obj;  
        int  
            layer;  
        for (layer = 1; layer <= UF_LAYER_MAX_LAYER; layer++)  
        {  
            obj = NULL_TAG;  
            UF_CALL(UF_LAYER_cycle_by_layer( layer, &obj));  
            if(obj == NULL_TAG)  
            {   
                return layer;  
            }  
        }  
        return 0;  
    }  
    static void do_it(void)  
    {  
        int  
            first_empty = 0;  
        char  
            msg[133];  
        first_empty = ask_first_empty_layer();  
        if(first_empty != 0)  
        {  
            sprintf(msg, "The first empty layer is: %d\n", first_empty);  
        }  
        else  
        {  
            sprintf(msg, "No empty layers found\n", first_empty);  
        }  
        if (!UF_UI_open_listing_window())  
        {  
            UF_UI_write_listing_window(msg);  
        }  
    }  
    /* qq3123197280 */  
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

> 根据代码内容，这是一段NX Open C++的二次开发代码，其主要功能是遍历NX的所有层，找到第一个为空的层并打印其编号。以下是代码的主要功能和流程：
>
> 1. 代码结构和头文件：包含了标准C语言头文件、UF框架头文件、UF UI头文件和UF层处理头文件。定义了UF_CALL宏，用于封装UF函数调用，并在出错时打印错误信息。定义了report_error函数，用于打印UF函数调用的错误信息。定义了ask_first_empty_layer函数，用于遍历层并找到第一个为空的层。定义了do_it函数，用于调用ask_first_empty_layer函数并打印结果。定义了ufusr函数，作为NX二次开发的入口函数。定义了ufusr_ask_unload函数，用于卸载二次开发程序。
> 2. 包含了标准C语言头文件、UF框架头文件、UF UI头文件和UF层处理头文件。
> 3. 定义了UF_CALL宏，用于封装UF函数调用，并在出错时打印错误信息。
> 4. 定义了report_error函数，用于打印UF函数调用的错误信息。
> 5. 定义了ask_first_empty_layer函数，用于遍历层并找到第一个为空的层。
> 6. 定义了do_it函数，用于调用ask_first_empty_layer函数并打印结果。
> 7. 定义了ufusr函数，作为NX二次开发的入口函数。
> 8. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
> 9. 主要逻辑：ufusr函数是程序的入口，首先初始化UF框架，然后调用do_it函数。do_it函数调用ask_first_empty_layer函数，该函数遍历所有层，找到第一个为空的层并返回其编号。如果找到为空的层，do_it函数打印该层的编号；如果所有层都已存在，则打印提示信息。最后，ufusr函数终止UF框架。
> 10. ufusr函数是程序的入口，首先初始化UF框架，然后调用do_it函数。
> 11. do_it函数调用ask_first_empty_layer函数，该函数遍历所有层，找到第一个为空的层并返回其编号。
> 12. 如果找到为空的层，do_it函数打印该层的编号；如果所有层都已存在，则打印提示信息。
> 13. 最后，ufusr函数终止UF框架。
> 14. 错误处理：UF_CALL宏用于封装UF函数调用，并在出错时调用report_error函数打印错误信息。report_error函数打印错误代码、错误消息和调用信息。
> 15. UF_CALL宏用于封装UF函数调用，并在出错时调用report_error函数打印错误信息。
> 16. report_error函数打印错误代码、错误消息和调用信息。
> 17. 用户界面：do_it函数尝试打开NX的Listing窗口，并在窗口中打印结果。如果没有打开窗口，则通过UF UI函数在Listing窗口中打印。
> 18. do_it函数尝试打开NX的Listing窗口，并在窗口中打印结果。如果没有打开窗口，则通过UF UI函数在Listing窗口中打印。
>
> 综上所述，这段代码实现了遍历NX层并找到第一个为空的层的功能，并提供了友好的错误处理和用户界面。
>
