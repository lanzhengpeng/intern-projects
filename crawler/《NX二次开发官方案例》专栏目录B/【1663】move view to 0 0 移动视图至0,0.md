### 【1663】move view to 0 0 移动视图至0,0

#### 代码

```cpp
    /*HEAD MOVE_VIEW_TO_0_0 CCC UFUN */  
    /* Moves selected drawing view so lower left is at 0,0 里海译:移动选定的绘图视图，使左下角位于0,0。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的功能。该功能用于打印系统日志。 */  
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
        /* Read view of cursor (work or any?) 里海译:游标视图的读取(工作或任何?) */  
        UF_CALL( UF_UI_ask_cursor_view( &view_of_cursor));  
        UF_CALL( UF_UI_set_cursor_view( any_view));  
        ret_code = uc1615(prompt, pos);  
        if( view_of_cursor == work_view )  
        /* Set view of cursor back to original 里海译:将光标视图恢复到原始状态 */  
            UF_CALL(UF_UI_set_cursor_view( work_view));  
        if (ret_code == 5)  
        {  
            /* Return view name of last position indication 里海译:返回上次位置指示的视图名称 */  
            UF_CALL( uc1653( view_name ));  
        }  
        else strcpy(view_name, "");  
    }  
    static void do_it(void)  
    {  
        char   
            view_name[31]="";  
        double   
            ref_pt[2],  
            view_bounds[4];  
        select_a_view("Select view to move to 0, 0",view_name);  
        printf("\n The View Name is: %s\n",view_name);  
        UF_CALL( uc6485(view_name,view_bounds));  
        printf( "Left bound is %f\n",view_bounds[0]);  
        printf( "Bottom bound is %f\n",view_bounds[1]);  
        printf( "Right bound is %f\n",view_bounds[2]);  
        printf( "Top bound is %f\n",view_bounds[3]);  
        ref_pt[0]=((view_bounds[2]-view_bounds[0])/2);  
        ref_pt[1]=((view_bounds[3]-view_bounds[1])/2);  
        UF_CALL( uc6484("SHEET1",view_name,ref_pt));  
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

> 这段代码是用于NX的二次开发，其主要功能是移动选定的绘图视图，使其左下角位于坐标原点(0,0)。以下是代码的主要功能和实现步骤：
>
> 1. 引入NX二次开发所需的头文件。
> 2. 定义宏ECHO和UF_CALL，分别用于打印变量值和调用NX函数，并在调用失败时打印错误信息。
> 3. 定义函数report_error，用于在函数调用失败时报告错误。
> 4. 定义函数select_a_view，用于提示用户选择要移动的视图，并返回视图名称。
> 5. 定义函数do_it，用于执行视图移动操作：调用select_a_view获取要移动的视图名称。获取选定视图的边界坐标。计算视图的中心点坐标。调用NX函数，将视图移动到中心点坐标，使其左下角位于原点。
> 6. 调用select_a_view获取要移动的视图名称。
> 7. 获取选定视图的边界坐标。
> 8. 计算视图的中心点坐标。
> 9. 调用NX函数，将视图移动到中心点坐标，使其左下角位于原点。
> 10. 定义ufusr函数，作为二次开发的入口函数，初始化NX环境，调用do_it执行操作，然后终止NX环境。
> 11. 定义ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 综上，这段代码实现了在NX中移动视图，使其左下角对齐坐标原点的功能，并提供了友好的用户提示和错误处理机制。
>
