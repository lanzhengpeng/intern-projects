### 【1252】extrude selected faces 挤压选定面

#### 代码

```cpp
    /*HEAD EXTRUDE_SELECTED_FACES CCC UFUN */  
    #include <stdio.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog是V18版本新增的功能。 */  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii, jj,  
            n_faces;  
        tag_t  
            *faces;  
        double  
            point[3],  
            vec[3];  
        char  
            *limits[2] = { "0", "2" };  
        uf_list_p_t  
            ext_list,  
            edge_list,  
            face_list,  
            feat_list,  
            tmp;  
        if (((n_faces = select_faces("Extrude faces", &faces)) > 0) &&  
            specify_vector("Extrude", vec, point))  
        {  
        /*  Extruding the faces may fail - see PR 1308910 里海译:挤压面可能会失败 - 参见PR 1308910 */  
            face_list = make_a_list(n_faces, faces);  
            if (!UF_MODL_create_extruded(face_list, "0", limits, point, vec,  
                UF_NULLSIGN, &feat_list) && (feat_list != NULL))  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
        /*  The work around is to extrude the non-shared edges of the faces 里海译:绕过的方法是将面的非共享边缘挤出。 */  
            else  
            {  
                UF_CALL(UF_MODL_create_list(&ext_list));  
                for (ii = 0; ii < n_faces; ii++)  
                {  
                    UF_CALL(UF_MODL_ask_face_edges(faces[ii], &edge_list));  
                    for (tmp = edge_list; tmp != NULL; tmp = tmp->next)  
                        UF_CALL(UF_MODL_put_list_item(ext_list, tmp->eid));  
                    UF_CALL(UF_MODL_delete_list(&edge_list));  
                }  
                for (ii = 0; ii < n_faces - 1; ii++)  
                    for (jj = ii + 1; jj < n_faces; jj++)  
                    {  
                        UF_CALL(UF_MODL_ask_shared_edges(faces[ii], faces[jj],  
                            &edge_list));  
                        for (tmp = edge_list; tmp != NULL; tmp = tmp->next)  
                            UF_CALL(UF_MODL_delete_list_item(&ext_list, tmp->eid));  
                        UF_CALL(UF_MODL_delete_list(&edge_list));  
                    }  
                if (!UF_CALL(UF_MODL_create_extruded(ext_list, "0", limits, point,  
                    vec, UF_NULLSIGN, &feat_list)) && (feat_list != NULL))  
                    UF_CALL(UF_MODL_delete_list(&feat_list));  
                UF_CALL(UF_MODL_delete_list(&ext_list));  
            }  
            UF_CALL(UF_MODL_delete_list(&face_list));  
            UF_free(faces);  
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

> 这段代码是用于NX的二次开发代码，主要功能是挤出用户选择的面。具体介绍如下：
>
> 1. 功能介绍：用户可以通过选择对话框选择需要挤出的面，并指定挤出的方向和距离。代码会根据这些输入，创建一个挤出特征。
> 2. 关键步骤：定义一个选择器，用于只选择面类型的实体。使用选择对话框让用户选择要挤出的面，并获取这些面的tag。根据用户指定的方向和距离创建一个挤出特征。如果直接挤出面失败，则尝试挤出这些面的非共享边，并删除这些边与其他面的共享边，以避免重复挤出。
> 3. 定义一个选择器，用于只选择面类型的实体。
> 4. 使用选择对话框让用户选择要挤出的面，并获取这些面的tag。
> 5. 根据用户指定的方向和距离创建一个挤出特征。
> 6. 如果直接挤出面失败，则尝试挤出这些面的非共享边，并删除这些边与其他面的共享边，以避免重复挤出。
> 7. 错误处理：代码中使用了UF_CALL宏来包装NX API调用，并在调用失败时打印错误信息。
> 8. 注意事项：直接挤出面可能会失败，因此需要处理这种情况。在挤出面的非共享边时，需要删除这些边与其他面的共享边，以避免重复挤出。
> 9. 直接挤出面可能会失败，因此需要处理这种情况。
> 10. 在挤出面的非共享边时，需要删除这些边与其他面的共享边，以避免重复挤出。
> 11. 代码结构：代码包括选择器定义、选择对话框、创建挤出特征、错误处理、注意事项等部分。整体结构清晰，功能明确。
> 12. 代码质量：代码质量较高，函数定义清晰，错误处理完善，适合作为NX二次开发的参考。
>
