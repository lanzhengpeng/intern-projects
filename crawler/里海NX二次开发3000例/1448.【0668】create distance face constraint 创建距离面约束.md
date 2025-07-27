### 【0668】create distance face constraint 创建距离面约束

#### 代码

```cpp
    /*HEAD CREATE_DISTANCE_FACE_CONSTRAINT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl_dfo.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static int select_faces(char *prompt, tag_t **faces)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select faces", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &cnt, faces));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*faces)[ii], FALSE));  
        return cnt;  
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
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void strip_trailing_zeros(char *s)  
    {  
        int ii;  
        if (strchr(s, '.'))  
        {  
            for (ii = strlen(s) - 1; s[ii] == '0'; ii--) s[ii] = '\0';  
            if (s[ii] == '.') s[ii] = '\0';  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feature,  
            ref_face;  
        double  
            dist = 0;  
        UF_MODL_dfo_region_t  
            region = { NULL, 0, NULL, 0, NULL, 0 };  
        UF_MODL_dfo_constraint_t  
            constraint = { UF_distance_dim, NULL_TAG, NULL_TAG, NULL_TAG,  
                "0" };  
        if (((region.num_seed = select_faces("Region Seed Faces",  
                &region.seed_faces)) > 0) &&  
            ((region.num_boundary = select_faces("Region Boundary Faces",  
                &region.boundary_faces)) > 0) &&  
            ((constraint.from_face = select_a_face("Target Face")) !=  
                NULL_TAG) &&  
            ((ref_face = select_planar_face("Constraint Reference")) !=  
                NULL_TAG) &&  
            prompt_for_a_number("Face Constraint", "Distance", &dist))  
        {  
            UF_CALL(UF_SO_create_dirr_plane(ref_face,  
                UF_SO_update_within_modeling, ref_face, FALSE,  
                &constraint.to_object));  
            sprintf(constraint.value, "%f", dist);  
            strip_trailing_zeros(constraint.value);  
            UF_CALL(UF_MODL_create_face_constraint(&region, &constraint,  
                &feature));  
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

> 这段代码是一个NX Open C++ API开发的程序，主要功能是创建距离面约束。
>
> 具体来说：
>
> 1. 包含了必要的头文件，并定义了错误报告函数report_error，用于在API调用失败时输出错误信息。
> 2. 定义了mask_for_faces函数，用于设置选择器以选择任意面。
> 3. 定义了select_faces函数，用于通过对话框选择多个面。
> 4. 定义了select_a_face函数，用于通过对话框选择一个面。
> 5. 定义了mask_for_planar_face函数，用于设置选择器以选择平面面。
> 6. 定义了select_planar_face函数，用于通过对话框选择一个平面面。
> 7. 定义了prompt_for_a_number函数，用于通过对话框提示用户输入一个数字。
> 8. 定义了strip_trailing_zeros函数，用于去除字符串尾部的0。
> 9. 定义了do_it函数，用于执行主体功能：
>
> 1. 定义了ufusr函数，作为程序的入口点，初始化NX Open，调用do_it函数，然后终止NX Open。
> 2. 定义了ufusr_ask_unload函数，用于卸载动态库。
>
> 总的来说，这段代码实现了选择面，并创建距离面约束的功能，利用了NX Open C++ API的选择器、对话框等功能，实现了交互式的约束创建。
>
