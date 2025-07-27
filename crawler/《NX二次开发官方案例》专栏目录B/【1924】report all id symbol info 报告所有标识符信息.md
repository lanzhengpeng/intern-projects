### 【1924】report all id symbol info 报告所有标识符信息

#### 代码

```cpp
    /*HEAD REPORT_ALL_ID_SYMBOL_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog 是 V18 新增的。 */  
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
    static tag_t ask_next_id_symbol(tag_t part, tag_t id_symbol)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &id_symbol)) && (id_symbol != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(id_symbol, &type, &subtype));  
            if (subtype == UF_draft_id_symbol_subtype) return id_symbol;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)   (UF_UI_write_listing_window(X))  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
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
    #define WRITEnD(X,n) (write_integer_array_to_listing_window(#X, X, 1, n))  
    static void write_integer_array_to_listing_window(char *name, int *array,  
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
                sprintf(msg, "%d", array[kk++]);  
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
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int ask_major_ug_version_number(void)  
    {  
        int  
            rel_no;  
        char  
            *major;  
        UF_CALL(UF_get_release(&major));  
        sscanf(major, "V%d", &rel_no);  
        UF_free(major);  
        return rel_no;  
    }  
    static void report_id_symbol_info(tag_t id_symbol)  
    {  
        int  
            ii, jj,  
            ugv;  
        double  
            id_symbol_origin[3];  
        UF_DRF_id_symbol_type_t  
            id_symbol_type;  
        UF_DRF_id_symbol_info_t  
            *id_symbol_info;  
        WRITE("\n");  
        WRITE_D(id_symbol);  
        UF_CALL(UF_DRF_ask_id_symbol_info(id_symbol, &id_symbol_type,  
            id_symbol_origin, &id_symbol_info));  
        WRITE_D(id_symbol_type);  
        WRITE3F(id_symbol_origin);  
        WRITE_F(id_symbol_info->size);  
        WRITE_D(id_symbol_info->num_text);  
        for (ii = 0; ii < id_symbol_info->num_text; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(id_symbol_info->text_info[ii].text_type);  
            WRITE_D(id_symbol_info->text_info[ii].text_font);  
            WRITE_F(id_symbol_info->text_info[ii].size);  
            WRITE_F(id_symbol_info->text_info[ii].angle);  
            WRITE3F(id_symbol_info->text_info[ii].origin);  
            WRITE_F(id_symbol_info->text_info[ii].length);  
            WRITE_F(id_symbol_info->text_info[ii].height);  
            WRITE_F(id_symbol_info->text_info[ii].distance);  
            WRITE_F(id_symbol_info->text_info[ii].aspect_ratio);  
            WRITE_F(id_symbol_info->text_info[ii].gap);  
            WRITE_F(id_symbol_info->text_info[ii].line_spacing);  
            WRITE_D(id_symbol_info->text_info[ii].num_lines);  
            for (jj=0; jj<id_symbol_info->text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_S(id_symbol_info->text_info[ii].text[jj].string);  
            }  
        }  
        WRITE_D(id_symbol_info->num_leaders);  
        if (id_symbol_info->num_leaders > 0)  
        {  
            WRITEnD(id_symbol_info->leader_info->leader_attach_type,  
                id_symbol_info->num_leaders);  
            WRITE_D(id_symbol_info->leader_info->num_linebks);  
            for (ii = 0; ii < id_symbol_info->leader_info->num_linebks; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->leader_info->  
                    leader_pnts[ii].sequence_number);  
                WRITE_D(id_symbol_info->leader_info->leader_pnts[ii].num_symbols);  
                WRITE_D(id_symbol_info->leader_info->leader_pnts[ii].num_segments);  
                write_double_array_to_listing_window(  
                    "id_symbol_info->leader_info->leader_pnts[ii].segment_pnts",  
                    id_symbol_info->leader_info->leader_pnts[ii].segment_pnts,  
                    id_symbol_info->leader_info->leader_pnts[ii].num_segments,  
                    6);  
            }  
            WRITE_D(id_symbol_info->leader_info->num_assoc_objs);  
            for (ii = 0; ii < id_symbol_info->leader_info->num_assoc_objs; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->leader_info->  
                    assoc_objs[ii].assoc_object_tag);  
                WRITE_D(id_symbol_info->leader_info->  
                    assoc_objs[ii].assoc_object_view_tag);  
                WRITE_D(id_symbol_info->leader_info->assoc_objs[ii].assoc_type);  
                WRITE_D(id_symbol_info->leader_info->assoc_objs[ii].assoc_modifier);  
            }  
        }  
        WRITE_D(id_symbol_info->num_arrows);  
    /*  There's a problem with the arrow data prior to V17 里海译:翻译：在V17之前，箭头数据存在问题。 */  
        ugv = ask_major_ug_version_number();  
        if (ugv > 16)  
            for (ii = 0; ii < id_symbol_info->num_arrows; ii++)  
            {  
                WRITE_D(ii);  
                WRITE_D(id_symbol_info->arrow_info[ii].sequence_number);  
                WRITE_D(id_symbol_info->arrow_info[ii].arrow_type);  
                WRITE_D(id_symbol_info->arrow_info[ii].arrow_fill);  
                WRITE3F(id_symbol_info->arrow_info[ii].arrow_origin);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_angle);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_include_angle);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_height);  
                WRITE_F(id_symbol_info->arrow_info[ii].arrow_width);  
            }  
        UF_CALL(UF_DRF_free_idsymbol(&id_symbol_info));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            id_symbol = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((id_symbol = ask_next_id_symbol(part, id_symbol)) != NULL_TAG)  
            report_id_symbol_info(id_symbol);  
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

> 这段代码是NX的二次开发代码，其主要功能是遍历零件中的所有标识符号，并输出每个标识符号的信息。具体来说：
>
> 1. 定义了错误报告函数report_error，用于打印出调用UF函数失败的错误信息。
> 2. 定义了辅助函数ask_next_id_symbol，用于遍历零件中的所有标识符号。
> 3. 定义了各种WRITE宏，用于向列表窗口输出不同类型的信息，方便调试。
> 4. 定义了ask_major_ug_version_number函数，用于获取NX的版本号。
> 5. 定义了report_id_symbol_info函数，用于输出一个标识符号的所有信息，包括类型、原点、大小、文本信息、引线信息、箭头信息等。
> 6. 定义了do_it函数，用于遍历当前显示的零件中的所有标识符号，并调用report_id_symbol_info输出每个标识符号的信息。
> 7. ufusr是NX调用的主函数，在初始化后调用do_it，然后结束。
> 8. ufusr_ask_unload函数用于卸载用户程序。
>
> 总的来说，这段代码利用NX提供的API，遍历零件中的所有标识符号，并输出每个标识符号的详细信息，用于调试和查看零件的标识符号信息。
>
