### 【2407】report ug version of current part 当前零件的UG版本报告

#### 代码

```cpp
    /*HEAD REPORT_UG_VERSION_OF_CURRENT_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新增功能。文档仅要求提供翻译，不要求其他内容，因此翻译为：

UF_print_syslog是V18版本的新增功能。 */  
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
            part = UF_PART_ask_display_part();  
        char  
            fspec[MAX_FSPEC_SIZE+1],  
            *major,  
            relnum[MAX_LINE_SIZE+1];  
        UF_PART_ask_part_name(part, fspec);  
        if (!uc4560(fspec, 2))  
        {  
            UF_CALL(uc4623(fspec, relnum));  
            printf("The release of %s is %s.\n", fspec, relnum);  
        }  
        else  
        {  
            UF_CALL(UF_get_release(&major));  
            printf("The release of %s is %s.\n", fspec, major);  
            UF_free(major);  
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

> 这段代码是NX的二次开发示例，其主要功能是获取当前显示零件的版本信息，并输出到控制台。
>
> 代码的主要逻辑如下：
>
> 1. 包含NX二次开发所需的头文件，如uf.h、uf_ui.h、uf_part.h等。
> 2. 定义了一个错误报告函数report_error，用于打印出函数执行失败时的错误代码和错误信息。
> 3. 定义了一个主体函数do_it，用于获取当前显示零件的版本信息。
> 4. 在do_it函数中，首先获取当前显示零件的tag，然后获取零件的文件名。
> 5. 判断零件文件名是否以“4560”结尾，如果是，则调用函数uc4623获取零件的版本号并输出；否则，调用UF_get_release获取NX的版本号并输出。
> 6. 定义了ufusr函数作为NX的入口函数，在函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，返回UF_UNLOAD_IMMEDIATELY表示该例程可以立即卸载。
>
> 总的来说，这段代码的主要作用是获取当前显示零件的版本信息，并通过控制台打印出来，同时包含了错误处理和NX环境管理等功能。
>
