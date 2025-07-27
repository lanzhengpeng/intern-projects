### 【0613】create auto face subdivision and mate mesh 创建自动面分割并匹配网格

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_sf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
            *choice = resp - 4;  /* returns 1 or 2 译:根据上下文，returns 1 or 2的翻译是：返回1或2。 */  
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
            asm_mesh,  
            *objects;  
        UF_SF_resulting_face_pairs_p_t  
            r_pairs;  
        UF_SF_assembly_type_t  
            astype = 1;  
        char  
            mname[133],  
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
                    ECHO("Resulting Pairs: %d\n", n_r_pairs);  
                    for (ii = 0; ii < n_r_pairs; ii++)  
                    {  
                        ECHO("%d. Resulting Pair Faces: %d, %d. Status:%d\n",  
                            ii, r_pairs[ii].face1, r_pairs[ii].face2,  
                                r_pairs[ii].relative_status);  
                        sprintf(mname, "mymesh%d", ii);  
                        UF_CALL(UF_SF_create_mesh_mating_condition(r_pairs[ii].face1,  
                              r_pairs[ii].face2, mname, 0, astype, distol,  
                            distol, &asm_mesh));  
                        ECHO("New Mating Mesh: %s, Tag: %d\n", mname, asm_mesh);  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 自动面分割功能：用户可以选择要分割的面或实体，然后调用UF_SF_create_auto_face_subdiv函数对选择的几何体进行自动面分割，并输出分割结果的面对。
> 2. 网格匹配条件创建：对每一对面，调用UF_SF_create_mesh_mating_condition函数创建网格匹配条件，用于后续的网格装配。
> 3. 用户交互：通过弹窗让用户选择分割对象类型（面或实体），并调用相应的选择函数进行选择。同时，通过自定义的函数来设置选择过滤。
> 4. 错误处理和日志输出：使用宏UF_CALL来包装NX API调用，捕获并打印错误信息。使用自定义函数ECHO来输出日志信息到列表窗口和系统日志。
> 5. 初始化和清理：在ufusr函数中调用UF_initialize进行初始化，调用UF_terminate进行清理。另外，提供了卸载函数ufusr_ask_unload来立即卸载用户库。
> 6. 自定义函数：包括用于选择实体的mask_body_type、用于选择面的mask_for_faces，以及用于创建网格匹配条件的函数等。
>
> 总体来说，这段代码通过自动面分割和网格匹配条件创建，实现了NX中的几何体网格装配准备功能，并通过用户交互、错误处理等机制增强了用户体验。
>
