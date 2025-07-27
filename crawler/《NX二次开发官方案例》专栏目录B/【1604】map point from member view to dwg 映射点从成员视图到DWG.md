### 【1604】map point from member view to dwg 映射点从成员视图到DWG

#### 代码

```cpp
    /*HEAD MAP_POINT_FROM_MEMBER_VIEW_TO_DWG CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以得出以下翻译：

注意：UF_print_syslog在V18中是新增的，请只回答翻译，不要添加任何废话。

翻译：注意：UF_print_syslog是V18新增的，请只回答翻译，不要添加任何废话。 */  
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
            ia2[2] = { 0,0 };  
        tag_t  
            member_view,  
            point;  
        double  
            loc[3],  
            pos[2];  
        char  
            view[MAX_ENTITY_NAME_SIZE + 1];  
        UF_CALL(UF_UI_set_cursor_view(0));  
        while (uc1616("Select point", ia2, 0, loc) == 5)  
        {  
            UF_CALL(uc1653(view));  
            UF_CALL(UF_VIEW_ask_tag_of_view_name(view, &member_view));  
            UF_CALL(UF_VIEW_map_model_to_drawing(member_view, loc, pos));  
            UF_CALL(UF_CURVE_create_point(pos, &point));  
        }  
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

> 根据代码内容，这是一段用于NX（Siemens NX）的二次开发代码，其主要功能是在NX中创建一个新视图，并在该视图中绘制一个点。以下是代码的主要功能介绍：
>
> 1. 包含必要的头文件，如uf.h、uf_ui.h等。
> 2. 定义了一个错误报告函数report_error，用于打印出错误信息。
> 3. 定义了主函数do_it，用于实现主要功能：
>
> 1. 定义了ufusr函数作为程序的入口点，初始化后调用do_it函数，完成后进行清理。
> 2. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 综上所述，该代码通过交互方式让用户选择一个点，并在当前视图的对应位置创建一个点。这为NX的二次开发提供了一个简单的示例，展示了如何进行视图操作和点创建。
>
