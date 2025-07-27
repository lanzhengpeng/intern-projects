### 【1823】read sketch dimensions Read sketch dimensions 读取草图尺寸信息

#### 代码

```cpp
    /*HEAD READ_SKETCH_DIMENSIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：
注意：UF_print_syslog在V18中是新增的，请只提供翻译，不要添加无关内容。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
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
    static void report_dimension_line_information(tag_t dim)  
    {  
        int  
            ii,  
            jj,  
            subtype;  
        double  
            origin[3];  
        UF_DRF_dim_info_t  
            *info;  
        UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info));  
        for (ii = 0; ii<info->num_text; ii++)  
            for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
                WRITE_S(info->text_info[ii].text[jj].string);  
        WRITE_D(info->num_lines);  
        for (ii = 0; ii < info->num_lines; ii++)  
        {  
            WRITE_D(info->dim_line_info->line_pnts[ii].num_segments);  
            for (jj=0; jj<info->dim_line_info->line_pnts[ii].num_segments; jj++)  
            {  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj*6]);  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj*6+3]);  
            }  
        }  
        WRITE_D(info->dim_line_info->num_assoc_objs);  
        for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
        {  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_view_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_type);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_modifier);  
        }  
        printf("\n");  
        UF_CALL(UF_DRF_free_dimension(&info));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *dims,  
            dim_exp,  
            sketch;  
        double  
            dim_origin[3];  
        UF_SKET_con_type_t  
            dim_type;  
        UF_SKET_dim_object_t  
            dim_objects[2];  
        while ((sketch = select_a_sketch("Select sketch")) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n, &dims));  
            WRITE_D(n);  
            for (ii = 0; ii < n; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(dims[ii]);  
                if (!UF_CALL(UF_SKET_read_dimension(sketch, dims[ii], &dim_type,  
                    &dim_objects[0], &dim_objects[1], dim_origin, &dim_exp)))  
                {  
                    WRITE_D(dim_type);  
                    WRITE_D(dim_objects[0].object_tag);  
                    WRITE_D(dim_objects[0].object_assoc_type);  
                    WRITE_D(dim_objects[0].object_assoc_mod_value);  
                    WRITE_D(dim_objects[1].object_tag);  
                    WRITE_D(dim_objects[1].object_assoc_type);  
                    WRITE_D(dim_objects[1].object_assoc_mod_value);  
                    WRITE_D(dim_exp);  
                }  
                report_dimension_line_information(dims[ii]);  
            }  
            if (n > 0) UF_free(dims);  
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

> 根据代码内容，这段代码是一个NX的二次开发示例，主要用于读取草图中的尺寸信息。
>
> 具体来说，这段代码的主要功能包括：
>
> 1. 定义错误处理函数report_error，用于输出函数调用错误信息。
> 2. 定义选择草图的函数select_a_sketch，提示用户选择一个草图。
> 3. 定义写入整数、字符串、数组到列表窗口的函数。
> 4. 定义报告尺寸线信息的函数report_dimension_line_information，用于输出尺寸线的详细信息。
> 5. 主函数do_it中，循环选择草图，读取每个草图的所有尺寸，并输出每个尺寸的类型、关联对象、表达式等信息，以及尺寸线的详细信息。
> 6. 定义ufusr函数，初始化NX，调用do_it函数，然后终止NX。
> 7. 定义卸载函数，用于卸载应用程序。
>
> 总体来说，这段代码实现了在NX中读取草图尺寸信息的功能，并提供了错误处理和用户交互。
>
