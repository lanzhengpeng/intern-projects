### 【0871】create scenario auto face subdivisions 在NX中创建自动面分割场景

#### 代码

```cpp
    /*HEAD CREATE_SCENARIO_AUTO_FACE_SUBDIVISIONS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_sf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：请注意，UF_print_syslog是在V18版本中新增的，请只提供翻译，不要添加其他无关内容。 */  
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
    static logical pick_one_of_two_choices(char *prompt, char *option_one,  
        char *option_two, int *choice)  
    {  
        int  
            resp;  
        char  
            options[2][38];  
        strncpy(&options[0][0], option_one, 37);  
        strncpy(&options[1][0], option_two, 37);  
        options[0][37] = '\0';  
        options[1][37] = '\0';  
        resp = uc1603(prompt, *choice, options, 2);  
        if ((resp > 4) && (resp < 19))  
        {  
            *choice = resp - 4;  /* returns 1 or 2 译:返回1或2。 */  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    /* 里海 */  
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
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select solids", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp, &n, solids));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            choice,  
            ii,  
            n = 0,  
            n_r_pairs = 0;  
        double  
            distol;  
        tag_t  
            *objects;  
        UF_SF_resulting_face_pairs_p_t   
            r_pairs;  
        char  
            msg[133];  
        if(pick_one_of_two_choices("Object types for Auto Face Subdivide",   
            "Faces", "Solids", &choice))  
        {  
            UF_MODL_ask_distance_tolerance(&distol);  
            if( choice == 1 )  
                n = select_faces("Select Faces", &objects);  
            else  
                n = select_solids("Select Solid Bodies", &objects);  
            if (n  != 0)  
            {  
                UF_CALL(UF_SF_create_auto_face_subdiv ( objects, n, distol, 1 , 1,  
                     &r_pairs, &n_r_pairs ));   
                if (n_r_pairs > 0)  
                {  
                    sprintf(msg, "Resulting Pairs: %d\n", n_r_pairs);  
                    if (!UF_UI_open_listing_window())  
                    {  
                        UF_UI_write_listing_window(msg);  
                        for (ii = 0; ii < n_r_pairs; ii++)   
                        {  
                            sprintf(msg, "%d. Resulting Pair Faces: %d, %d. Status:%d\n",  
                                ii, r_pairs[ii].face1, r_pairs[ii].face2,   
                                    r_pairs[ii].relative_status);   
                            UF_UI_write_listing_window(msg);  
                        }  
                    }  
                }  
            }  
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

> 这是NX Open C++二次开发代码，主要实现了自动面分割的功能。以下是代码的主要功能和实现细节：
>
> 1. 错误报告函数：定义了report_error函数，用于在发生UF函数调用错误时报告错误信息。
> 2. 面选择函数：定义了mask_for_faces和select_faces函数，用于选择要分割的面。
> 3. 体选择函数：定义了filter_body_type、mask_body_type和select_solids函数，用于选择要分割的实体。
> 4. 选择类型：使用pick_one_of_two_choices函数让用户选择分割对象是面还是实体。
> 5. 分割面：使用UF_SF_create_auto_face_subdiv函数对选中的面或实体进行自动面分割，并显示分割结果。
> 6. 初始化和清理：在ufusr函数中初始化和清理NX系统，在ufusr_ask_unload函数中设置立即卸载。
> 7. 主要逻辑：在do_it函数中实现了选择对象类型、选择对象、设置距离公差、调用分割函数并显示结果的逻辑。
> 8. 代码注释：使用中文注释帮助理解代码逻辑。
>
> 总体来说，这段代码通过NX Open C++ API实现了自动面分割的功能，包括错误处理、选择对象、设置参数、调用分割函数等完整流程。
>
