### 【1244】extract face with holes deleted 提取无孔洞的面

#### 代码

```cpp
    /*HEAD EXTRACT_FACE_WITH_HOLES_DELETED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_wave.h>  
    #include <uf_undo.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
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
    /* qq3123197280 */  
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
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static void remove_object_associativity(tag_t object)  
    {  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        UF_CALL(UF_MODL_put_list_item(list, object));  
        UF_CALL(UF_MODL_delete_object_parms(list));  
        UF_CALL(UF_MODL_delete_list(&list));  
    }  
    static tag_t extract_face_without_holes(tag_t face)  
    {  
        int  
            n;  
        tag_t  
            *faces,  
            link_sheet,  
            link_feat,  
            sheet;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_extract_face(face, 0, &sheet));  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        n = make_an_array(&face_list, &faces);  
        if (n > 0)  
        {  
            UF_CALL(UF_WAVE_create_linked_region(n, faces, 0, NULL,  
                NULL_TAG, face, TRUE, FALSE, TRUE, &link_feat));  
            UF_free(faces);  
        }  
        UF_CALL(UF_WAVE_ask_linked_feature_geom(link_feat, &link_sheet));  
        remove_object_associativity(link_sheet);  
        UF_CALL(UF_OBJ_delete_object(sheet));  
        return link_sheet;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face,  
            sheet;  
        while ((face = select_a_face("Select face to extract")) != NULL_TAG)  
        {  
            set_undo_mark("Extract Face w/no Holes");  
            sheet = extract_face_without_holes(face);  
            UF_CALL(UF_DISP_set_highlight(sheet, TRUE));  
            uc1601("New sheet", TRUE);  
            UF_CALL(UF_DISP_set_highlight(sheet, FALSE));  
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

> 这段代码是NX的二次开发代码，主要实现了从实体中提取无孔洞的面的功能。
>
> 主要功能包括：
>
> 1. 选择一个实体面。
> 2. 提取该面，生成一个薄片实体。
> 3. 删除薄片实体与其他实体的关联性。
> 4. 删除原始生成的薄片实体，保留提取出的无孔洞面。
> 5. 高亮显示提取出的无孔洞面。
> 6. 循环以上步骤，直到用户取消选择。
>
> 关键步骤包括：
>
> 1. 使用UF_UI库中的函数，弹出选择对话框，让用户选择一个实体面。
> 2. 调用UF_MODL_extract_face函数，提取该面生成一个薄片实体。
> 3. 获取薄片上的所有面，使用UF_WAVE库中的函数创建一个关联区域，提取无孔洞面。
> 4. 删除无孔洞面与其他实体的关联性。
> 5. 删除原始生成的薄片实体。
> 6. 使用UF_DISP库中的函数高亮显示提取出的无孔洞面。
>
> 此外，代码中还使用了UF_UNDO库来设置撤销标记，以便用户可以撤销操作。
>
