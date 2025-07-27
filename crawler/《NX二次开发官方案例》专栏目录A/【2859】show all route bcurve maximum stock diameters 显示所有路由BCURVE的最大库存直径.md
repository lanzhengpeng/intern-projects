### 【2859】show all route bcurve maximum stock diameters 显示所有路由BCURVE的最大库存直径

#### 代码

```cpp
    /*HEAD SHOW_ALL_ROUTE_BCURVE_MAXIMUM_STOCK_DIAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_route.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的函数。该函数的中文翻译为：UF_print_syslog。 */  
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
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void flag_object(tag_t object, char *msg)  
    {  
        double  
            loc[3];  
        if (UF_OBJ_ask_name_origin(object, loc))  
        {  
            UF_CALL(UF_OBJ_set_name(object, "X"));  
            UF_CALL(UF_OBJ_ask_name_origin(object, loc));  
            UF_CALL(UF_OBJ_delete_name(object));  
        }  
        display_temporary_text(msg, loc);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_segments,  
            n_stock;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *segments,  
            *stock;  
        double  
            diameter;  
        char  
            max_dia[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ROUTE_ask_part_segs(part, &n_segments, &segments));  
        for (ii = 0; ii < n_segments; ii++)  
        {  
            UF_CALL(UF_ROUTE_ask_segment_stock(segments[ii], &n_stock, &stock));  
            if (n_stock == 0) continue;  
            UF_CALL(UF_ROUTE_ask_stock_diameter(stock[0], &diameter));  
            sprintf(max_dia, "%.3f", diameter);  
            flag_object(segments[ii], max_dia);  
            UF_free(stock);  
        }  
        if (n_segments > 0) UF_free(segments);  
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

> 这段代码是NX的二次开发代码，其主要功能是显示当前零件中所有BCurve的最大库存直径，并在每个BCurve上显示其最大直径的值。具体代码如下：
>
> 1. 包含必要的NX API头文件，定义了UF_CALL宏用于错误检查，以及report_error函数用于打印错误信息。
> 2. 定义了display_temporary_text函数，用于在指定位置显示临时文本。
> 3. 定义了flag_object函数，用于给指定对象打标记，并显示文本信息。
> 4. 定义了do_it函数，其中：获取当前零件的所有BCurve段遍历每个BCurve段，获取其关联的库存信息计算每个BCurve段的最大直径在BCurve段上显示最大直径的值
> 5. 获取当前零件的所有BCurve段
> 6. 遍历每个BCurve段，获取其关联的库存信息
> 7. 计算每个BCurve段的最大直径
> 8. 在BCurve段上显示最大直径的值
> 9. ufusr函数是NX二次开发的入口函数，调用do_it函数执行主要功能，并在结束时释放资源。
> 10. ufusr_ask_unload函数用于卸载二次开发库。
>
> 总体而言，这段代码实现了在NX中显示所有BCurve的最大库存直径的功能，为用户提供了直观的参数信息，提高了工作效率。
>
