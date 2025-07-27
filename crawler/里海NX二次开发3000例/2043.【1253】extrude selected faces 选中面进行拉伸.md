### 【1253】extrude selected faces 选中面进行拉伸

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
        /*  Extruding the faces may fail - see PR 1308910 里海译:翻译：挤出面可能会失败，请参阅PR 1308910。 */  
            face_list = make_a_list(n_faces, faces);  
            if (!UF_MODL_create_extruded(face_list, "0", limits, point, vec,  
                UF_NULLSIGN, &feat_list) && (feat_list != NULL))  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
        /*  The work around is to extrude the non-shared edges of the faces 里海译:翻译：解决这个问题的方法是拉伸非共享的边。 */  
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

> 这段代码是一个NX Open C++程序，用于执行以下功能：
>
> 1. 选择模型中的面，并进行挤出操作。
> 2. 如果直接对面进行挤出失败，尝试先选择面的非共享边，然后进行挤出。
> 3. 使用UF库函数来实现选择、创建列表、创建挤出特征等操作。
> 4. 提供错误报告机制，并在出现UF函数调用错误时输出错误信息。
> 5. 使用UF_print_syslog函数将错误信息打印到系统日志。
> 6. 提供一个卸载函数，用于在用户关闭程序时立即卸载。
> 7. 使用UF_initialize和UF_terminate函数来初始化和终止NX Open。
>
> 这段代码的主要流程包括：
>
> 1. 初始化NX Open。
> 2. 选择模型中的面，创建面的列表。
> 3. 尝试直接对面进行挤出，如果失败，则选择面的非共享边进行挤出。
> 4. 输出错误信息，如果调用UF函数失败。
> 5. 卸载NX Open。
> 6. 终止NX Open。
>
> 总的来说，这段代码提供了一个简单的挤出操作示例，并处理了一些常见问题，如直接对面进行挤出失败的情况。
>
