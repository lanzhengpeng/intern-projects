### 【1006】delete all empty drawings 删除所有空绘图

#### 代码

```cpp
    /*HEAD DELETE_ALL_EMPTY_DRAWINGS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #include <uf_layout.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中的新增功能，用于打印系统日志消息。 */  
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
            ii,  
            n_drawings;  
        tag_t  
            dobj,  
            *drawings,  
            view;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            vname[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings == 0) return;  
        for (ii = 0; ii < n_drawings; ii++)  
        {  
            UF_CALL(UF_DRAW_open_drawing(drawings[ii]));  
            UF_CALL(UF_OBJ_ask_name(drawings[ii], dname));  
            sprintf(vname, "%s@0", dname);  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(vname, &view));  
            dobj = NULL_TAG;  
            UF_CALL(UF_VIEW_cycle_objects(view, UF_VIEW_VISIBLE_OBJECTS, &dobj));  
            if (dobj == NULL_TAG)  
            {  
                uc6468("L1", 1, 0);  
                UF_CALL(UF_OBJ_delete_object(drawings[ii]));  
            }  
        }  
        UF_free(drawings);  
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

> 这段代码是一个NX Open的UF函数，其功能是删除所有空的绘制对象。具体来说，代码的主要功能包括：
>
> 1. 调用UF_DRAW_ask_drawings获取当前会话中的所有绘制对象，并保存到drawings数组中。
> 2. 遍历drawings数组，对每个绘制对象进行操作：调用UF_DRAW_open_drawing打开绘制对象。调用UF_OBJ_ask_name获取绘制对象的名称，并据此构建视图名称。调用UF_VIEW_ask_tag_of_view_name获取视图的tag。调用UF_VIEW_cycle_objects遍历视图中的所有可见对象，如果没有返回任何对象，说明视图为空。如果视图为空，则调用UF_OBJ_delete_object删除该绘制对象。
> 3. 调用UF_DRAW_open_drawing打开绘制对象。
> 4. 调用UF_OBJ_ask_name获取绘制对象的名称，并据此构建视图名称。
> 5. 调用UF_VIEW_ask_tag_of_view_name获取视图的tag。
> 6. 调用UF_VIEW_cycle_objects遍历视图中的所有可见对象，如果没有返回任何对象，说明视图为空。
> 7. 如果视图为空，则调用UF_OBJ_delete_object删除该绘制对象。
> 8. 最后释放drawings数组。
> 9. ufusr函数是NX Open的入口函数，用于初始化和调用do_it函数，并在结束时终止NX Open。
> 10. ufusr_ask_unload函数用于设置NX Open立即卸载。
>
> 综上所述，这段代码的主要功能是遍历当前会话中的所有绘制对象，并删除那些内容为空的绘制对象。这可以提高绘制效率，避免无用的空绘制对象占据内存。
>
