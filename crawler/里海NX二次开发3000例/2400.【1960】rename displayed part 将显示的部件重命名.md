### 【1960】rename displayed part 将显示的部件重命名

#### 代码

```cpp
    /*HEAD RENAME_DISPLAYED_PART CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
            part = UF_PART_ask_display_part(),  
            other = part;  
        UF_CALL(UF_PART_rename(part, "/tmp/4015489.prt"));  
    /*  Work around to update ANT and Displayed Part in Graphics window 里海译:更新ANT和图形窗口中的显示部件的解决方法 */  
        for (ii = 0; ((ii < UF_PART_ask_num_parts()) && (other == part)); ii++)  
            other = UF_PART_ask_nth_part(ii);  
        UF_CALL(UF_PART_set_display_part(other));  
        UF_CALL(UF_PART_set_display_part(part));  
    /*  End work around 里海译:End work around 翻译为：结束工作绕行。 */  
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

> 这段代码是NX Open C++的一个二次开发示例，主要功能是重命名当前显示的部件并更新图形窗口中的显示部件。
>
> 代码主要流程如下：
>
> 1. 包含必要的头文件，定义了UF_CALL宏来调用UF函数并报告错误。
> 2. 定义了report_error函数，用于输出错误信息。
> 3. 定义了do_it函数，该函数首先获取当前显示的部件，然后调用UF_PART_rename函数对其进行重命名。由于重命名后，NX不会自动更新图形窗口中的显示部件，因此通过遍历所有部件，找到下一个部件，先设置为显示部件，然后再将当前部件设置为显示部件，以刷新图形窗口。
> 4. 定义了ufusr函数，该函数初始化UF，调用do_it函数执行重命名和更新操作，然后终止UF。
> 5. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了NX部件重命名的同时，也解决了图形窗口显示部件更新的问题。
>
