### 【2416】report user defined symbol data 报告用户定义符号数据

#### 代码

```cpp
    /*HEAD REPORT_USER_DEFINED_SYMBOL_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_csys.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据注释，UF_print_syslog是V18版本中新增的功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static tag_t ask_next_user_defined_symbol(tag_t part, tag_t uds)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &uds)) && (uds != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(uds, &type, &subtype));  
            if (subtype == UF_draft_user_defined_subtype) return uds;  
        }  
        return NULL_TAG;  
    }  
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void show_objects_csys(tag_t object, double where[3])  
    {  
        tag_t  
            mx_id;  
        double  
            mx[9];  
        if (!UF_CALL(UF_CSYS_ask_matrix_of_object(object, &mx_id)))  
        {  
            UF_CALL(UF_CSYS_ask_matrix_values(mx_id, mx));  
            display_conehead(where, &mx[0]);  
            display_conehead(where, &mx[3]);  
            display_conehead(where, &mx[6]);  
        }  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_user_defined_symbol_data(tag_t uds)  
    {  
        logical  
            is_sbf;  
        UF_DRF_symbol_data_t  
            symbol_data;  
        UF_CALL(UF_DRF_is_sbf_symbol(uds, &is_sbf));  
        if (is_sbf && !UF_CALL(UF_DRF_ask_symbol_data(uds, &symbol_data)))  
        {  
            UF_CALL(UF_DISP_set_highlight(uds, TRUE));  
            show_objects_csys(uds, symbol_data.anchor_point);  
            WRITE3F(symbol_data.anchor_point);  
            display_temporary_point(symbol_data.orient_point);  
            WRITE3F(symbol_data.orient_point);  
            WRITE_F(symbol_data.angle);  
            WRITE_F(symbol_data.length);  
            WRITE_F(symbol_data.height);  
            WRITE_D(symbol_data.symbol_font_tag);  
            uc1601("OK?", TRUE);  
            UF_CALL(UF_DISP_set_highlight(uds, FALSE));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            uds = NULL_TAG;  
        while ((uds = ask_next_user_defined_symbol(part, uds)) != NULL_TAG)  
            report_user_defined_symbol_data(uds);  
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

> 这段代码是一个NX的二次开发示例，主要功能是遍历当前部件中的所有用户定义符号，并显示它们的相关信息。
>
> 关键功能包括：
>
> 1. 错误报告宏：定义了UF_CALL宏，用于调用NX API函数，并在出错时报告错误信息。
> 2. 写入整数/浮点数到列表窗口：定义了WRITE_D和WRITE_F宏，用于将整数和浮点数写入列表窗口。
> 3. 写入浮点数组到列表窗口：定义了WRITE3F宏，用于写入浮点数组到列表窗口。
> 4. 遍历用户定义符号：定义了ask_next_user_defined_symbol函数，用于遍历当前部件中的所有用户定义符号。
> 5. 显示圆锥头：定义了display_conehead函数，用于显示圆锥头。
> 6. 显示对象坐标系：定义了show_objects_csys函数，用于显示对象坐标系。
> 7. 显示临时点：定义了display_temporary_point函数，用于显示临时点。
> 8. 报告用户定义符号数据：定义了report_user_defined_symbol_data函数，用于获取用户定义符号的相关信息并显示。
> 9. 执行主体逻辑：定义了do_it函数，用于遍历当前部件中的所有用户定义符号，并报告它们的数据。
> 10. ufusr函数：NX二次开发必须实现的函数，用于初始化、执行主体逻辑和终止。
> 11. 卸载函数：定义了ufusr_ask_unload函数，用于卸载用户自定义程序。
>
> 总体来说，这段代码利用NX提供的API函数，实现了对当前部件中所有用户定义符号的遍历，并获取并显示它们的属性信息。这为二次开发提供了一个很好的示例。
>
