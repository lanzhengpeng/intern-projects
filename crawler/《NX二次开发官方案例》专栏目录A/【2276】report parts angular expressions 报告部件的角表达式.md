### 【2276】report parts angular expressions 报告部件的角表达式

#### 代码

```cpp
    /*HEAD REPORT_PARTS_ANGULAR_EXPRESSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            number_of_exps;  
        tag_t  
            *exps;  
        char  
            *descriptor,  
            *expression;  
        UF_CALL(UF_MODL_ask_exps_of_part(UF_ASSEM_ask_work_part(),  
            &number_of_exps, &exps));  
        for (ii = 0; ii < number_of_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_descriptor_of_exp(exps[ii], &descriptor));  
            if (descriptor != NULL)  
            {  
                if (strstr(descriptor, "Angle"))  
                {  
                    UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &expression));  
                    WRITE(expression);  
                    WRITE(" - ");  
                    WRITE(descriptor);  
                    WRITE("\n");  
                    UF_free(expression);  
                }  
                UF_free(descriptor);  
            }  
        }  
        if (number_of_exps > 0) UF_free(exps);  
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

> 这段代码是一个NX的二次开发示例，主要功能是获取当前工作部件的所有表达式，并筛选出其中的角度表达式，然后将其打印到日志窗口中。
>
> 主要步骤包括：
>
> 1. 定义了一个报告错误的宏UF_CALL，用于在调用NX API函数时检测错误，并在发生错误时打印错误信息。
> 2. 定义了一个do_it函数，用于获取当前工作部件的所有表达式，并筛选出角度表达式，打印到日志窗口。
> 3. 在ufusr函数中，初始化NX系统，调用do_it函数，打印出所有角度表达式，然后终止NX系统。
> 4. 定义了一个ufusr_ask_unload函数，返回立即卸载标志，表示在卸载用户函数时不需要等待。
> 5. 通过宏WRITE简化了打开日志窗口和写入日志窗口的代码。
> 6. 代码中使用了NX的UF模块和UI模块的API，实现了获取表达式、获取描述符、获取表达式字符串等功能。
> 7. 代码风格良好，使用了宏简化代码，错误处理完善，是一个典型的NX二次开发示例。
>
