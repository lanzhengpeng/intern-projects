### 【2159】report instance origins of gdt index numbers 报告GDT索引号实例的起源

#### 代码

```cpp
    /*HEAD REPORT_INSTANCE_ORIGINS_OF_GDT_INDEX_NUMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_gdt.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中的新功能。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
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
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            UF_CALL(UF_UI_write_listing_window(msg));  
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
            UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static logical is_on_current_drawing(tag_t object)  
    {  
        int  
            status;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            view[MAX_ENTITY_NAME_SIZE+1];  
        uc6492(dname);  
        strcat(dname, "@0");  
        uc6409(object, &status, view);  
        if (!status || strcmp(view, dname)) return FALSE;  
        return TRUE;  
    }  
    #define SAY(X)    (report_an_integer(#X, X))  
    static void report_an_integer(char *variable, int value)  
    {  
        char  
            msg[133];  
        sprintf(msg, "%s = %d\n", variable, value);  
        uc1601(msg, TRUE);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            index = 0;  
        tag_t  
            fcf,  
            *instances,  
            tol_feat;  
        double  
            origin[3];  
        for (index = 0; index < 100; index++)  
        {  
            if (!UF_GDT_ask_tolerance_from_index(index, &tol_feat, &fcf))  
            {  
                WRITE_D(index);  
                if (tol_feat != NULL_TAG)  
                {  
                    WRITE_D(tol_feat);  
                    UF_CALL(UF_GDT_ask_tol_feat_instance(tol_feat, &n, &instances));  
                    for (ii = 0; ii < n; ii++)  
                    {  
                        WRITE_D(ii);  
                        WRITE_D(instances[ii]);  
                        UF_CALL(UF_DRF_ask_origin(instances[ii], origin));  
                        WRITE3F(origin);  
                        if (is_on_current_drawing(instances[ii]))  
                        {  
                            display_temporary_colored_point(origin, UF_OBJ_RED);  
                            UF_CALL(UF_DISP_set_highlight(instances[ii], TRUE));  
                            SAY(index);  
                            UF_CALL(UF_DISP_set_highlight(instances[ii], FALSE));  
                        }  
                    }  
                    if (n > 0) UF_free(instances);  
                }  
                WRITENZ(fcf);  
            }  
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

> 这段代码是一个NX Open的UF示例，其主要功能是遍历当前图纸中的所有公差特征，并获取每个公差特征的实例索引、对象标签、原点坐标等信息，并在当前视图中高亮显示每个公差特征的实例。
>
> 关键功能包括：
>
> 1. 使用UF_GDT_ask_tolerance_from_index函数遍历图纸中的公差特征，并获取每个公差特征的标签。
> 2. 使用UF_GDT_ask_tol_feat_instance函数获取每个公差特征的实例索引和对象标签。
> 3. 使用UF_DRF_ask_origin函数获取每个公差特征实例的原点坐标。
> 4. 使用UF_DISP_display_temporary_point和UF_DISP_set_highlight函数在当前视图中高亮显示每个公差特征实例。
> 5. 定义了宏WRITE_D和WRITE3F用于向日志窗口输出整型和浮点型变量。
> 6. 定义了函数display_temporary_colored_point用于显示临时彩色点。
> 7. 定义了函数is_on_current_drawing用于判断特征是否在当前图纸中。
> 8. 主函数do_it中遍历图纸中的公差特征，并输出每个特征的信息。
> 9. 定义了UF示例主函数ufusr。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码展示了如何遍历和获取图纸中公差特征的信息，并在视图中高亮显示，对于学习NX二次开发中的特征遍历和显示非常有帮助。
>
