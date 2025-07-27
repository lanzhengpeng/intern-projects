### 【2923】split bodies 使用指定面、平面或基准面将实体分割成多个部分的函数

#### 代码

```cpp
    /*HEAD SPLIT_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog is new in V18。 */  
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
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_bodies(char *prompt, tag_t **bodies)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp, &cnt,  
            bodies));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*bodies)[ii], FALSE));  
        return cnt;  
    }  
    /* qq3123197280 */  
    static int mask_for_face_plane_datums(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_a_face_plane_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, mask_for_face_plane_datums, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            num_bodies,  
            num_split_bodies;  
        tag_t  
            *bodies,  
            cutting_body,  
            *split_bodies;  
        while ((cutting_body = select_a_face_plane_datum(  
                "Select faces, planes, datums to split with")) &&  
                (num_bodies = select_bodies("Select bodies", &bodies)))  
        {  
            UF_CALL(UF_MODL_split_body (num_bodies, bodies, cutting_body,  
                &num_split_bodies, &split_bodies));  
            for (ii = 0; ii < num_split_bodies; ii++)  
                UF_DISP_set_highlight(split_bodies[ii], TRUE);  
            uc1601("Split Completed ", TRUE);  
            for (ii = 0; ii < num_split_bodies; ii++)  
                UF_DISP_set_highlight(split_bodies[ii], FALSE);  
            UF_free(split_bodies);  
            UF_free(bodies);  
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

> 这段代码是NX二次开发中的一个示例，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在函数执行失败时输出错误信息。
> 2. 定义了两个选择掩码函数mask_for_bodies和mask_for_face_plane_datums，分别用于在对话框中选择实体和面/平面/基准面。
> 3. 定义了两个选择函数select_bodies和select_a_face_plane_datum，分别用于选择多个实体和一个面/平面/基准面。
> 4. 定义了主函数do_it，循环选择切割面和被切割实体，然后调用UF_MODL_split_body函数进行分割，并高亮显示分割后的实体。
> 5. 定义了ufusr函数，用于初始化NX环境，调用主函数，然后终止NX环境。
> 6. 定义了卸载函数ufusr_ask_unload，返回立即卸载标志。
>
> 总的来说，这段代码实现了一个简单的NX二次开发示例，用于选择一个面/平面/基准面和一个或多个实体，然后进行分割，并高亮显示分割后的结果。
>
