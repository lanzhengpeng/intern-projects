### 【1307】find wave linked faces 找到波纹连接的曲面

#### 代码

```cpp
    /*HEAD FIND_WAVE_LINKED_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18中的新增功能，请仅提供翻译，不要添加任何评论。

UF_print_syslog在V18版本中是新增的功能。 */  
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
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            status,  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            status = UF_OBJ_ask_status(face);  
            if ((subtype == UF_solid_face_subtype) && (status == UF_OBJ_ALIVE))  
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
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        printf("%d = %s\n", feat, string);  
        UF_free(string);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            face = NULL_TAG,  
            *feats,  
            part = UF_PART_ask_display_part();  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        while ((face = ask_next_face(part, face)) != NULL_TAG)  
        {  
            if (UF_ASSEM_is_occurrence(face)) continue;  
            UF_CALL(UF_MODL_ask_face_feats(face, &feat_list));  
            n_feats = make_an_array(&feat_list, &feats);  
            for (ii = 0; ii < n_feats; ii++)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feats[ii], &type));  
                if (!strcmp(type, "LINKED_FACE"))  
                {  
                    report_feature_name(feats[ii]);  
                    UF_CALL(UF_DISP_set_highlight(face, TRUE));  
                }  
                UF_free(type);  
            }  
            UF_free(feats);  
        }  
        uc1601("linked faces are highlighted", TRUE);  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是用于在NX中查找并高亮显示链接面特征的二次开发代码。具体功能包括：
>
> 1. 定义错误处理函数：report_error()用于打印错误信息，便于调试。
> 2. 遍历零件中的面：ask_next_face()函数用于遍历零件中的每个面，并返回下一个未处理的实体面。
> 3. 内存分配和释放：allocate_memory()和make_an_array()用于分配和释放内存，用于存储特征列表。
> 4. 特征名称打印：report_feature_name()用于打印特征的名称。
> 5. 遍历每个面：do_it()函数遍历每个面，并调用UF_MODL_ask_face_feats()获取面的特征列表。
> 6. 特征类型判断：通过UF_MODL_ask_feat_type()判断特征类型是否为"LINKED_FACE"。
> 7. 高亮显示：若为链接面特征，则调用UF_DISP_set_highlight()进行高亮显示。
> 8. 主函数：ufusr()初始化NX，调用do_it()执行查找并高亮显示链接面特征，然后结束NX初始化。
> 9. 卸载函数：ufusr_ask_unload()用于卸载二次开发。
>
> 总的来说，这段代码通过遍历零件中的每个面，获取面的特征列表，并判断特征类型，将链接面特征进行高亮显示，以方便用户识别。
>
