### 【1285】find gdt features of all visible faces 找到所有可见面的GDT特征

#### 代码

```cpp
    /*HEAD FIND_GDT_FEATURES_OF_ALL_VISIBLE_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_gdt.h>  
    #include <uf_view.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中提到的信息，UF_print_syslog是V18版本中新增的函数。 */  
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
    static tag_t ask_next_visible_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &face)) && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_face_subtype))  
                return face;  
        }  
        return NULL_TAG;  
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
    static int ask_all_visible_faces(tag_t **faces)  
    {  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_create_list(&face_list));  
        while ((face = ask_next_visible_face(face)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(face_list, face));  
        return (make_an_array(&face_list, faces));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_ASSEM_is_occurrence(item))  
            item = UF_ASSEM_ask_prototype_of_occ(item);  
        UF_CALL(UF_OBJ_ask_owning_part(item, &part));  
        UF_PART_ask_part_name(part, partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n,  
            n_feats;  
        tag_t  
            proto_face,  
            *faces,  
            *feats;  
        if ((n = ask_all_visible_faces(&faces)) > 0)  
        {  
            for (ii = 0; ii < n; ii++)  
            {  
                WRITE_D(faces[ii]);  
                if (!UF_CALL(UF_GDT_ask_features_of_face(faces[ii], &feats,  
                    &n_feats)))  
                {  
                    WRITE_D(n_feats);  
                    if (n_feats > 0) UF_free(feats);  
                }  
                if (UF_ASSEM_is_occurrence(faces[ii]))  
                {  
                    proto_face = UF_ASSEM_ask_prototype_of_occ(faces[ii]);  
                    ensure_fully_loaded(proto_face);  
                    WRITE_D(proto_face);  
                    if (!UF_CALL(UF_GDT_ask_features_of_face(proto_face, &feats,  
                        &n_feats)))  
                    {  
                        WRITE_D(n_feats);  
                        if (n_feats > 0) UF_free(feats);  
                    }  
                }  
            }  
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

> 这段代码是用于NX的二次开发，主要功能是遍历当前视图下所有可见的面，并查询每个面的特征信息。
>
> 主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于输出函数调用失败的信息。
> 2. 定义ask_next_visible_face函数，用于遍历视图下所有可见的面。
> 3. 定义allocate_memory函数，用于申请内存。
> 4. 定义make_an_array函数，用于将列表转换为数组。
> 5. 定义ask_all_visible_faces函数，用于获取所有可见面的数组。
> 6. 定义write_integer_to_listing_window函数，用于输出整数值到列表窗口。
> 7. 定义report_load_status函数，用于报告加载状态。
> 8. 定义ensure_fully_loaded函数，用于确保面所属的part完全加载。
> 9. 定义do_it函数，用于遍历所有可见面，并查询每个面的特征信息。
> 10. 定义ufusr函数，作为NX的入口函数，调用do_it函数。
> 11. 定义ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了遍历视图下所有可见面，并获取每个面的特征信息的功能。
>
