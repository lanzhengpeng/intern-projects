### 【1646】move all simple section line labels to arrow tip 移动所有简单剖面线标签到箭头尖端

#### 代码

```cpp
    /*HEAD MOVE_ALL_SIMPLE_SECTION_LINE_LABELS_TO_ARROW_TIP CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_eval.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_drf.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，可以翻译为：

UF_print_syslog是在V18版本中新增的功能，仅需要回答翻译，不要添加其他无关内容。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_section_lines_of_type(tag_t part,  
        UF_DRAW_sxline_type_t type, tag_t **objects)  
    {  
        UF_DRAW_sxline_type_t  
            this_type;  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_section_line_type,  
            &object)) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_DRAW_ask_sxline_type(object, &this_type));  
            if (this_type == type)  
                (UF_MODL_put_list_item(object_list, object));  
        }  
        return (make_an_array(&object_list, objects));  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static tag_t ask_next_note(tag_t part, tag_t a_note)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type,  
            &a_note)) && (a_note != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(a_note, &type, &subtype));  
            if (subtype == UF_draft_note_subtype) return a_note;  
        }  
        return NULL_TAG;  
    }  
    static tag_t find_note_with_text(tag_t part, char *text)  
    {  
        int  
            ii, jj,  
            n_text;  
        tag_t  
            a_note = NULL_TAG;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        while ((a_note = ask_next_note(part, a_note)) != NULL_TAG)  
        {  
            UF_CALL(UF_DRF_ask_draft_aid_text_info(a_note, &n_text, &text_info));  
            for (ii = 0; ii < n_text; ii++)  
            {  
                for (jj = 0; jj < text_info[ii].num_lines; jj++)  
                {  
                    if (strstr(text_info[ii].text[jj].string, text) != NULL)  
                        break;  
                }  
                if (jj < text_info[ii].num_lines) break;  
            }  
            UF_CALL(UF_DRF_free_text(n_text, &text_info));  
            if (ii < n_text) return a_note;  
        }  
        return NULL_TAG;  
    }  
    static void set_drf_object_align_site(tag_t object, UF_DRF_align_position_t pos)  
    {  
        int  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27];  
        if (!UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia)))  
        {  
            mpi[30] = pos;  
            UF_CALL(UF_DRF_set_object_preferences(object, mpi, mpr, rad, dia));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_sxlines,  
            n_sxsegs,  
            n_sxviews;  
        tag_t  
            cut,  
            note,  
            part = UF_PART_ask_display_part(),  
            pview,  
            sxseg,  
            *sxlines,  
            *sxsegs,  
            *sxviews;  
        double  
            arrow_dir[3],  
            end[3],  
            offset[3],  
            new_origin[3],  
            start[3],  
            step_dir[3];  
        UF_DRAW_sxseg_info_t  
            sxseg_info;  
        UF_DRF_object_t  
            *object;  
        UF_DRAW_sxline_status_t  
            sxline_status;  
        uf_list_p_t  
            seg_list;  
        char  
            letter_text[MAX_LINE_SIZE+1];  
        n_sxlines = ask_all_section_lines_of_type(part, UF_DRAW_simple_sxline,  
            &sxlines);  
        for (ii = 0; ii < n_sxlines; ii++)  
        {  
            UF_CALL(UF_DRAW_ask_simple_sxline(sxlines[ii], step_dir, arrow_dir,  
                &pview, &n_sxviews, &sxviews, &n_sxsegs, &sxsegs, &sxline_status));  
            UF_CALL(UF_MODL_create_list(&seg_list));  
            for (ii = 0; ii < n_sxsegs; ii++)  
            {  
                UF_CALL(UF_DRAW_ask_sxline_sxseg(sxsegs[ii], &sxseg_info,  
                    &sxseg, &object));  
                UF_CALL(UF_MODL_put_list_item(seg_list, sxseg));  
                if (sxseg_info.sxseg_type == UF_DRAW_sxseg_cut) cut = sxsegs[ii];  
            }  
            if (n_sxviews > 0) UF_free(sxviews);  
            if (n_sxsegs > 0) UF_free(sxsegs);  
            n_sxsegs = make_an_array(&seg_list, &sxsegs);  
            sprintf(letter_text, "<W!%d@SX_SEG_START_LETTER_ATTR>", cut);  
            note = find_note_with_text(part, letter_text);  
            if (note != NULL_TAG)  
            {  
                ask_curve_ends(sxsegs[0], start, end);  
                UF_VEC3_sub(start, end, offset);  
                UF_VEC3_add(start, offset, new_origin);  
                set_drf_object_align_site(note, UF_DRF_ALIGN_MID_CENTER);  
                UF_CALL(UF_DRF_set_origin(note, new_origin));  
            }  
            sprintf(letter_text, "<W!%d@SX_SEG_END_LETTER_ATTR>", cut);  
            note = find_note_with_text(part, letter_text);  
            if (note != NULL_TAG)  
            {  
                ask_curve_ends(sxsegs[n_sxsegs-1], start, end);  
                UF_VEC3_sub(start, end, offset);  
                UF_VEC3_add(start, offset, new_origin);  
                set_drf_object_align_site(note, UF_DRF_ALIGN_MID_CENTER);  
                UF_CALL(UF_DRF_set_origin(note, new_origin));  
            }  
            if (n_sxsegs > 0) UF_free(sxsegs);  
        }  
        if (n_sxlines > 0) UF_free(sxlines);  
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

> 根据代码内容，这是一段用于NX软件的二次开发代码，主要功能是：
>
> 1. 获取零件中的所有简单剖面线对象，并将其存储在一个数组中。
> 2. 遍历每个剖面线对象，获取其包含的剖面线段对象，并将其存储在一个数组中。
> 3. 对于每个剖面线对象，找到其起始和结束的注释对象（如果存在）。
> 4. 获取剖面线段对象的起始和结束点，然后根据注释文本中的剖面线段编号，将注释对象的中心对齐到对应的剖面线段端点，并更新注释对象的位置。
> 5. 释放分配的内存。
>
> 主要函数包括：
>
> 通过以上操作，代码实现了将零件中的剖面线注释对象对齐到对应的剖面线段端点的功能。
>
