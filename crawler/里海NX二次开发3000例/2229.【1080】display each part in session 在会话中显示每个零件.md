### 【1080】display each part in session 在会话中显示每个零件

#### 代码

```cpp
    /*HEAD DISPLAY_EACH_PART_IN_SESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是在V18版本中新增的，请只回答翻译，不要添加任何废话。 */  
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
        int  
            ii;  
        tag_t  
            part;  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        for (ii = 0; ii < UF_PART_ask_num_parts(); ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            UF_CALL(UF_PART_set_display_part(part));  
            UF_PART_ask_part_name(part, fspec);  
            uc1601(fspec, TRUE);  
        }  
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

> 这段代码是NX Open C++ API编写的NX二次开发代码，主要功能是在NX会话中显示每个部件。代码的逻辑如下：
>
> 1. 引入所需的头文件，包括标准库头文件、NX Open C++ API头文件和用户界面头文件。
> 2. 定义一个错误报告函数report_error，用于在出错时打印错误信息到系统日志和列表窗口。
> 3. 定义一个do_it函数，用于遍历当前会话中的所有部件，并设置每个部件为显示状态，然后获取部件名称并打印。
> 4. 定义ufusr函数，这是NX二次开发的入口函数。在该函数中，首先调用UF_initialize进行初始化，然后调用do_it函数执行显示部件的逻辑，最后调用UF_terminate终止会话。
> 5. 定义ufusr_ask_unload函数，用于设置在卸载用户函数时立即卸载。
>
> 总的来说，这段代码实现了在NX会话中显示每个部件的功能，通过NX Open C++ API对NX进行了二次开发。
>
