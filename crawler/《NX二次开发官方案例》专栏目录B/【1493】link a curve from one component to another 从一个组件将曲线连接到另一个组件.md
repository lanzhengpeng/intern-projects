### 【1493】link a curve from one component to another 从一个组件将曲线连接到另一个组件

#### 代码

```cpp
    /*HEAD LINK_A_CURVE_FROM_ONE_COMPONENT_TO_ANOTHER CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_so.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18版本中是新增的。 */  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t select_anything(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &object,cp,&view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_member;  
        int  
            ii,  
            n_sets,  
            view_dep;  
        tag_t  
            curve,  
            feat,  
            new_disp,  
            orig_work = UF_ASSEM_ask_work_part(),  
            orig_disp = UF_PART_ask_display_part(),  
            proto,  
            *sets,  
            to_obj,  
            to_proto,  
            view,  
            xform;  
        char  
            view_name[MAX_ENTITY_NAME_SIZE+1];  
        while (((curve = select_a_curve("Select curve to link")) != NULL_TAG) &&  
            ((to_obj = select_anything("Select object in destination component"))  
            != NULL_TAG))  
        {  
            if (UF_ASSEM_is_occurrence(curve) && UF_ASSEM_is_occurrence(to_obj))  
            {  
                proto = UF_ASSEM_ask_prototype_of_occ(curve);  
                to_proto = UF_ASSEM_ask_prototype_of_occ(to_obj);  
                UF_CALL(UF_SO_create_xform_assy_ctxt(to_proto,  
                    UF_ASSEM_ask_part_occurrence(curve),  
                    UF_ASSEM_ask_part_occurrence(to_obj), &xform));  
            /*  Make sure the prototype curve is NOT view dependent 里海译:请确保原型曲线与视角无关。 */  
                UF_CALL(uc6409(proto, &view_dep, view_name));  
                if (view_dep)  
                {  
                /*  In order to convert an object to model mode the display  
                    part and the work part must be the same */  
                    UF_CALL(UF_DISP_set_display(UF_DISP_SUPPRESS_DISPLAY));  
                    UF_CALL(UF_OBJ_ask_owning_part(proto, &new_disp));  
                    UF_CALL(UF_PART_set_display_part(new_disp));  
                    UF_CALL(UF_ASSEM_set_work_part(new_disp));  
                /*  The above call to uc6409 may not have gotten the view_name  
                    if the component was not fully loaded, so call it again */  
                    UF_CALL(uc6409(proto, &view_dep, view_name));  
                    UF_CALL(UF_VIEW_ask_tag_of_view_name(view_name, &view));  
                /*  In order to convert an object to model mode it cannot be  
                    a member of any reference sets */  
                    UF_CALL(UF_ASSEM_is_ref_set_member(proto, &is_member));  
                    if (is_member)  
                    {  
                        UF_CALL(UF_ASSEM_ask_ref_sets(proto, &n_sets, &sets));  
                        for (ii = 0; ii < n_sets; ii++)  
                            UF_CALL(UF_ASSEM_remove_ref_set_members(sets[ii],  
                                1, &proto));  
                    }  
                /*  Convert the object to model mode 里海译:将对象转换为模型模式 */  
                    UF_CALL(UF_VIEW_convert_to_model(view, proto));  
                /*  Put it back into the original reference sets 里海译:根据上下文，Put it back into the original reference sets的意思是将其放回原来的参考集中。 */  
                    if (is_member)  
                    {  
                        for (ii = 0; ii < n_sets; ii++)  
                            UF_CALL(UF_ASSEM_add_ref_set_members(sets[ii],  
                                1, &proto));  
                    }  
                    if (n_sets > 0) UF_free(sets);  
                /*  Set the display and work part back as they were 里海译:将显示和工作部分恢复原状。 */  
                    UF_CALL(UF_PART_set_display_part(orig_disp));  
                    UF_CALL(UF_ASSEM_set_work_part(orig_work));  
                    UF_CALL(UF_DISP_set_display(UF_DISP_UNSUPPRESS_DISPLAY));  
                }  
            /*  Now the linked feature can be created 里海译:现在可以创建关联功能了。 */  
                UF_CALL(UF_WAVE_create_linked_curve(proto, xform, to_proto, TRUE,  
                    &feat));  
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

> 这段代码是用于NX二次开发的UF函数，主要功能是实现从一个组件中的曲线链接到另一个组件的曲线。代码的主要步骤如下：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个用于选择曲线的函数select_a_curve，它使用对话框提示用户选择一个曲线，并返回曲线的tag。
> 3. 定义了一个用于选择任意对象的函数select_anything，同样使用对话框提示用户选择一个对象，并返回对象的tag。
> 4. 主函数do_it()循环调用select_a_curve和select_anything，获取源曲线和目标对象。然后检查它们是否都是组件实例，如果是，获取它们的原型，并建立变换关系。
> 5. 检查原型曲线是否依赖于视图，如果是，则转换为模型模式，移除参考集成员关系，转换后重新添加到参考集。
> 6. 使用UF_WAVE_create_linked_curve函数创建链接曲线特征。
> 7. ufusr函数是NX二次开发的入口函数，初始化后调用do_it()，然后终止。
> 8. ufusr_ask_unload函数用于卸载，返回立即卸载。
>
> 总体来说，这段代码通过用户交互选择源曲线和目标对象，然后进行转换和链接，实现曲线从一个组件到另一个组件的关联。
>
