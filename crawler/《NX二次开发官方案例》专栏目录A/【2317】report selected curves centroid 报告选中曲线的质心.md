### 【2317】report selected curves centroid 报告选中曲线的质心

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #include <uf_object_types.h>  
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
    /* qq3123197280 */  
    static int mask_for_curves_and_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve or edge", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves_and_edges, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    /*  
        The function UF_CURVE_ask_centroid does not work correctly.  See PR  
        6551040.  This function can be used as a work around.  
        The source for GRIP program ask_centroid.grs looks like this:  
            ENTITY/ curve, temp_b  
            NUMBER/ a_res(32), xbar, ybar  
            UFARGS/ curve, xbar, ybar  
            temp_b = BOUND/curve  
            ANLSIS/TWOD, temp_b, INCHES, a_res  
            xbar = a_res(3)  
            ybar = a_res(4)  
            DELETE/temp_b  
            HALT  
        Put the ask_centroid.grx in any application folder, e.g.  
        UGII_USER_DIR\application\ask_centroid.grx  
    */  
    static void ask_curve_centroid(tag_t curve, double twod_centroid[2])  
    {  
        UF_args_t  
            args[3];  
        char  
            *path = NULL;  
        args[0].type = UF_TYPE_TAG_T;  
        args[0].length = 0;  
        args[0].address = &curve;  
        args[1].type = UF_TYPE_DOUBLE;  
        args[1].length = 0;  
        args[1].address = &twod_centroid[0];  
        args[2].type = UF_TYPE_DOUBLE;  
        args[2].length = 0;  
        args[2].address = &twod_centroid[1];  
        UF_CALL(UF_find_file("application", "ask_centroid.grx", &path));  
        if (path)  
        {  
            UF_CALL(UF_call_grip(path, 3, args));  
            UF_free(path);  
        }  
        else  
        {  
            ECHO("Cannot find application\ask_centroid.grx\n");  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            curve;  
        double  
            twod_centroid[2],  
            centroid[3];  
        while ((curve = select_a_curve_or_edge("Report Centroid")) != NULL_TAG)  
        {  
            UF_CALL(UF_CURVE_ask_centroid(curve, centroid));  
            ECHO("UF_CURVE_ask_centroid returned ");  
            WRITE3F(centroid);  
            ask_curve_centroid(curve, twod_centroid);  
            ECHO("\nANALSIS/TWOD returned ");  
            WRITE2F(twod_centroid);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用NX API时报告错误信息。
> 2. 定义了一个选择曲线或边的函数select_a_curve_or_edge，它使用NX的选择对话框让用户选择一个曲线或边，并返回选择的实体标签。
> 3. 定义了一个输出数组到列表窗口的函数write_double_array_to_listing_window，用于输出数组变量到NX的列表窗口。
> 4. 定义了一个获取曲线质心的函数ask_curve_centroid，通过调用GRIP脚本来实现，因为UF_CURVE_ask_centroid API存在bug。
> 5. 主函数do_it会循环调用select_a_curve_or_edge，获取用户选择的曲线或边，并使用UF_CURVE_ask_centroid和ask_curve_centroid两个函数分别获取质心，并输出到列表窗口。
> 6. 定义了ufusr入口函数，在NX启动时调用do_it函数。
> 7. 定义了ufusr_ask_unload函数，在NX卸载时返回立即卸载标志。
>
> 总体来说，这段代码实现了通过用户交互选择曲线或边，并获取其质心的功能，同时考虑了NX API存在的bug，通过调用GRIP脚本进行了规避。
>
