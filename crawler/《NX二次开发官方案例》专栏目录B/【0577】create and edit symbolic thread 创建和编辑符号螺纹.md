### 【0577】create and edit symbolic thread 创建和编辑符号螺纹

#### 代码

```cpp
    /*HEAD CREATE_AND_EDIT_SYMBOLIC_THREAD CCC UFUN */  
    /*  
        the methods used here should work in Unigraphics NX V1.0.1 - See PR 1174555  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_types.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档描述，UF_print_syslog是V18版本新增的函数，主要用于打印系统日志信息。该函数目前没有提供更详细的实现细节或示例。 */  
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
    /* 里海 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            hole,  
            plane,  
            sym_thread;  
        UF_MODL_symb_thread_data_t  
            thread;  
        if (((hole = select_a_cylindrical_face("Pick Hole Face")) != NULL_TAG) &&  
            ((plane = select_planar_face("Select Top Face")) != NULL_TAG))  
        {  
            thread.cyl_face = hole;  
            thread.start_face = plane;  
            thread.axis_direction[0]=0;  
            thread.axis_direction[1]=0;  
            thread.axis_direction[2]=-1;  
            thread.include_instances = UF_MODL_INCL_INSTANCES;  
            thread.rotation = UF_MODL_RIGHT_HAND;  
            thread.length_flag = UF_MODL_FIXED_LENGTH;  
            thread.tapered = UF_MODL_NON_TAPERED;  
            thread.num_starts = 1;  
            thread.length= ".75";  
            thread.form = "Unified";  
            thread.method = "CUT";  
            thread.callout = NULL;  
            thread.major_dia = "0.3125";  
            thread.tapped_dia = "0.2674";  
            thread.minor_dia = "0.2464";  
            thread.pitch = "0.0556";  
            thread.angle = "60";  
            UF_CALL(UF_MODL_create_symb_thread(&thread, &sym_thread));  
            thread.length= "1.0";  
            thread.form = NULL;  
            thread.method = NULL;  
            thread.major_dia = NULL;  
            thread.tapped_dia = NULL;  
            thread.minor_dia = NULL;  
            thread.pitch = NULL;  
            thread.angle = NULL;  
            UF_CALL(UF_MODL_edit_symb_thread(sym_thread, &thread));  
            UF_CALL(UF_MODL_update());  
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

> 这段代码是一个NX的二次开发示例，主要实现了创建和编辑符号螺纹的功能。
>
> 主要步骤包括：
>
> 1. 包含必要的头文件，定义了UF_CALL宏用于简化错误处理。
> 2. 实现了report_error函数，用于打印错误信息。
> 3. 实现了mask_for_cylindrical_faces和mask_for_planar_face函数，用于设置选择过滤条件，分别用于选择圆柱面和平面。
> 4. 实现了select_a_cylindrical_face和select_planar_face函数，用于让用户选择圆柱面和平面。
> 5. 在do_it函数中，首先调用select_a_cylindrical_face和select_planar_face函数让用户选择圆柱面和平面。
> 6. 设置UF_MODL_symb_thread_data_t结构体，填充螺纹参数，并调用UF_MODL_create_symb_thread创建符号螺纹。
> 7. 修改螺纹参数，调用UF_MODL_edit_symb_thread编辑符号螺纹。
> 8. 调用UF_MODL_update更新模型。
> 9. ufusr函数中初始化NX环境，调用do_it函数执行创建和编辑螺纹，然后终止NX环境。
> 10. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 该代码利用NX提供的螺纹API，实现了选择面、创建和编辑螺纹的功能，展示了NX二次开发的基本流程。
>
