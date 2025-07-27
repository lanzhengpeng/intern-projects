### 【2935】subtract target into multiple pieces and retain tool 将目标物体切割成多个碎片并保留工具

#### 代码

```cpp
    /*HEAD SUBTRACT_TARGET_INTO_MULTIPLE_PIECES_AND_RETAIN_TOOL CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_undo.h>  
    #include <uf_assem.h>  
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
    /* qq3123197280 */  
    static int filter_body_type(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            body_type;  
        UF_CALL(UF_MODL_ask_body_type(object, &body_type));  
        if (body_type == *(int *)data)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    static int mask_body_type( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_type, NULL, type)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static tag_t copy_object(tag_t object)  
    {  
        int  
            subtype,  
            this_subtype,  
            type;  
        tag_t  
            last = NULL_TAG,  
            next = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
    /*  find the last object in the work part of the same type 里海译:找到工作区中同类最后一个对象 */  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        while ((!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &next))) && (next != NULL_TAG))  
            last = next;  
        UF_CALL(UF_PART_ask_part_name(part, part_name));  
        UF_CALL(UF_PART_export_with_options(part_name, 1, &object, &options));  
    /*  find the new copied object in the work part 里海译:在零件图中找到新复制的对象。 */  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &last)) && (last != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(last, &type, &this_subtype));  
            if (this_subtype == subtype) break;  
        }  
        return last;  
    }  
    static void do_it(void)  
    {  
        int  
            num_result;  
        tag_t  
            target,  
            tool,  
            *resulting_bodies;  
        UF_UNDO_mark_id_t  
            mark;  
        while (((target = select_a_solid("Select target solid")) != NULL_TAG) &&  
               ((tool = select_a_solid("Select tool solid")) != NULL_TAG))  
        {  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Subtract", &mark));  
            tool = copy_object(tool);  
            if (UF_CALL(UF_MODL_subtract_bodies(target, tool, &num_result,  
                &resulting_bodies)))  
            {  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
                continue;  
            }  
            number_objects(resulting_bodies, num_result);  
            if (num_result > 0) UF_free(resulting_bodies);  
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

> 这段代码是一个NX Open的UF开发示例，实现了从目标实体中减去工具实体的功能，并将结果实体进行编号。以下是代码的主要功能：
>
> 1. 包含必要的头文件，定义了UF接口函数和类型。
> 2. 定义了一个错误报告函数report_error，用于打印错误信息。
> 3. 定义了一个选择过滤函数filter_body_type，用于过滤出指定的实体类型。
> 4. 定义了一个设置选择掩码的函数mask_body_type。
> 5. 定义了一个选择实体的函数select_a_solid。
> 6. 定义了一个显示临时文本的函数display_temporary_text。
> 7. 定义了一个为实体编号的函数number_objects。
> 8. 定义了一个复制实体的函数copy_object。
> 9. 定义了主函数do_it，用于实现从目标实体中减去工具实体，并对结果实体进行编号。
> 10. 定义了ufusr函数，用于初始化和执行主函数。
> 11. 定义了卸载函数ufusr_ask_unload。
>
> 整体来看，这段代码通过UF接口实现了NX中的实体减法运算，并将结果进行了编号显示，是一个典型的NX Open开发示例。
>
