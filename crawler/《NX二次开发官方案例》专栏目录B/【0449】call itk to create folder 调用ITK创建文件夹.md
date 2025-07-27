### 【0449】call itk to create folder 调用ITK创建文件夹

#### 代码

```cpp
    /*HEAD CALL_ITK_TO_CREATE_FOLDER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_ugmgr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：UF_print_syslog是在V18中新增的功能。 */  
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
    int main(int argc, char *argv[])  
    {  
        int  
            output_code;  
        char  
            part_name[132],  
            *output_string = NULL;  
        UF_CALL(UF_UGMGR_initialize(argc,(const char**)argv));  
        printf("Enter New Folder Name : ");  
        scanf("%s", part_name);  
        UF_CALL(UF_UGMGR_invoke_pdm_server(1, part_name, &output_code,  
            &output_string));  
        if (output_string != NULL) free(output_string);  
        return 0;  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一段用于NX CAD软件的二次开发示例代码，主要功能是调用NX API来创建一个新文件夹。代码的主要组成部分包括：
>
> 1. 头文件包含：引入了C语言标准库的头文件，以及NX API的头文件uf.h等。
> 2. 错误报告函数：定义了一个report_error函数，用于在调用NX API时捕捉错误，并打印错误信息。
> 3. 主函数：首先调用UF_UGMGR_initialize初始化NX环境，然后提示用户输入文件夹名称，并调用UF_UGMGR_invoke_pdm_server函数创建该文件夹。
> 4. 卸载函数：定义了ufusr_ask_unload函数，用于在卸载用户程序时释放资源。
>
> 这段代码通过简单的交互式操作，展示了如何利用NX的API进行二次开发，实现基本的文件夹创建功能。它为学习NX API提供了一个简单的入门示例。
>
