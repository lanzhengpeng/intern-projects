### 【3148】report label info 遍历当前显示部件中的所有标注，并报告每个标注的详细信息

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
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
    static tag_t ask_next_label(tag_t part, tag_t label)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part,UF_drafting_entity_type,&label));  
            if (label)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(label, &type, &subtype));  
                if (subtype == UF_draft_label_subtype) return label;  
            }  
        } while (label);  
        return NULL_TAG;  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            jj,  
            kk,  
            n_assoc;  
        tag_t  
            label = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            arrow_pnt[3],  
            leader_pnt[3],  
            origin[3],  
            text_pnt[3];  
        UF_DRF_label_info_t  
            *info;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        while ((label = ask_next_label(part, label)) != NULL_TAG)  
        {  
            WRITE_D(label);  
            UF_CALL(UF_DRF_ask_label_info(label, origin, &info));  
            ECHO("unmapped ");  
            WRITE3F(origin);  
            map_obj2abs(label, origin);  
            ECHO("absolute ");  
            WRITE3F(origin);  
            display_temporary_colored_point(origin, UF_OBJ_WHITE);  
            WRITE_D(info->num_text);  
            for (ii = 0; ii < info->num_text; ii++)  
            {  
                UF_VEC3_copy(info->text_info[ii].origin, text_pnt);  
                ECHO("unmapped ");  
                WRITE3F(text_pnt);  
                map_obj2abs(label, text_pnt);  
                ECHO("absolute ");  
                WRITE3F(text_pnt);  
                display_temporary_colored_point(text_pnt, UF_OBJ_YELLOW);  
                for (jj = 0; jj < info->text_info[ii].num_lines; jj++)  
                    WRITE_S(info->text_info[ii].text[jj].string);  
            }  
            WRITE_D(info->num_leaders);  
            for (jj = 0; jj < info->num_leaders; jj++)  
                WRITE_D(info->leader_info->leader_attach_type[jj]);  
            WRITE_D(info->leader_info->num_linebks);  
            for (jj = 0; jj < info->leader_info->num_linebks; jj++)  
            {  
                WRITE_D(info->leader_info->leader_pnts[jj].num_segments);  
                for (kk = 0;  
                    kk < info->leader_info->leader_pnts[jj].num_segments * 6;  
                    kk = kk + 3)  
                {  
                    UF_VEC3_copy(  
                        &info->leader_info->leader_pnts[jj].segment_pnts[kk],  
                        leader_pnt);  
                    ECHO("unmapped ");  
                    WRITE3F(leader_pnt);  
                    map_obj2abs(label, leader_pnt);  
                    ECHO("absolute ");  
                    WRITE3F(leader_pnt);  
                    display_temporary_colored_point(leader_pnt, UF_OBJ_GREEN);  
                }  
            }  
            WRITE_D(info->leader_info->num_assoc_objs);  
            for (jj = 0; jj < info->leader_info->num_assoc_objs; jj++)  
            {  
                WRITE_D(info->leader_info->assoc_objs[jj].assoc_object_tag);  
                WRITE_D(info->leader_info->assoc_objs[jj].  
                    assoc_object_view_tag);  
                WRITE_D(info->leader_info->assoc_objs[jj].assoc_type);  
                WRITE_D(info->leader_info->assoc_objs[jj].assoc_modifier);  
            }  
            WRITE_D(info->num_arrows);  
            for (ii = 0; ii < info->num_arrows; ii++)  
            {  
                UF_VEC3_copy(info->arrow_info[ii].arrow_origin, arrow_pnt);  
                ECHO("unmapped ");  
                WRITE3F(arrow_pnt);  
                map_obj2abs(label, arrow_pnt);  
                ECHO("absolute ");  
                WRITE3F(arrow_pnt);  
                display_temporary_colored_point(arrow_pnt, UF_OBJ_CYAN);  
            }  
            UF_CALL(UF_DRF_free_label(&info));  
            UF_CALL(UF_DRF_ask_associativity_data(label, &n_assoc, &assoc_data));  
            WRITE_D(n_assoc);  
            for (ii = 0; ii < n_assoc; ii++)  
            {  
                WRITE_D(assoc_data[ii].assoc_object_1);  
                WRITE_D(assoc_data[ii].object_view);  
                WRITE3F(assoc_data[ii].assoc_point);  
                display_temporary_colored_point(assoc_data[ii].assoc_point,  
                    UF_OBJ_RED);  
            }  
            if (n_assoc > 0) UF_free(assoc_data);  
            ECHO("\n");  
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

> 这段NX二次开发代码的主要功能是遍历当前显示部件中的所有标注，并获取和显示标注的详细信息。具体来说，代码实现了以下功能：
>
> 1. 定义了ECHO宏，用于输出信息到日志窗口和系统日志。
> 2. 定义了UF_CALL宏，用于调用UF函数，并在函数调用失败时报告错误。
> 3. 定义了WRITE_D、WRITE_S、WRITE3F等宏，用于输出信息到日志窗口。
> 4. ask_next_label函数用于遍历部件中的下一个标注。
> 5. map_obj2abs函数用于将对象坐标转换到绝对坐标系。
> 6. display_temporary_colored_point函数用于在指定坐标处显示一个临时点。
> 7. do_it函数是主函数，遍历当前显示部件中的所有标注，获取标注的详细信息，包括文本位置、引线位置、箭头位置、关联对象等，并显示在日志窗口。
> 8. ufusr是NX调用的主函数，初始化UF模块后调用do_it函数，然后终止UF模块。
> 9. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 总的来说，这段代码实现了对NX模型中标注的遍历和详细信息的获取，以帮助用户更好地理解和分析模型中的标注信息。
>
