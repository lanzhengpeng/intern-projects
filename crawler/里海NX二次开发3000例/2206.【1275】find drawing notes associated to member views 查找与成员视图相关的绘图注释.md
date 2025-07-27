### 【1275】find drawing notes associated to member views 查找与成员视图相关的绘图注释

#### 代码

```cpp
    /*HEAD ASK_ASSOC_NOTES_OF_VIEW CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_ui.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的原文，我将其翻译如下：

注意：UF_print_syslog 是 V18 新增的功能。

在翻译过程中，我遵循了原文的准确性和简洁性。 */  
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
    static tag_t select_a_view(char *prompt)  
    {  
        int  
            view_of_cursor,  
            any_view =0,  
            work_view = 1,  
            resp;  
        double  
            pos[3];  
        char  
            vname[31];  
        tag_t  
            member_view = NULL_TAG;  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( any_view));  
        resp = uc1615(prompt, pos);  
        /* Set view of cursor back to original 里海译:将光标视图恢复到原始状态 */  
        if( view_of_cursor == work_view )  
            UF_CALL(UF_UI_set_cursor_view( work_view));  
        if (resp == 5)  
        {  
            UF_CALL(uc1653(vname));  
            UF_CALL( UF_OBJ_cycle_by_name( vname, &member_view));  
            return member_view;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t   view_tag, *the_notes;  
        int     num_notes, ii;  
        view_tag = select_a_view( "Select view to get notes of...");  
        UF_CALL( UF_DRAW_ask_view_notes( view_tag, &num_notes, &the_notes));  
        printf("\n\nFound %d notes\n", num_notes);  
        for( ii = 0; ii < num_notes; ii++)  
            printf("  %d) Tag of note: %d\n", ii+1, the_notes[ii]);  
        UF_free( the_notes);  
    }  
    /* qq3123197280 */  
    extern void ufusr(char *param, int *retcode, int paramLen)  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息到系统日志和列表窗口。
> 2. 定义了选择视图的函数select_a_view，提示用户选择一个视图，并返回视图的tag。
> 3. 定义了获取指定视图注释的函数do_it，通过调用UF_DRAW_ask_view_notes函数获取视图的注释，并输出注释的数量和tag。
> 4. 定义了主函数ufusr，初始化NX环境后调用do_it函数，完成所有功能。
> 5. 定义了卸载函数ufusr_ask_unload，用于在卸载时立即清理资源。
>
> 综合来看，这段代码实现了选择视图、获取视图注释并输出的功能，为NX的视图管理提供了实用的辅助功能。
>
