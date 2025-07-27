### 【0722】create associative line through point normal to face 创建穿过点且垂直于面的相关联直线

#### 代码

```cpp
    /*HEAD CREATE_ASSOCIATIVE_LINE_THROUGH_POINT_NORMAL_TO_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_curve.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        UF_CURVE_line_arc_t  
            data;  
        data.curve_type = UF_CURVE_asso_line;  
        data.arc_constraint_subtype = 0;  // NOT USED  
        data.constraints[0].constraint_type = UF_CURVE_coincident;  
        data.constraints[0].end_type = UF_CURVE_start;  
        data.constraints[0].object_tag = NULL_TAG; // selected later  
        data.constraints[0].value = 0.000000;  
        data.constraints[1].constraint_type = UF_CURVE_normal;  
        data.constraints[1].end_type = UF_CURVE_end;  
        data.constraints[1].object_tag = NULL_TAG; // selected later  
        data.constraints[1].value = 0.000000;  
        data.constraints[1].help_data.help_data_type = UF_CURVE_help_data_value;  
        data.constraints[1].help_data.value[0] = 0.000000;  
        data.constraints[1].help_data.value[1] = 0.000000;  
        data.constraints[1].help_data.value[2] = 0.000000;  
        data.limits[0].limit_type = UF_CURVE_limit_to_constraint;  
        data.limits[0].help_data.help_data_type = UF_CURVE_help_data_none;  
        data.limits[1].limit_type = UF_CURVE_limit_value;  
        data.limits[1].value = 1.500000;  
        data.limits[1].help_data.help_data_type = UF_CURVE_help_data_none;  
        data.plane_of_curve = NULL_TAG;  
        data.complement = FALSE;  
        data.closed = FALSE;  
        data.is_associative = TRUE;  
        while (((data.constraints[0].object_tag =  
            select_a_point("Line Thru Point")) != NULL_TAG) &&  
            ((data.constraints[1].object_tag =  
            select_a_face("Line Normal Face")) != NULL_TAG))  
        {  
            set_undo_mark("Line Thru Point Normal To Face");  
            UF_CALL(UF_CURVE_create_line_arc(&data, &feat));  
        //  See PR 6266084 - extraneous plane is displayed  
            UF_CALL(UF_DISP_regenerate_display());  
        }  
    }  
    /* 里海 */  
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

> 这段NX二次开发代码的功能是创建一条通过点且垂直于面的关联线。主要步骤包括：
>
> 1. 定义了错误报告函数report_error，用于输出错误信息。
> 2. 定义了两个选择函数select_a_point和select_a_face，用于选择点和面。
> 3. 定义了设置撤销标记的函数set_undo_mark。
> 4. 主函数do_it中，首先定义了关联线的数据结构，包括线类型、两个约束条件（通过点、垂直于面）等。
> 5. 循环调用选择点面函数，设置约束条件，然后调用UF_CURVE_create_line_arc创建关联线。
> 6. ufusr函数初始化NX，调用do_it，然后终止NX。
> 7. ufusr_ask_unload函数设置为立即卸载二次开发程序。
>
> 通过这段代码，用户可以方便地在NX中创建满足特定条件的关联线。
>
