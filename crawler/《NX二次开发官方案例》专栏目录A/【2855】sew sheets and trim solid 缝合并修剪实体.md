### 【2855】sew sheets and trim solid 缝合并修剪实体

#### 代码

```cpp
    /*HEAD SEW_SHEETS_AND_TRIM_SOLID CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据您提供的信息，UF_print_syslog是V18版本中新增的函数。对于该函数的翻译，我们可以直接翻译为：

UF_print_syslog 在 V18 版本是新增的。

这样既简洁又准确地传达了原文的意思。 */  
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
    static int select_sheets(char *prompt, tag_t **sheets)  
    {  
        int  
            ii,  
            n,  
            sheet = UF_MODL_SHEET_BODY,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog( "Select sheet bodies", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &sheet, &resp, &n, sheets));  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight((*sheets)[ii], FALSE));  
        return n;  
    }  
    static tag_t select_a_solid(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            solid = UF_MODL_SOLID_BODY,  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog( "Select a solid", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART, mask_body_type, &solid, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_feat_first_face(tag_t feat)  
    {  
        int  
            n;  
        tag_t  
            face;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        if (n > 0)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            return face;  
        }  
        else  
            return NULL_TAG;  
    }  
    static void display_mid_face_normal(tag_t face)  
    {  
        double  
            parm[2],  
            pnt[3],  
            junk[3],  
            norm_dir[3],  
            uvs[4];  
        UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
        parm[0] = (uvs[0] + uvs[1]) / 2;  
        parm[1] = (uvs[2] + uvs[3]) / 2;  
        UF_CALL(UF_MODL_ask_face_props(face, parm, pnt, junk, junk, junk,  
            junk, norm_dir, junk));  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, pnt, norm_dir, 0);  
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
    static void do_it(void)  
    {  
        logical  
            rev_dir;  
        int  
            n_sheets;  
        tag_t  
            sew = NULL_TAG,  
            *sheets,  
            solid,  
            trim,  
            trim_face;  
        double  
            tol;  
        uf_list_p_t  
            disjoint_list = NULL;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((n_sheets = select_sheets("Select sheets to sew", &sheets)) > 1)  
            && ((solid = select_a_solid("Select solid body to trim")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_create_sew(0, 1, sheets, n_sheets-1, &sheets[1], tol,  
                0, &disjoint_list, &sew));  
            if (disjoint_list != NULL)  
                UF_CALL(UF_MODL_delete_list(&disjoint_list));  
            trim_face = ask_feat_first_face(sew);  
            display_mid_face_normal(trim_face);  
            rev_dir = ask_yes_or_no("Trim Body", "Reverse Default Direction");  
            UF_DISP_refresh();  
            UF_CALL(UF_MODL_trim_body(solid, trim_face, rev_dir, &trim));  
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

> 根据提供的代码，这是一段用于NX二次开发的代码，其主要功能如下：
>
> 1. 包含必要的头文件，如stdio.h、string.h、uf.h等。
> 2. 定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 3. 定义了一个过滤函数filter_body_type，用于在选择时过滤出特定类型的body。
> 4. 定义了一个设置选择掩码的函数mask_body_type。
> 5. 定义了一个选择sheet体的函数select_sheets。
> 6. 定义了一个选择单个solid体的函数select_a_solid。
> 7. 定义了一个获取特征第一个面的函数ask_feat_first_face。
> 8. 定义了一个显示面法向量的函数display_mid_face_normal。
> 9. 定义了一个询问Yes/No的函数ask_yes_or_no。
> 10. 定义了一个主函数do_it，用于执行选择sheet体，创建缝合体，选择solid体，选择缝合体的第一个面，显示法向量，询问是否反向，然后修剪solid体的操作。
> 11. 定义了ufusr函数，作为二次开发的入口函数，调用do_it函数。
> 12. 定义了询问卸载函数ufusr_ask_unload。
>
> 综上所述，这段代码的主要功能是让用户选择sheet体进行缝合，然后选择一个solid体，并修剪solid体到缝合体的第一个面上。它包含了错误处理、选择过滤、创建缝合体、显示法向量、询问操作等完整流程。
>
