### 【0700】create filebox 创建文件对话框

#### 代码

```cpp
    /*HEAD CREATE_FILEBOX CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的功能。 */  
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
            resp;  
        char  
            *dir_name,  
            filename[MAX_FSPEC_SIZE+1],  
            filter_string[MAX_FSPEC_SIZE+1],  
            messg[300];  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &dir_name));  
        strcpy(filter_string, dir_name);  
        UF_free(dir_name);  
        while (!UF_CALL(UF_UI_create_filebox("Select data file", "Data file",  
            filter_string, "", filename, &resp)) && (resp != UF_UI_CANCEL))  
        {  
            sprintf(messg, "Selected %s", filename);  
            uc1601(messg, TRUE);  
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

> 这段代码是一个NX的二次开发示例，主要功能是创建一个文件选择对话框，让用户选择一个数据文件。
>
> 主要步骤包括：
>
> 1. 初始化NX，调用UF_initialize()。
> 2. 调用UF_UI_ask_dialog_directory()获取NX部件库的路径，并拼接成文件过滤器。
> 3. 调用UF_UI_create_filebox()创建文件选择对话框，用户可以在对话框中选择文件。如果用户选择了文件，会在NX日志窗口输出提示信息。
> 4. 重复调用UF_UI_create_filebox()，直到用户点击取消按钮。
> 5. 结束时，调用UF_terminate()关闭NX。
>
> 此外，还定义了report_error()函数，用于在出错时输出错误信息。ufusr()函数是二次开发程序的入口点。ufusr_ask_unload()函数用于处理程序卸载，返回立即卸载。
>
> 整体来看，这是一个简单的文件选择对话框的二次开发示例。
>
