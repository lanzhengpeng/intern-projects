### 【1905】report all dimension information 报告所有尺寸信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
    #include <uf_gdt.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    #define WRITE_F(X) ECHO("%s = %f\n", #X, X)  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    static tag_t ask_next_tol_instance(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part,  
            UF_tol_feature_instance_type, &dim)) && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
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
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, &mx[0],0,"X");  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, &mx[3],0,"Y");  
            UF_DISP_labeled_conehead(UF_DISP_ALL_ACTIVE_VIEWS, where, &mx[6],0,"Z");  
        }  
    }  
    static void map_obj2abs(tag_t obj, double pos[3])  
    {  
        tag_t  
            obj_mx;  
        double  
            mx[9];  
        UF_CALL(UF_CSYS_ask_matrix_of_object(obj, &obj_mx));  
        UF_CALL(UF_CSYS_ask_matrix_values(obj_mx, mx));  
        UF_MTX3_vec_multiply_t(pos, mx, pos);  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_ASTERISK));  
    }  
    static void report_all_dimension_information(tag_t dim)  
    {  
        int  
            ii,  
            jj,  
            subtype;  
        double  
            origin[3];  
        UF_DRF_dim_info_t  
            *info;  
        if (UF_CALL(UF_DRF_ask_dim_info(dim, &subtype, origin, &info)))  
            return;  
        ECHO("unmapped ");  
        WRITE3F(origin);  
        map_obj2abs(dim, origin);  
        ECHO("absolute ");  
        WRITE3F(origin);  
        show_objects_csys(dim, origin);  
        WRITE_D(info->num_text);  
        for (ii = 0; ii<info->num_text; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(info->text_info[ii].text_type);  
            WRITE_D(info->text_info[ii].text_font);  
            WRITE_F(info->text_info[ii].size);  
            WRITE_F(info->text_info[ii].angle);  
            ECHO("unmapped ");  
            WRITE3F(info->text_info[ii].origin);  
            map_obj2abs(dim, info->text_info[ii].origin);  
            ECHO("absolute ");  
            WRITE3F(info->text_info[ii].origin);  
            display_temporary_colored_point(info->text_info[ii].origin,  
                UF_OBJ_YELLOW);  
            WRITE_F(info->text_info[ii].length);  
            WRITE_F(info->text_info[ii].height);  
            WRITE_F(info->text_info[ii].distance);  
            WRITE_F(info->text_info[ii].aspect_ratio);  
            WRITE_F(info->text_info[ii].gap);  
            WRITE_F(info->text_info[ii].line_spacing);  
            WRITE_D(info->text_info[ii].num_lines);  
            for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(info->text_info[ii].text[jj].num_chars);  
                WRITE_S(info->text_info[ii].text[jj].string);  
                WRITE_D(info->text_info[ii].text[jj].num_ints);  
                WRITE_D(info->text_info[ii].text[jj].full_num_chars);  
                WRITE_S(info->text_info[ii].text[jj].full_string);  
            }  
        }  
        WRITE_D(info->num_lines);  
        for (ii = 0; ii < info->num_lines; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(info->dim_line_info->line_pnts[ii].sequence_number);  
            WRITE_D(info->dim_line_info->line_pnts[ii].num_symbols);  
            for (jj = 0; jj < info->dim_line_info->line_pnts[ii].num_symbols;  
                jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    master);  
                WRITE_L(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    is_gap);  
                WRITE3F(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    origin);  
                WRITE_F(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    parameter_on_element);  
                WRITE_F(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    symbol_length);  
                WRITE_F(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    symbol_height);  
                WRITE_D(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    creation_mask);  
                WRITE_D(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    record_number);  
                WRITE_D(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    segment_number);  
                WRITE_L(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    is_mirrored);  
                WRITE_L(info->dim_line_info->line_pnts[ii].symbol_data[jj].  
                    is_reflected);  
            }  
            WRITE_D(info->dim_line_info->line_pnts[ii].num_segments);  
            for (jj = 0;  
                 jj < info->dim_line_info->line_pnts[ii].num_segments * 6;  
                 jj = jj + 3)  
            {  
                WRITE_D(jj);  
                ECHO("unmapped ");  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj]);  
                map_obj2abs(dim,  
                    &info->dim_line_info->line_pnts[ii].segment_pnts[jj]);  
                ECHO("absolute ");  
                WRITE3F(&info->dim_line_info->line_pnts[ii].segment_pnts[jj]);  
                display_temporary_colored_point(  
                    &info->dim_line_info->line_pnts[ii].segment_pnts[jj],  
                    UF_OBJ_GREEN);  
            }  
        }  
        WRITE_D(info->dim_line_info->num_assoc_objs);  
        for (ii = 0; ii < info->dim_line_info->num_assoc_objs; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_object_view_tag);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_type);  
            WRITE_D(info->dim_line_info->assoc_objs[ii].assoc_modifier);  
        }  
        WRITE_D(info->num_arcs);  
        for (ii = 0; ii < info->num_arcs; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(info->arc_info[ii].arc_type);  
            WRITE_D(info->arc_info[ii].sequence_number);  
            ECHO("unmapped ");  
            WRITE3F(info->arc_info[ii].arc_center);  
            map_obj2abs(dim, info->arc_info[ii].arc_center);  
            ECHO("absolute ");  
            WRITE3F(info->arc_info[ii].arc_center);  
            display_temporary_colored_point(info->arc_info[ii].arc_center,  
                UF_OBJ_RED);  
            WRITE_F(info->arc_info[ii].radius);  
            WRITE_F(info->arc_info[ii].start_angle);  
            WRITE_F(info->arc_info[ii].start_angle * RADEG);  
            WRITE_F(info->arc_info[ii].end_angle);  
            WRITE_F(info->arc_info[ii].end_angle * RADEG);  
            WRITE_D(info->arc_info[ii].num_symbols);  
            for (jj = 0; jj < info->arc_info[ii].num_symbols; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(info->arc_info[ii].symbol_data[jj].master);  
                WRITE_L(info->arc_info[ii].symbol_data[jj].is_gap);  
                WRITE3F(info->arc_info[ii].symbol_data[jj].origin);  
                WRITE_F(info->arc_info[ii].symbol_data[jj].  
                    parameter_on_element);  
                WRITE_F(info->arc_info[ii].symbol_data[jj].symbol_length);  
                WRITE_F(info->arc_info[ii].symbol_data[jj].symbol_height);  
                WRITE_D(info->arc_info[ii].symbol_data[jj].creation_mask);  
                WRITE_D(info->arc_info[ii].symbol_data[jj].record_number);  
                WRITE_D(info->arc_info[ii].symbol_data[jj].segment_number);  
                WRITE_L(info->arc_info[ii].symbol_data[jj].is_mirrored);  
                WRITE_L(info->arc_info[ii].symbol_data[jj].is_reflected);  
            }  
            if (ii > 0) continue;  
            WRITE_D(info->arc_info[ii].num_assoc_objs);  
            for (jj = 0; jj < info->arc_info[ii].num_assoc_objs; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(info->arc_info[ii].assoc_objs[jj].assoc_object_tag);  
                WRITE_D(info->arc_info[ii].assoc_objs[jj].  
                    assoc_object_view_tag);  
                WRITE_D(info->arc_info[ii].assoc_objs[jj].assoc_type);  
                WRITE_D(info->arc_info[ii].assoc_objs[jj].assoc_modifier);  
            }  
        }  
        WRITE_D(info->num_arrows);  
        for (ii = 0; ii < info->num_arrows; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(info->arrow_info[ii].sequence_number);  
            WRITE_D(info->arrow_info[ii].arrow_type);  
            WRITE_D(info->arrow_info[ii].arrow_fill);  
            ECHO("unmapped ");  
            WRITE3F(info->arrow_info[ii].arrow_origin);  
            map_obj2abs(dim, info->arrow_info[ii].arrow_origin);  
            ECHO("absolute ");  
            WRITE3F(info->arrow_info[ii].arrow_origin);  
            display_temporary_colored_point(info->arrow_info[ii].arrow_origin,  
                UF_OBJ_CYAN);  
            WRITE_F(info->arrow_info[ii].arrow_angle);  
            WRITE_F(info->arrow_info[ii].arrow_include_angle);  
            WRITE_F(info->arrow_info[ii].arrow_height);  
            WRITE_F(info->arrow_info[ii].arrow_width);  
        }  
        ECHO("\n");  
        UF_CALL(UF_DRF_free_dimension(&info));  
    }  
    static void do_it(void)  
    {  
        int  
            ii;  
        tag_t  
            dim = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_GDT_annotation_tags_t  
            gdt_dims;  
        while ((dim = ask_next_dim(part, dim)) != NULL_TAG)  
            report_all_dimension_information(dim);  
        while ((dim = ask_next_tol_instance(part, dim)) != NULL_TAG)  
        {  
            UF_CALL(UF_GDT_ask_annotation_tags(dim, &gdt_dims));  
            for (ii = 0; ii < gdt_dims.num_dimension_tags; ii++)  
                report_all_dimension_information(gdt_dims.dimension_tags[ii]);  
            UF_CALL(UF_GDT_free(UF_GDT_ANNOTATION_TAGS_TYPE, (void *)&gdt_dims));  
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

> 这段代码是一个NX Open API开发的二次开发示例，其主要功能是遍历当前活动部件中的所有尺寸，并打印出每个尺寸的详细信息。代码的具体实现如下：
>
> 1. 定义了一些宏，用于打印变量、布尔值、字符串、浮点数和三维坐标等信息。
> 2. 实现了几个辅助函数，如ask_next_dim()用于遍历尺寸，ask_next_tol_instance()用于遍历公差实例，map_obj2abs()用于将对象坐标映射到绝对坐标系，以及show_objects_csys()用于显示对象坐标系等。
> 3. 实现了report_all_dimension_information()函数，用于打印出一个尺寸的所有信息，包括文本信息、尺寸线信息、圆弧信息、箭头信息等。
> 4. 在do_it()函数中，首先遍历所有尺寸，然后遍历所有公差实例，并打印出每个尺寸的详细信息。
> 5. 在ufusr()函数中，完成初始化和执行do_it()，并在结束前进行清理。
> 6. ufusr_ask_unload()函数用于卸载二次开发。
>
> 总体来说，这段代码实现了遍历当前活动部件中的所有尺寸，并打印出每个尺寸的详细信息的功能，展示了NX Open API的二次开发能力。
>
