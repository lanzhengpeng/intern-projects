### 【2532】scale selected finish symbols 选择结束符号进行缩放

#### 代码

```cpp
    /*HEAD SCALE_SELECTED_FINISH_SYMBOLS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_drf.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译如下：

UF_print_syslog是V18版本新增的。 */  
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
    /* qq3123197280 */  
    static int mask_for_draft_assorted_parts(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_drafting_entity_type, UF_draft_assorted_parts_subtype, 0};  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_draft_assorted_parts(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select draft assorted parts",  
            prompt, UF_UI_SEL_SCOPE_WORK_PART, mask_for_draft_assorted_parts,  
            NULL, &resp, &cnt, objects));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*objects)[ii], FALSE));  
        return cnt;  
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
    static logical ask_finish_symbol_origin(tag_t symb, double origin[3])  
    {  
        int  
            ann_data[10],  
            ann_data_type,  
            ann_data_form,  
            cycle_flag = 0,  
            ii,  
            mask[4] = { 1,0,0,0 },  
            num_segments;  
        double  
            ann_origin[2],  
            endpoints[4],  
            radius_angle;  
        if (!UF_CALL(UF_DRF_ask_ann_data(&symb, mask, &cycle_flag, ann_data,  
            &ann_data_type, &ann_data_form, &num_segments, ann_origin,  
            &radius_angle)) && (cycle_flag != 0) && (num_segments > 0))  
        {  
            ii = 1;  
            UF_CALL(UF_DRF_ask_ann_line_seg_ends(&ii, ann_data, endpoints));  
            UF_VEC2_copy(&endpoints[0], origin);  
            origin[2] = 0.0;  
            return TRUE;  
        }  
        else  
            return FALSE;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            *objs;  
        double  
            mx[12],  
            org[3],  
            scale = { 2.0 };  
        while (((n = select_draft_assorted_parts("Select finish symbols to scale",  
            &objs)) > 0) && (prompt_for_a_number("Enter scale", "Scale", &scale)))  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                if (ask_finish_symbol_origin(objs[ii], org))  
                {  
                    display_temporary_point(org);  
                    FTN(uf5944)(&one, &scale, org, mx, &resp);  
                    FTN(uf5947)(mx, &objs[ii], &one, &one, &zero, &two,  
                        NULL, NULL, &resp);  
                }  
            }  
            UF_free(objs);  
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

> 根据代码内容，这是一个用于NX的二次开发代码，其主要功能是选择并缩放草图中的符号。以下是代码的主要组成部分：
>
> 1. 头文件和宏定义：代码包含了必要的NX API头文件，并定义了UF_CALL宏用于错误处理。
> 2. 错误处理函数：report_error函数用于在出错时报告错误信息。
> 3. 选择过滤函数：mask_for_draft_assorted_parts用于在用户选择时过滤出草图杂项实体。
> 4. 选择函数：select_draft_assorted_parts用于弹出选择对话框，让用户选择草图中的符号。
> 5. 参数输入函数：prompt_for_a_number用于提示用户输入缩放比例。
> 6. 获取符号原点函数：ask_finish_symbol_origin用于获取符号的原点信息。
> 7. 临时点显示函数：display_temporary_point用于在图形窗口显示一个临时点。
> 8. 主要逻辑函数：do_it用于执行选择符号、获取原点、显示临时点、缩放符号等主要流程。
> 9. 主函数：ufusr用于初始化NX，执行主逻辑，然后终止NX。
> 10. 卸载函数：ufusr_ask_unload用于设置立即卸载二次开发代码。
>
> 综合来看，这段代码通过交互方式，实现了选择草图符号并按指定比例进行缩放的功能。
>
