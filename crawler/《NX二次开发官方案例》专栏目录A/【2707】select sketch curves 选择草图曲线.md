### 【2707】select sketch curves 选择草图曲线

#### 代码

```cpp
    /*HEAD SELECT_SKETCH_CURVES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_sket.h>  
    #include <uf_part.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的函数。 */  
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
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static void ask_all_sketch_curves(uf_list_p_t curve_list)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            for (ii = 0; ii < n; ii++)  
                UF_CALL(UF_MODL_put_list_item(curve_list, geoms[ii]));  
            if (n > 0) UF_free(geoms);  
        }  
    }  
    /* qq3123197280 */  
    static int is_a_choice(tag_t object, int type[3], void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            choices = (uf_list_p_t)user_data;  
        for (; choices != NULL; choices = choices->next)  
        {  
            if (object == choices->eid) return UF_UI_SEL_ACCEPT;  
        }  
        return UF_UI_SEL_REJECT;  
    }  
    /* qq3123197280 */  
    static int mask_for_sketch_curves(UF_UI_selection_p_t select, void *choices)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        ask_all_sketch_curves((uf_list_p_t)choices);  
        UF_CALL(UF_UI_set_sel_procs(select, is_a_choice, NULL, choices));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_sketch_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        uf_list_p_t  
            sketch_curves;  
        UF_CALL(UF_MODL_create_list(&sketch_curves));  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_sketch_curves,  
                (void *)sketch_curves, &resp, &object, cp, &view));  
        UF_CALL(UF_MODL_delete_list(&sketch_curves));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_sketch_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        uf_list_p_t  
            sketch_curves;  
        UF_CALL(UF_MODL_create_list(&sketch_curves));  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_sketch_curves,  
            (void *)sketch_curves, &resp, &cnt, curves));  
        UF_CALL(UF_MODL_delete_list(&sketch_curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            object,  
            *objects;  
        while ((object = select_a_sketch_curve("Select sketch curve")) != NULL_TAG)  
            ECHO(object);  
        while ((n = select_sketch_curves("Select sketch curves", &objects)) > 0)  
        {  
            ECHO(n);  
            UF_free(objects);  
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

> 这段代码是用于NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个报告错误函数report_error，用于在函数执行失败时输出错误信息。
> 2. 定义了ask_next_sketch函数，用于遍历当前显示部件中的所有草图并返回下一个草图的tag。
> 3. 定义了ask_all_sketch_curves函数，用于获取当前显示部件中所有草图曲线的列表。
> 4. 定义了is_a_choice函数，用于在用户选择时判断选择的对象是否为草图曲线列表中的对象。
> 5. 定义了mask_for_sketch_curves函数，用于设置选择对话框的过滤条件，只允许选择直线、圆、二次曲线和样条线。
> 6. 定义了select_a_sketch_curve函数，用于让用户选择一个草图曲线并返回其tag。
> 7. 定义了select_sketch_curves函数，用于让用户选择多个草图曲线并返回其tag数组。
> 8. do_it函数用于遍历选择一个草图曲线和选择多个草图曲线的流程。
> 9. ufusr函数是二次开发程序的入口函数，在这里调用do_it函数执行流程。
> 10. ufusr_ask_unload函数用于在卸载时立即释放内存。
>
> 总的来说，这段代码实现了通过对话框让用户选择草图曲线的功能，通过遍历获取草图曲线列表，并设置了选择过滤条件。
>
