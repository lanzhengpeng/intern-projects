### 【2463】reverse normals on sheets 反转片体法线

#### 代码

```cpp
    /*HEAD REVERSE_NORMALS_ON_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <uf_so.h>  
    #include <uf_wave.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，用于在用户的本地日志中打印系统日志信息。 */  
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
    static tag_t ask_sheet_face(tag_t sheet)  
    {  
        int  
            n;  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            face,  
            *sheets,  
            rn_feat;  
        if ((n = select_sheets("Select sheets to reverse", &sheets)) > 0)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                face = ask_sheet_face(sheets[ii]);  
                display_mid_face_normal(face);  
            }  
            uc1601("Original Normals", TRUE);  
            UF_DISP_refresh();  
            for (ii = 0; ii < n; ii++)  
            {  
                UF_CALL(UF_MODL_create_reverse_normal(sheets[ii],1, &rn_feat));  
                printf("ii = %d\n", ii);  
            }  
            for (ii = 0; ii < n; ii++)  
            {  
                face = ask_sheet_face(sheets[ii]);  
                display_mid_face_normal(face);  
            }  
            uc1601("New Normals", TRUE);  
            UF_DISP_refresh();  
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

> 这段代码是用于NX的二次开发代码，其主要功能是：
>
> 1. 选择工作部件中的sheet体，并显示其正面法线。
> 2. 对选中的sheet体创建反向法线特征，并显示新的法线。
> 3. 通过UF_MODL_create_reverse_normal API实现sheet体法线的反转。
> 4. 通过自定义选择过滤器，只选择sheet类型的实体。
> 5. 使用UF_DISP_conehead API在实体上显示法线。
> 6. 通过错误报告函数，在出错时输出错误信息。
> 7. 使用UF_print_syslog API在系统日志中打印信息。
> 8. 使用UF_DISP_refresh API刷新视图。
> 9. 使用UF_UI_set_sel_mask等API实现自定义选择。
> 10. 考虑了用户交互，如打开选择窗口等。
>
> 总的来说，这段代码实现了选择sheet体、反转法线、显示法线等功能，通过自定义选择器、错误处理、用户交互等设计，实现了较好的用户体验。
>
