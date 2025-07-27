### 【2323】report selected drawing as note 报告所选图纸作为注释

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_DRAWING_AS_NOTE CCC UFUN */  
    /* Reports the selected drawing name,   
    its size and number of all drawings.  
    Then a note is created at any selected location */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_drf.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。在回答问题时，请直接提供翻译结果，不要添加任何其他废话。 */  
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
    static int mask_for_drawings(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[1] = { { UF_view_type, UF_view_drawing_sheet_subtype, 0 } };  
    //        mask[1] = { { UF_drawing_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_drawing(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a drawing", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_drawings, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void report_drawing_size(tag_t drawing, char *draw_info)  
    {  
        char  
            //draw_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1],  
            *unit_names[2] = { "mm", "inches" };  
        UF_DRAW_info_t  
            info;  
        UF_CALL(UF_OBJ_ask_name(drawing, draw_info));  
        UF_CALL(UF_DRAW_ask_drawing_info(drawing, &info));  
        switch (info.size_state)  
        {  
            case UF_DRAW_METRIC_SIZE:  
                switch (info.size.metric_size_code)  
                {  
                    case UF_DRAW_A0:  
                        strcat(draw_info, " - A0 (841 x 1189 mm)");  
                        break;  
                    case UF_DRAW_A1:  
                        strcat(draw_info, " - A1 (594 x 841 mm)");  
                        break;  
                    case UF_DRAW_A2:  
                        strcat(draw_info, " - A2 (420 x 594 mm)");  
                        break;  
                    case UF_DRAW_A3:  
                        strcat(draw_info, " - A3 (297 x 420 mm)");  
                        break;  
                    case UF_DRAW_A4:  
                        strcat(draw_info, " - A4 (210 x 297 mm)");  
                        break;  
                    default:  
                        strcat(draw_info, " - Unknown metric size");  
                        break;  
                }  
                break;  
            case UF_DRAW_ENGLISH_SIZE:  
                switch (info.size.english_size_code)  
                {  
                    case UF_DRAW_A:  
                        strcat(draw_info, " - A (8.5 x 11 inches)");  
                        break;  
                    case UF_DRAW_B:  
                        strcat(draw_info, " - B (11 x 17 inches)");  
                        break;  
                    case UF_DRAW_C:  
                        strcat(draw_info, " - C (17 x 22 inches)");  
                        break;  
                    case UF_DRAW_D:  
                        strcat(draw_info, " - D (22 x 34 inches)");  
                        break;  
                    case UF_DRAW_E:  
                        strcat(draw_info, " - E (34 x 44 inches)");  
                        break;  
                    case UF_DRAW_F:  
                        strcat(draw_info, " - F (28 x 40 inches)");  
                        break;  
                    case UF_DRAW_H:  
                        strcat(draw_info, " - H (28 x 44 inches)");  
                        break;  
                    case UF_DRAW_J:  
                        strcat(draw_info, " - J (34 x 55 inches)");  
                        break;  
                    default:  
                        strcat(draw_info, " - Unknown english size");  
                        break;  
                }  
                break;  
            case UF_DRAW_CUSTOM_SIZE:  
                sprintf(msg, " - Custom size (%.1f x %.1f %s)\n",  
                    info.size.custom_size[0], info.size.custom_size[1],  
                    unit_names[info.units - 1]);  
                strcat(draw_info, msg);  
                break;  
            default:  
                strcat(draw_info, " - Unknown size");  
                break;  
        }  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void map_abs2wcs(double *cc)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            wcs[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            wcs_ent,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        FTN(uf5940)(abs, wcs, mx, &irc);  
        FTN(uf5941)(cc, mx);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing,  
            *drawings,  
            view,  
            note_tag;  
        int  
            n_drawings;  
        double   
            origin[3];  
        char  
            draw_info[1][133],  
            msg[10];  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        if (n_drawings > 0) UF_free(drawings);  
        while ((view = select_a_drawing("Retrieve Drawing Information")) != NULL_TAG)  
        {  
            UF_DRAW_ask_drawing_of_view (view, &drawing );  
            report_drawing_size(drawing, draw_info[0]);  
            sprintf(msg, " / %d", n_drawings);  
            strcat( draw_info[0], msg);  
            /* just for testing  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window(draw_info[0]);  
            */  
            while (specify_point("Indicate note position", origin))  
            {  
                map_abs2wcs(origin);  
                uc5540(1, draw_info, origin, &note_tag);  
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

> 这段代码是用于NX的二次开发，其主要功能包括：
>
> 1. 选择并报告图纸信息：使用UF_DRAW_ask_drawings函数获取所有图纸的信息，然后让用户选择一个图纸，并使用UF_DRAW_ask_drawing_of_view函数获取图纸的tag。接着，使用UF_DRAW_info_t结构体获取图纸的尺寸信息，并将其格式化成字符串输出。
> 2. 创建注释：在用户指定的位置创建注释。首先，通过对话框让用户指定注释的位置，然后使用自定义函数map_abs2wcs将绝对坐标系转换到工作坐标系。最后，使用自定义函数uc5540在指定位置创建注释。
> 3. 错误处理：使用宏UF_CALL封装UF函数调用，并在调用失败时输出错误信息到日志窗口。
> 4. 初始化和终止：在ufusr函数中初始化和终止NX API。
> 5. 提示卸载：在ufusr_ask_unload函数中提示立即卸载用户自定义函数。
>
> 该代码实现了选择并报告图纸信息，以及在图纸指定位置创建注释的功能，并且具有良好的错误处理和提示卸载机制。
>
