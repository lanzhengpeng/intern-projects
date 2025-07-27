### 【1643】modify view bounds 修改视图边界

#### 代码

```cpp
    /*HEAD MODIFY_VIEW_BOUNDS CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。

这个函数的作用是打印系统日志。 */  
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
        tag_t  
            draw_tag,  
            vw_tag;  
        double  
            brdr[4],  
            low_pt[3],  
            up_pt[3];  
        char  
            view_nm[30];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        uc1615("select member view to modify", low_pt);  
        UF_CALL(uc1653(view_nm));  
        UF_CALL(UF_VIEW_ask_tag_of_view_name(view_nm, &vw_tag));  
        UF_CALL(UF_UI_set_cursor_view(1));  
        uc1615("select LOWER diagonal point", low_pt);  
        uc1615("select UPPER diagonal point", up_pt);  
        brdr[0] = low_pt[0];  
        brdr[1] = low_pt[1];  
        brdr[2] = up_pt[0];  
        brdr[3] = up_pt[1];  
        UF_CALL(UF_DRAW_define_view_manual_rect(vw_tag, brdr));  
        UF_CALL(UF_DRAW_ask_current_drawing(&draw_tag));  
        UF_CALL(UF_DRAW_update_one_view(draw_tag, vw_tag));  
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

> 这段代码是用于修改NX视图中视图边界的二次开发代码。
>
> 代码主要包括以下几个部分：
>
> 1. 引入所需的NX Open库头文件，如uf.h、uf_view.h等。
> 2. 定义了report_error函数，用于打印错误信息。
> 3. 定义了do_it函数，主要执行以下步骤：选择要修改的视图成员视图，并获取其视图标签vw_tag。选择视图边界下左下角和右上角点，确定边界矩形范围。调用UF_DRAW_define_view_manual_rect函数，根据选择的边界点手动定义vw_tag视图的边界。获取当前绘图标签draw_tag，并调用UF_DRAW_update_one_view更新vw_tag视图的边界。
> 4. 选择要修改的视图成员视图，并获取其视图标签vw_tag。
> 5. 选择视图边界下左下角和右上角点，确定边界矩形范围。
> 6. 调用UF_DRAW_define_view_manual_rect函数，根据选择的边界点手动定义vw_tag视图的边界。
> 7. 获取当前绘图标签draw_tag，并调用UF_DRAW_update_one_view更新vw_tag视图的边界。
> 8. 定义了ufusr函数，用于初始化NX Open，调用do_it函数，然后终止NX Open。
> 9. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总的来说，这段代码实现了根据用户选择手动修改NX视图边界的功能。通过选择视图和边界点，调用NX Open的相关函数，实现了视图边界的动态修改。
>
