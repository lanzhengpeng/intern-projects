### 【1862】remove parameters from selected curves 从选定的曲线上删除参数

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int           
            ii;  
        uf_list_p_t   
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    static void do_it(void)  
    {  
        int  
            count = 0;  
        tag_t  
            *curves = NULL;  
        uf_list_p_t  
            curvesList = NULL;  
        count = select_curves("Pick Curves:", &curves);  
        if(count == 0) return;  
        curvesList = make_a_list(count, curves);  
        UF_CALL(UF_MODL_delete_object_parms(curvesList));  
        UF_MODL_delete_list(&curvesList);  
        UF_free(curves);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于打印信息和错误。
> 2. 定义了一个UF_CALL宏，用于调用NX的UF函数，并打印错误信息。
> 3. 实现了一个mask_for_curves函数，用于设置选择曲线的类型掩码。
> 4. 实现了一个select_curves函数，用于让用户选择曲线，并返回选择的曲线数组。
> 5. 实现了一个make_a_list函数，用于根据给定的tag数组创建一个UF列表。
> 6. 实现了一个do_it函数，用于选择曲线，创建曲线列表，删除曲线的参数，并释放内存。
> 7. ufusr函数是程序的入口点，初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. ufusr_ask_unload函数用于卸载程序。
>
> 该程序的主要流程是：
>
> 1. 用户选择曲线。
> 2. 程序获取选择的曲线，创建列表。
> 3. 程序删除曲线的参数。
> 4. 程序释放内存。
> 5. 程序结束。
>
> 整体上，这是一个简单的NX二次开发示例，实现了选择曲线并删除曲线参数的功能。
>
