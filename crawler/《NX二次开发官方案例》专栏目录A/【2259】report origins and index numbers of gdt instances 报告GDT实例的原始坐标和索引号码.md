### 【2259】report origins and index numbers of gdt instances 报告GDT实例的原始坐标和索引号码

#### 代码

```cpp
    /*HEAD REPORT_ORIGINS_AND_INDEX_NUMBERS_OF_GDT_INSTANCES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_gdt.h>  
    #include <uf_drf.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 新增的函数。 */  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
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
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
        printf(msg);  
        uc1601(msg, TRUE);  
    }  
    static void do_it(void)  
    {  
        logical  
            has_size;  
        int  
            index;  
        tag_t  
            instance = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            tol_feat;  
        double  
            origin[3];  
        while ((instance = ask_next_of_type(part, UF_tol_feature_instance_type,  
                    instance)) != NULL_TAG)  
        {  
            WRITE_D(instance);  
            UF_CALL(UF_DRF_ask_origin(instance, origin));  
            WRITE3F(origin);  
            UF_CALL(UF_GDT_ask_tol_feat_of_instance(instance, &tol_feat));  
            WRITE_D(tol_feat);  
            UF_CALL(UF_GDT_has_size_tolerance(tol_feat, &has_size));  
            WRITE_L(has_size);  
            if (has_size &&  
                !UF_CALL(UF_GDT_ask_tolerance_index(tol_feat, &index)))  
                    WRITE_D(index);  
            if (is_on_current_drawing(instance))  
            {  
                display_temporary_colored_point(origin, UF_OBJ_YELLOW);  
                UF_CALL(UF_DISP_set_highlight(instance, TRUE));  
                SAY(index);  
                UF_CALL(UF_DISP_set_highlight(instance, FALSE));  
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

> 这段代码是一个NX的二次开发程序，主要用于遍历当前活动部件中的所有公差特征实例，并报告它们的原点坐标、关联的公差特征对象、是否具有尺寸公差，以及对应的公差索引。主要功能如下：
>
> 1. 定义了一些宏，用于简化报告错误、写入列表窗口等操作。
> 2. 定义了一个辅助函数ask_next_of_type，用于遍历部件中的指定类型的对象。
> 3. 定义了几个辅助函数，用于向列表窗口写入整数、浮点数数组、布尔值等。
> 4. 定义了一个函数display_temporary_colored_point，用于在当前视图中显示一个临时点，并设置颜色。
> 5. 定义了一个函数is_on_current_drawing，用于判断一个对象是否在当前图纸中。
> 6. 定义了一个函数report_an_integer，用于报告一个整数值。
> 7. 定义了主函数do_it，用于遍历所有公差特征实例，报告相关信息，并在当前视图中高亮显示。
> 8. 定义了ufusr函数，作为程序的入口，初始化后调用do_it函数，最后终止。
> 9. 定义了ufusr_ask_unload函数，用于卸载用户程序。
>
> 该程序通过遍历所有公差特征实例，可以方便地报告它们的详细信息，并在图形窗口中高亮显示，以便进行查看。
>
