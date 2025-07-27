### 【0730】create intersection curves between target and multiple tool solids ii 创建目标和多个刀具实体的交线

#### 代码

```cpp
    /*HEAD CREATE_INTERSECTION_CURVES_BETWEEN_TARGET_AND_MULTIPLE_TOOL_SOLIDS_II CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog在V18中是新增的。 */  
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
    static int mask_for_solid_bodies( UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_SOLID_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solid_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int select_solids(char *prompt, tag_t **solids)  
    {  
        int  
            ii,  
            n,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select solids", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_solid_bodies, NULL,  
            &resp, &n, solids));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*solids)[ii], FALSE));  
        return n;  
    }  
    static void do_it(void)  
    {  
        int  
            flag,  
            ii,  
            n,  
            n_tools;  
        tag_t  
            target,  
            *tools;  
        double  
            tol;  
        uf_list_p_t  
            jj, kk,  
            target_faces,  
            tool_faces;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((target = select_a_solid("Select Target Body")) != NULL_TAG) &&  
               ((n_tools = select_solids("Select Tool Bodies", &tools)) > 0))  
        {  
            UF_CALL(UF_MODL_ask_body_faces(target, &target_faces));  
            for (ii = 0; ii < n_tools; ii++)  
            {  
                UF_CALL(UF_MODL_check_interference(target, 1, &tools[ii], &flag));  
                if (flag == 1)  
                {  
                    UF_CALL(UF_MODL_ask_body_faces(tools[ii], &tool_faces));  
                    for (jj = target_faces; jj != NULL; jj = jj->next)  
                        for (kk = tool_faces; kk != NULL; kk = kk->next)  
                        {  
                            UF_CALL(UF_MODL_intersect_objects(jj->eid, kk->eid,  
                                tol, &n, &intersections));  
                            if (n > 0) UF_free(intersections);  
                        }  
                    UF_CALL(UF_MODL_delete_list(&tool_faces));  
                }  
            }  
            UF_CALL(UF_MODL_delete_list(&target_faces));  
            UF_free(tools);  
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

> 这段代码是一个NX Open C++二次开发示例，用于创建目标体和多个工具体之间的交线曲线。其主要功能包括：
>
> 1. 包含了必要的NX Open头文件。
> 2. 定义了错误报告函数report_error，用于打印错误信息。
> 3. 定义了选择实体的函数select_a_solid和select_solids，用于选择目标实体和工具实体。
> 4. 定义了执行主体逻辑的函数do_it，其中：获取系统默认的公差值。循环选择目标体和工具体。获取目标体和工具体的面列表。检查目标体和工具体是否干涉，若干涉，则计算它们之间的交线。
> 5. 获取系统默认的公差值。
> 6. 循环选择目标体和工具体。
> 7. 获取目标体和工具体的面列表。
> 8. 检查目标体和工具体是否干涉，若干涉，则计算它们之间的交线。
> 9. 定义了ufusr函数作为主入口点，初始化NX Open，调用do_it函数，然后终止NX Open。
> 10. 定义了卸载函数ufusr_ask_unload。
>
> 总的来说，这段代码通过选择目标体和工具体，计算它们之间的交线，并最终创建交线曲线，实现了NX二次开发中的交线功能。
>
