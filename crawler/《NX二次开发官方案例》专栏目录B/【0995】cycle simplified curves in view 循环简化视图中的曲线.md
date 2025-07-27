### 【0995】cycle simplified curves in view 循环简化视图中的曲线

#### 代码

```cpp
    /*HEAD CYCLE_SIMPLIFIED_CURVES_IN_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新增功能，用于打印系统日志消息。 */  
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
    static void select_a_view( char *prompt, char *view_name)  
    {  
     int  
         ret_code,  
         any_view = 0,  
         work_view = 1,  
         view_of_cursor;  
     double  
         pos[3];  
     /* Read view of cursor (work or any?) 里海译:游标的工作视图或任何视图的读取视图 */  
     UF_CALL(UF_UI_ask_cursor_view( &view_of_cursor ));  
     if( view_of_cursor == work_view )  
     /* Set view of cursor to any 里海译:将光标视图设置为任意 */  
         UF_CALL(UF_UI_set_cursor_view( any_view ));  
     /* Pick a position within a view 里海译:选择视图内的一个位置 */  
     ret_code = uc1615( prompt, pos);  
     /* Set view of cursor back to original 里海译:将光标视图恢复到原始位置 */  
     if( view_of_cursor == work_view )  
         UF_CALL(UF_UI_set_cursor_view( work_view));  
     if (ret_code == 5)  
     {  
         /* Return view name of last position indication 里海译:返回最后位置指示的视图名称。 */  
         UF_CALL( uc1653( view_name ));  
     }  
     else strcpy(view_name, "");  
    }  
    static void do_it(void)  
    {  
     char                view_nam[30];  
     char                type_str[30];  
     int                 type, sub;  
     tag_t               group_tag;  
     tag_t               object_id = NULL_TAG;  
     int     group_type, group_subtype;  
     select_a_view("Select view", view_nam);  
     while((!UF_CALL( uc6457( view_nam, 2, &object_id))) && object_id != 0)  
     {  
         UF_CALL( UF_OBJ_ask_type_and_subtype( object_id, &type, &sub));  
         UF_CALL( UF_DISP_set_highlight( object_id, 1));  
         sprintf(type_str, "type = %d,  subtype = %d\n", type, sub);  
         uc1601( type_str, 1);  
         UF_CALL( UF_DISP_set_highlight( object_id, 0));  
         if(type == 5)  
         {  
             UF_CALL( UF_DRAW_ask_group_of_curve(  
                      object_id, &group_tag, &group_type, &group_subtype));  
             printf("Group type: %d\t", group_type);  
             printf("\tGroup subtype: %d\n", group_subtype);  
         }  
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

> 这段代码是一个NX的二次开发程序，其主要功能是选择一个视图，然后遍历视图中的所有对象，并对曲线对象进行高亮显示，并打印其类型和子类型。主要步骤包括：
>
> 1. 定义了report_error函数，用于打印错误信息。
> 2. 定义了select_a_view函数，用于提示用户选择一个视图，并获取视图名称。
> 3. 定义了do_it函数，其功能包括：
>
> 1. 定义了ufusr函数，作为程序的入口，首先初始化NX环境，然后调用do_it函数执行主体功能，最后终止NX环境。
> 2. 定义了ufusr_ask_unload函数，用于立即卸载程序。
>
> 总的来说，这段代码实现了选择视图、遍历视图对象、对曲线对象高亮显示和打印信息的功能。
>
