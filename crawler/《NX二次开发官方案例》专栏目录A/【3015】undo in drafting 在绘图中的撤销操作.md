### 【3015】undo in drafting 在绘图中的撤销操作

#### 代码

```cpp
    /*HEAD UNDO_IN_DRAFTING CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_undo.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
            mark_id;  
        double  
            origin[3] = { 6., 6., 0. };  
        tag_t  
            the_note = NULL_TAG;  
        char  
            *text1[1] = { "This note is for testing UNDO in drafting." };  
        UF_CALL(UF_UNDO_set_mark (UF_UNDO_invisible, NULL, &mark_id));  
        UF_CALL(UF_DRF_create_note(1, text1, origin, 0, &the_note));  
        uc1601("Before Undo", TRUE);  
        UF_CALL(UF_UNDO_undo_to_mark(mark_id, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark_id, NULL));  
        UF_CALL(UF_DISP_regenerate_display());  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是段NX的二次开发代码，主要功能是在NX草绘模块中创建一个注释并撤销这个操作。
>
> 主要步骤包括：
>
> 1. 包含必要的NX头文件，如uf.h, uf_ui.h, uf_undo.h, uf_drf.h。
> 2. 定义一个错误报告函数report_error，用于输出错误信息。
> 3. 定义一个do_it函数，用于执行创建注释并撤销的操作。
> 4. 在do_it函数中，首先设置一个撤销标记点。
> 5. 然后使用UF_DRF_create_note函数在指定位置创建一个注释。
> 6. 输出提示信息，表明撤销前的状态。
> 7. 使用UF_UNDO_undo_to_mark函数撤销到之前设置的标记点。
> 8. 删除标记点。
> 9. 重新生成显示。
> 10. 在ufusr主函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 11. 定义ufusr_ask_unload函数，以便在卸载时立即卸载。
>
> 代码整体逻辑清晰，通过使用NX的撤销功能，实现了在草绘模块中创建注释并撤销注释的功能。
>
