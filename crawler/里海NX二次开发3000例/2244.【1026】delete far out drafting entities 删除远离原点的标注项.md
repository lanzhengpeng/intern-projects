### 【1026】delete far out drafting entities 删除远离原点的标注项

#### 代码

```cpp
    /*HEAD DELETE_FAR_OUT_DRAFTING_ENTITIES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_drf.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
注意：UF_print_syslog是V18版本新增的功能，请只回答译文，不要发表其他无关内容。 */  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
    static logical ask_yes_or_no(char *prompt, logical *deflt)  
    {  
        int  resp;  
        char options[2][38] = { "Yes", "No" };  
        resp = 2 - *deflt;  
        resp = uc1603(prompt, resp, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
        *deflt = 6 - resp;  
        return TRUE;  
        }  
        else  
        return FALSE;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
        UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static tag_t ask_next_drafting_aid(tag_t part, tag_t draft)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type, &draft));  
        return (draft);  
    }  
    static int check_position( double max, double pos[3])  
    {  
        int   
            ii,  
            status = 0;  
        for (ii = 0; ii < 3; ii++)  
        {  
            double abs_coord = fabs(pos[ii]);  
            if ( abs_coord > max)  
            {  
                if (abs_coord > max)  
                status = 1;  
                break;  
            }  
        }  
        return status;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void put_far_out_drafting_aids_on_list  
        (double max, tag_t draft_aid, uf_list_p_t bad_list)  
    {  
        int  
            ii,  
            n_strings,  
            ps;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        char  
            msg[133];  
        UF_CALL(UF_DRF_ask_draft_aid_text_info(draft_aid, &n_strings, &text_info));  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            ps = check_position(max, text_info[ii].origin);  
            if (ps == 1) /* far away 里海译:遥远 */  
            {  
                sprintf(msg, "[tag %d], string %d of %d exceeds max at:\n \  
                    %f, %f, %f\n", draft_aid, ii, n_strings,text_info[ii].origin[0],   
                    text_info[ii].origin[1], text_info[ii].origin[2]);    
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_DISP_set_highlight(draft_aid, TRUE));  
                UF_CALL(UF_MODL_put_list_item(bad_list, draft_aid));  
                display_temporary_point(text_info[ii].origin);  
                break;  
            }  
        }  
        UF_CALL(UF_DRF_free_text(n_strings, &text_info));  
    }  
    static void do_it(void)  
    {  
        double  max;  
        int           
            n_bad,  
            units;  
        tag_t  
            *bad,  
            draft_aid = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        logical   
            go = TRUE;  
        char      
            messg[MAX_LINE_SIZE+1];  
        uf_list_p_t   
            bad_list;  
        UF_MODL_create_list(&bad_list);  
        UF_CALL(UF_PART_ask_units(part, &units));  
        if (units == UF_PART_METRIC)  
            max = 1000 * 1000;      /* 1000 meters in mm 里海译:1000 meters in mm等于1,000,000 mm。 */  
        else  
            max = 1000 * 1000 / 25.4;   /* 1000 meters in inches 里海译:1,000米等于39,370.1英寸。 */  
        if (prompt_for_a_number("Delete notes far from the origin",  
         "Farther than", &max))  
        {  
            UF_CALL(UF_UI_open_listing_window());  
            while ((draft_aid = ask_next_drafting_aid(part, draft_aid)) != NULL_TAG)  
            {  
                put_far_out_drafting_aids_on_list(max, draft_aid, bad_list);  
            }  
            n_bad = make_an_array(&bad_list, &bad);  
            if (n_bad > 0)  
            {  
                sprintf(messg, "%d invalid objects have been found\n", n_bad);  
                UF_CALL(UF_UI_write_listing_window(messg));  
                if (ask_yes_or_no("Delete them?", &go) && (go))  
                {  
                    UF_CALL(UF_OBJ_delete_array_of_objects(n_bad, bad, NULL));  
                }  
                else  
                    set_highlight_object_array(n_bad, bad, FALSE);  
                UF_free(bad);  
            }  
            else  
                UF_UI_write_listing_window("No invalid objects found\n");  
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

> 这段代码是一个用于删除远离原点的标注和注释的NX Open API程序。
>
> 主要功能包括：
>
> 1. 询问用户希望删除的标注和注释的最大距离，并据此设置最大距离值。
> 2. 遍历当前显示部件中的所有标注和注释，检查它们的原点坐标是否超出了最大距离。
> 3. 将超出最大距离的标注和注释放入一个列表中，并高亮显示。
> 4. 询问用户是否删除这些标注和注释，如果用户选择删除，则调用UF_OBJ_delete_array_of_objects删除列表中的对象。
> 5. 如果用户不删除，则取消高亮显示。
> 6. 在整个过程中，将相关信息输出到日志窗口。
>
> 这个程序的主要逻辑包括：
>
> 这个程序实现了删除远离原点的标注和注释的功能，并提供了良好的交互过程。
>
