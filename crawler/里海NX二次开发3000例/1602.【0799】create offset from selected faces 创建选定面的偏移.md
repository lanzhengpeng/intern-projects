### 【0799】create offset from selected faces 创建选定面的偏移

#### 代码

```cpp
    /*HEAD CREATE_OFFSET_FROM_SELECTED_FACES CCC UFUN */  
    /*Create Offset Faces from all selected faces*/  
    /*This Workaround to PR-1351918 uses the class selection dialog*/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog 是 V18 中新增的函数。它用于打印系统日志消息到文件。 */  
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
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
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
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *faces,  
            feat_id;  
        double  
            distance = 1.0;  
        uf_list_p_t  
            face_list;  
        char  
            ca2[133];  
        while (((n = select_faces("Select faces", &faces)) != 0)  
            && prompt_for_a_number("Offset", "Distance", &distance))  
        {  
            sprintf(ca2, "%f", distance);  
            face_list = make_a_list(n, faces);  
            UF_CALL( UF_MODL_create_face_offset( ca2, face_list, &feat_id));  
            UF_CALL( UF_MODL_delete_list( &face_list) );  
            UF_free(faces);  
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

> 根据代码内容，这段NX二次开发代码的主要功能是从NX模型中选择面，并创建这些面的偏置面。具体来说，代码流程如下：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义面选择掩码函数mask_for_faces，用于设置选择对话框的面选择掩码。
> 4. 定义面选择函数select_faces，用于让用户在对话框中选择面。
> 5. 定义创建列表函数make_a_list，用于将选中的面数组转换为NX列表。
> 6. 定义数字输入函数prompt_for_a_number，用于提示用户输入偏置距离。
> 7. 定义主体函数do_it，循环让用户选择面，并提示输入偏置距离，然后调用NX API创建偏置面。
> 8. 定义ufusr函数，初始化NX，调用do_it函数，然后结束NX。
> 9. 定义卸载函数ufusr_ask_unload，用于在卸载时返回立即卸载标志。
>
> 总体来说，这段代码通过对话框实现了选择面、输入偏置距离、创建偏置面的功能，具有较好的用户交互性。
>
