### 【1397】highlight planar faces on selected body 高亮选中实体的平面面

#### 代码

```cpp
    /*HEAD HIGHLIGHT_PLANAR_FACES_ON_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的函数。 */  
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
    static void set_highlight_object_list(uf_list_p_t list, int flag)  
    {  
        uf_list_p_t  
            tmp;  
        for (tmp = list; tmp != NULL; tmp = tmp->next)  
            UF_CALL(UF_DISP_set_highlight(tmp->eid, flag));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            face_count,  
            face_type,  
            total_faces;  
        tag_t  
            face,  
            body;  
        char  
            msg[MAX_LINE_SIZE+1];  
        uf_list_p_t  
            face_list;  
        while ((body = select_a_solid("Highlight planar faces")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
            UF_CALL(UF_MODL_ask_list_count(face_list, &total_faces));  
            for (ii = 0, face_count = total_faces; ii < face_count; ii++)  
            {  
                UF_CALL(UF_MODL_ask_list_item(face_list, ii, &face));  
                UF_CALL(UF_MODL_ask_face_type(face, &face_type));  
                if (face_type != UF_MODL_PLANAR_FACE)  
                {  
                    UF_CALL(UF_MODL_delete_list_item(&face_list, face));  
                    ii--;  
                    face_count--;  
                }  
            }  
            set_highlight_object_list(face_list, TRUE);  
            sprintf(msg, "%d planar faces highlighted", face_count);  
            uc1601(msg, TRUE);  
            set_highlight_object_list(face_list, FALSE);  
            UF_CALL(UF_MODL_delete_list(&face_list));  
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

> 这段代码是用于NX的二次开发，主要功能是在NX中选择一个实体，并高亮显示其上的平面面。
>
> 主要步骤如下：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并在出错时输出错误信息。
> 3. 定义了filter_body_type函数，用于过滤出实体类型。
> 4. 定义了mask_body_type函数，用于设置选择过滤器。
> 5. 定义了select_a_solid函数，用于选择一个实体。
> 6. 定义了set_highlight_object_list函数，用于设置对象的高亮显示。
> 7. 定义了do_it函数，用于选择一个实体，获取其上的平面面，并高亮显示。
> 8. ufusr函数是主函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. ufusr_ask_unload函数用于在卸载用户函数时立即卸载。
>
> 总的来说，这段代码实现了选择实体，过滤出平面面，并进行高亮显示的功能，属于一个典型的NX二次开发示例。
>
