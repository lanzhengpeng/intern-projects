### 【2988】translate components 组件平移

#### 代码

```cpp
    /*HEAD TRANSLATE_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_vec.h>  
    #include <uf_csys.h>  
    #include <uf_disp.h>  
    #include <uf_trns.h>  
    #include <uf_assem.h>  
    #include <uf_undo.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，它用于将错误日志打印到系统日志中。 */  
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
    static void vec_map_csys2abs(tag_t input_csys, double vec[3])  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(input_csys, &csys_mx, csys));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, &csys[3]));  
        FTN(uf5940)(csys, abs, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    /* qq3123197280 */  
    static int mask_for_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a component", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_components, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_vector(double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609("Enter vector parameters", menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void translate_component(tag_t comp, double vec[3])  
    {  
        tag_t  
            instance;  
        double  
            instance_origin[3],  
            instance_matrix[9],  
            instance_transform[4][4],  
            old_instance_origin[3];  
        char  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE];  
        instance = UF_ASSEM_ask_inst_of_part_occ(comp);  
        UF_CALL( UF_ASSEM_ask_component_data( instance, part_name, refset_name,  
            instance_name, old_instance_origin, instance_matrix,  
            instance_transform));  
        UF_VEC3_add(old_instance_origin, vec, instance_origin);  
        UF_CALL( UF_ASSEM_reposition_instance(instance, instance_origin,  
            instance_matrix));  
        if (ask_yes_or_no("Oops", "Put it back?"))  
            UF_CALL(UF_ASSEM_reposition_instance(instance, old_instance_origin,  
                instance_matrix));  
    }  
    static void do_it(void)  
    {  
        double  
            tmp[3],  
            vec[3] = { 1, 0, 0 };  
        tag_t  
            comp,  
            csys;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL_TAG, &mark));  
        while (((comp = select_a_component("Select a component")) != NULL_TAG) &&  
               ((csys = select_a_csys("Select a csys")) != NULL_TAG) &&  
                (ask_vector(vec)))  
        {  
            UF_VEC3_copy(vec, tmp);  
            vec_map_csys2abs(csys, tmp);  
            translate_component(comp, tmp);  
        }  
        if (ask_yes_or_no("Nevermind", "Back out all changes?"))  
            UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
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

> 这段代码是一个NX Open C++ API的二次开发示例，主要功能是平移装配体中的组件。
>
> 主要功能包括：
>
> 1. 错误报告函数：用于在函数调用失败时打印错误信息。
> 2. 坐标系转换函数：用于将向量从指定坐标系转换到绝对坐标系。
> 3. 选择组件：通过对话框选择组件，并设置选择掩码。
> 4. 选择坐标系：通过对话框选择坐标系，并设置选择掩码。
> 5. 询问向量参数：通过菜单对话框询问用户输入向量参数。
> 6. 询问Yes/No：通过对话框询问Yes/No问题。
> 7. 平移组件：根据输入的向量平移组件，并记录旧的实例原点。
> 8. 执行流程：通过循环选择组件、坐标系和向量，然后调用平移组件函数。如果用户选择撤回，则通过UNDO恢复到标记点。
> 9. 初始化和终止：在主函数中初始化和终止NX Open API。
>
> 整体来看，这段代码实现了在NX装配体环境下选择组件和坐标系，输入平移向量，然后平移组件的功能。
>
