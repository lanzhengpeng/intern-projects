### 【2966】test write access to work part 检查工作零件的写入权限

#### 代码

```cpp
    /*HEAD TEST_WRITE_ACCESS_TO_WORK_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据原文，我翻译如下：

请注意，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical is_part_writeable(tag_t part)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        FILE  
            *test;  
        UF_PART_ask_part_name(part, fspec);  
        test = fopen(fspec, "a");  
        if (test != NULL)  
        {  
            fclose(test);  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            work = UF_ASSEM_ask_work_part();  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(work, fspec);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(fspec);  
        if (is_part_writeable(work))  
        {  
            UF_UI_write_listing_window(" write access OK\n");  
        }  
        else  
            UF_UI_write_listing_window(" NO write access\n");  
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

> 这段代码是NX二次开发代码，主要功能是检查当前工作部件是否具有写访问权限。
>
> 主要步骤包括：
>
> 1. 定义了一个报告错误的函数report_error，用于打印错误信息。
> 2. 定义了一个检查部件是否可写的函数is_part_writeable，通过尝试以追加模式打开部件文件来判断。
> 3. 定义了一个主函数do_it，首先获取当前工作部件，然后调用is_part_writeable函数检查写访问权限，并通过UF_UI接口打印部件名称和写访问权限信息。
> 4. ufusr函数是NX调用的主函数，初始化后调用do_it函数，然后结束NX会话。
> 5. ufusr_ask_unload函数定义了立即卸载的返回值。
>
> 总的来说，这段代码通过检查工作部件文件的写访问权限，帮助用户确认当前工作部件是否可以被修改，从而确保了NX会话的顺利进行。
>
