### 【1530】list files in directory matching template 列出目录中与模板匹配的文件

#### 代码

```cpp
    /*HEAD LIST_FILES_IN_DIRECTORY_MATCHING_TEMPLATE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            cnt,  
            resp;  
        char  
            err[133],  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            filter[MAX_FSPEC_SIZE+1] = { "*.prt" },  
            fspec[MAX_FSPEC_SIZE+1],  
            msg[133];  
        while (prompt_for_text("List parts*Enter directory path", dirspec) &&  
               prompt_for_text("List parts*Enter wildcard template", filter))  
        {  
            cnt = 0;  
            UF_UI_open_listing_window();  
            uc4508(dirspec, (1<<11)|(1<<13), 0, filter);  
            while ((resp = uc4518()) <= 0)  
            {  
                UF_CALL(uc4519(fspec));  
                if (resp < 0)  
                {  
                    UF_get_fail_message(resp, err);  
                    sprintf(msg, "\n%s - %s\n", fspec, err);  
                    UF_UI_write_listing_window(msg);  
                    continue;  
                }  
                cnt++;  
                sprintf(msg, "%s\n", fspec);  
                UF_UI_write_listing_window(msg);  
            }  
            UF_CALL(uc4548());  
            sprintf(msg,"\n%d files found in %s matching %s\n",cnt,dirspec,filter);  
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

> 这段NX二次开发代码的主要功能是列出指定目录下匹配指定模板的所有文件，并将结果输出到NX的列表窗口中。
>
> 代码的主要流程如下：
>
> 1. 初始化NX环境，调用UF_initialize()。
> 2. 在主循环中，提示用户输入目录路径和文件模板，然后调用NX的文件系统函数uc4508()来获取匹配的文件列表。
> 3. 使用uc4518()和uc4519()循环获取文件名，直到获取完为止。对于每个文件，调用uc4519()获取文件名，如果成功，则将文件名输出到列表窗口，并更新文件计数；如果失败，则记录错误信息。
> 4. 循环结束后，调用uc4548()来关闭文件列表，然后输出匹配的文件总数。
> 5. 继续循环，直到用户不再输入目录和模板。
> 6. 退出主循环后，调用UF_terminate()来清理NX环境。
> 7. 定义ufusr()函数作为NX的回调函数，当NX加载该代码时，会调用ufusr()。
> 8. 定义ufusr_ask_unload()函数，返回UF_UNLOAD_IMMEDIATELY表示该代码可以被NX立即卸载。
>
> 总体来说，这段代码实现了基本的目录文件列表功能，并提供了简单的错误处理。
>
